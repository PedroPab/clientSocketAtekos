Aquí tienes un `README.md` bien estructurado y visualmente atractivo para tu repositorio de GitHub. 

---

# ESP8266 WebSocket Client con Control de LED y Conexión WiFi

Este proyecto permite a un módulo **ESP8266** conectarse a una red WiFi y a un servidor WebSocket, donde envía mensajes de identificación y suscripción para recibir eventos en tiempo real. A través de estos eventos, se puede controlar el LED integrado del ESP8266.

## 🚀 Características

- Conexión a WiFi usando **ESP8266WiFiMulti**.
- Comunicación en tiempo real con WebSocket para el envío y recepción de mensajes.
- Control del LED integrado y un LED adicional a través de comandos WebSocket.
- Adaptación para entornos de desarrollo y producción mediante configuración condicional.

## 📋 Requisitos

Antes de comenzar, asegúrate de tener lo siguiente:

- Un módulo **ESP8266** (NodeMCU o similar).
- [Arduino IDE](https://www.arduino.cc/en/software) instalado.
- Las siguientes bibliotecas instaladas en el Arduino IDE:
  - [ArduinoJson](https://github.com/bblanchon/ArduinoJson)
  - [WebSockets_Generic](https://github.com/khoih-prog/WebSockets_Generic)
  - [ESP8266WiFi](https://github.com/esp8266/Arduino)

## 🔧 Instalación

1. **Clona el repositorio** o copia el código en tu IDE de Arduino.

   ```bash
   git clone https://github.com/tu-usuario/tu-repositorio.git
   ```

2. **Configura las credenciales de WiFi** en el código:

   ```cpp
   const char* SSID = "TuRedWiFi";
   const char* PASSWORD = "TuContraseña";
   ```

3. **Configura el Servidor WebSocket** en las líneas correspondientes, según si es un entorno de desarrollo o producción:

   ```cpp
   #ifdef DEV_ENV
     const char* WS_SERVER = "192.168.1.20"; // IP de desarrollo
     const uint16_t WS_PORT = 3012;          // Puerto de desarrollo
   #else
     const char* WS_SERVER = "www.atekoss.com"; // Configuración para producción
     const uint16_t WS_PORT = 443;             // Puerto seguro en producción
   #endif
   ```

4. **Sube el código** a tu ESP8266 y abre el monitor serie para ver la salida en tiempo real.

## 🕹️ Uso

### Funcionalidades Principales

- **Identificación**: Una vez conectado, el ESP8266 envía un mensaje de identificación al servidor WebSocket.
- **Suscripción a Eventos**: Al identificarse, el ESP8266 se suscribe al evento `room.update` del servidor.
- **Control de LED**: Dependiendo del estado (`state`) en el evento `room.update`, el LED integrado y un LED adicional se encenderán o apagarán.

### Ejemplo de Comandos WebSocket

Al conectarse, el ESP8266 envía el siguiente mensaje de identificación:

```json
{
  "type": "identify",
  "userId": "1"
}
```

Para suscribirse al evento de actualización de la sala (`room.update`), envía el siguiente mensaje:

```json
{
  "type": "subscribe",
  "event": "room.update",
  "room": "1"
}
```

El servidor puede responder con un mensaje para encender o apagar el LED mediante el siguiente formato:

```json
{
  "event": "room.update",
  "data": {
    "id": "1",
    "state": true
  }
}
```

### Uso en la Vida Real

Este proyecto se puede adaptar para usos como:

- **Notificaciones Visuales**: Recibir notificaciones y señalizarlas a través de LEDs o pantallas.
- **Sistemas de Monitoreo**: Integrarse en sistemas IoT para notificar eventos específicos en tiempo real.
- **Automatización del Hogar**: Integrarse en proyectos de domótica, permitiendo el control de luces y otros dispositivos.

## 🖥️ Configuración en el Entorno de Desarrollo

Para activar el entorno de desarrollo, asegúrate de definir `DEV_ENV` en el código o desde el entorno de compilación. Así, el ESP8266 usará las configuraciones de servidor y puerto correspondientes para el entorno de desarrollo.

```cpp
#define DEV_ENV
```

## 🚨 Depuración

- **Conexión WiFi**: Si el ESP8266 no se conecta a la red, verifica las credenciales de WiFi.
- **Servidor WebSocket**: Asegúrate de que el servidor WebSocket esté activo y en la dirección IP/URL especificada.
- **LED**: Si el LED no responde, verifica las conexiones de hardware y el estado del `Serial Monitor`.

## 📝 Notas Adicionales

- Asegúrate de verificar los pines de tu módulo **ESP8266**, ya que el pin del LED integrado puede variar según el modelo.
- Usa una fuente de alimentación estable para evitar reinicios durante la conexión a WiFi o WebSocket.

## 🖊️ Licencia

Este proyecto está bajo la Licencia MIT. Puedes ver más detalles en el archivo `LICENSE` de este repositorio.

---

¡Si te gusta el proyecto, dale una estrella ⭐ y contribuye!
