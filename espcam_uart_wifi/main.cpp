#include <WiFi.h>
#include <PubSubClient.h>

// ========= Wi-Fi =========
const char* ssid     = "SEU_SSID";
const char* password = "SUA_SENHA";

// ========= MQTT =========
// Use IP ou hostname do broker (Mosquitto, EMQX, HiveMQ, etc.)
const char* mqtt_host = "192.168.1.10";
const uint16_t mqtt_port = 1883;

// Identificação do dispositivo (use algo único)
const char* device_id = "node1";

// Tópicos
String topic_pub = String("sensors/") + device_id + "/telemetry";
String topic_sub = String("sensors/") + device_id + "/cmd";

// ========= UART para o MCU =========
HardwareSerial MCU(2);
const int MCU_RX = 15;          // RX do ESP32 (vai no TX do MCU)
const int MCU_TX = 14;          // TX do ESP32 (vai no RX do MCU)
const uint32_t MCU_BAUD = 115200;

// ========= Clientes =========
WiFiClient wifiClient;
PubSubClient mqtt(wifiClient);

// Buffer de linha UART
String lineBuf;

// Timers (reconexão não bloqueante)
unsigned long lastWiFiAttempt = 0;
unsigned long lastMqttAttempt = 0;

void sendToMCU(const String& s) {
  MCU.println(s);
}

void onMqttMessage(char* topic, byte* payload, unsigned int len) {
  // Encaminha comando MQTT para o MCU (como linha)
  // Ex: CMD:<payload>
  String msg;
  msg.reserve(len + 8);
  msg = "CMD:";
  for (unsigned int i = 0; i < len; i++) msg += (char)payload[i];
  sendToMCU(msg);
}

void setupWiFiNonBlocking() {
  if (WiFi.status() == WL_CONNECTED) return;

  unsigned long now = millis();
  if (now - lastWiFiAttempt < 5000) return; // tenta a cada 5s
  lastWiFiAttempt = now;

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  sendToMCU("WIFI_CONNECTING");
}

void setupMqttNonBlocking() {
  if (WiFi.status() != WL_CONNECTED) return;
  if (mqtt.connected()) return;

  unsigned long now = millis();
  if (now - lastMqttAttempt < 3000) return; // tenta a cada 3s
  lastMqttAttempt = now;

  sendToMCU("MQTT_CONNECTING");

  // clientId único ajuda em brokers que derrubam conexões duplicadas
  String clientId = String("esp32cam-") + device_id;

  // Se precisar usuário/senha:
  // bool ok = mqtt.connect(clientId.c_str(), "user", "pass");
  bool ok = mqtt.connect(clientId.c_str());

  if (ok) {
    mqtt.subscribe(topic_sub.c_str());
    sendToMCU("MQTT_CONNECTED");
    sendToMCU("TOPIC_PUB:" + topic_pub);
  } else {
    sendToMCU("MQTT_CONNECT_FAIL");
  }
}

void publishLineToMqtt(const String& payload) {
  if (!mqtt.connected()) {
    // opcional: avisar que descartou / ou bufferizar
    sendToMCU("ERR:MQTT_DOWN");
    return;
  }

  // Publica como texto (JSON/CSV/etc.)
  bool ok = mqtt.publish(topic_pub.c_str(), payload.c_str());
  if (!ok) sendToMCU("ERR:PUB_FAIL");
}

void setup() {
  MCU.begin(MCU_BAUD, SERIAL_8N1, MCU_RX, MCU_TX);
  sendToMCU("ESP32CAM_BOOT");

  mqtt.setServer(mqtt_host, mqtt_port);
  mqtt.setCallback(onMqttMessage);
}

void loop() {
  // Wi-Fi / MQTT state machines
  setupWiFiNonBlocking();
  setupMqttNonBlocking();

  // Mantém MQTT vivo
  if (mqtt.connected()) mqtt.loop();

  // Se Wi-Fi conectou recentemente, avisa IP uma vez
  static wl_status_t lastWiFiStatus = WL_DISCONNECTED;
  wl_status_t cur = WiFi.status();
  if (cur != lastWiFiStatus) {
    lastWiFiStatus = cur;
    if (cur == WL_CONNECTED) {
      IPAddress ip = WiFi.localIP();
      sendToMCU("WIFI_CONNECTED");
      sendToMCU("IP:" + String(ip[0]) + "." + String(ip[1]) + "." + String(ip[2]) + "." + String(ip[3]));
    } else {
      sendToMCU("WIFI_LOST");
    }
  }

  // UART RX: lê linha e publica
  while (MCU.available()) {
    char c = (char)MCU.read();

    if (c == '\n') {
      if (lineBuf.length()) {
        // Remove \r, espaços
        lineBuf.trim();
        if (lineBuf.length()) publishLineToMqtt(lineBuf);
        lineBuf = "";
      }
    } else if (c != '\r') {
      lineBuf += c;
      if (lineBuf.length() > 512) { // limite razoável
        lineBuf = "";
        sendToMCU("ERR:LINE_TOO_LONG");
      }
    }
  }
}
