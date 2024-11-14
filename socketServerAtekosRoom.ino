#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ArduinoJson.h>
#include <WebSocketsClient_Generic.h>
#include <Hash.h>

// Configuraciones de WiFi y WebSocket
ESP8266WiFiMulti WiFiMulti;
WebSocketsClient webSocket;

// Definimos el LED integrado en la ESP8266
#define LED_BUILTIN_PIN 2 // En algunas placas puede ser diferente, verifica el pin exacto
#define pin 0


// Constantes de configuración
const char* SSID = "MONTES";
const char* PASSWORD = "Casalinda";

// Configuración para el entorno de desarrollo
#ifdef DEV_ENV
  const char* WS_SERVER = "192.168.1.20"; // IP de desarrollo
  const uint16_t WS_PORT = 3012; // Puerto de desarrollo
#else
  // Configuración para el entorno de producción
  const char* WS_SERVER = "www.atekoss.com";
  const uint16_t WS_PORT = 443; // Puerto seguro en producción
#endif

const char* USER_ID = "1"; // Cambia según sea necesario
const char* ROOM_ID = "1"; // Cambia según sea necesario

bool alreadyConnected = false;

// Función para enviar mensaje de identificación
void sendIdentifyMessage() {
  DynamicJsonDocument doc(1024);
  doc["type"] = "identify";
  doc["userId"] = USER_ID;

  String output;
  serializeJson(doc, output);
  webSocket.sendTXT(output);
}

// Función para enviar mensaje de suscripción al evento `room.update`
void sendSubscribeMessage() {
  DynamicJsonDocument doc(1024);
  doc["type"] = "subscribe";
  doc["event"] = "room.update";
  doc["room"] = ROOM_ID;

  String output;
  serializeJson(doc, output);
  webSocket.sendTXT(output);
}

// Función para manejar los eventos de WebSocket
void webSocketEvent(WStype_t type, uint8_t* payload, size_t length) {
  switch (type) {
    case WStype_DISCONNECTED:
      Serial.println("[WSc] Disconnected!");
      alreadyConnected = false;
      break;

    case WStype_CONNECTED:
      alreadyConnected = true;
      Serial.println("[WSc] Connected to WebSocket server.");
      sendIdentifyMessage(); // Enviar mensaje de identificación al conectarse
      break;

    case WStype_TEXT: {
      Serial.printf("[WSc] Received text: %s\n", payload);

      // Parseamos el JSON recibido
      DynamicJsonDocument doc(1024);
      DeserializationError error = deserializeJson(doc, payload);

      if (!error) {
        const char* eventType = doc["event"];

        if (strcmp(eventType, "room.update") == 0) {
          bool state = doc["data"]["state"];
          
          // Controlamos el LED según el estado recibido
          digitalWrite(LED_BUILTIN_PIN, state ? LOW : HIGH); // LOW enciende y 
          digitalWrite(pin, state ? LOW : HIGH); // encender LED por defecto HIGH apaga en la ESP8266
          Serial.printf("LED %s\n", state ? "ON" : "OFF");
        }
      } else {
        Serial.println("Error al parsear JSON");
      }
      break;
    }

    case WStype_BIN:
      Serial.printf("[WSc] Received binary data of length: %u\n", length);
      break;

    default:
      break;
  }
}

void setup() {
  Serial.begin(115200);
  delay(200);

  // Configuración del LED integrado
  pinMode(LED_BUILTIN_PIN, OUTPUT);
  pinMode(pin, OUTPUT);

  digitalWrite(LED_BUILTIN_PIN, LOW); // encender LED por defecto
  digitalWrite(pin, LOW); // encender LED por defecto

  // Conexión WiFi
  WiFiMulti.addAP(SSID, PASSWORD);
  while (WiFiMulti.run() != WL_CONNECTED) {
    Serial.print(".");
    delay(100);
  }
  Serial.println();

  // Información de IP y servidor
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
  Serial.print("Connecting to WebSocket server at ");
  Serial.print(WS_SERVER);
  Serial.print(":");
  Serial.println(WS_PORT);

  // Configuración del WebSocket
#ifdef DEV_ENV
  webSocket.begin(WS_SERVER, WS_PORT, "/"); // WebSocket sin SSL en desarrollo
#else
  webSocket.beginSSL(WS_SERVER, WS_PORT, "/"); // SSL en producción
#endif

  webSocket.onEvent(webSocketEvent);
  webSocket.setReconnectInterval(5000);
  webSocket.enableHeartbeat(15000, 3000, 2);
}

void loop() {
  webSocket.loop();

  // Enviar el mensaje de suscripción si ya está conectado e identificado
  if (alreadyConnected) {
    sendSubscribeMessage();
    alreadyConnected = false; // Evitar enviar la suscripción repetidamente
  }
}
