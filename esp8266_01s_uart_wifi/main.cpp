#include <ESP8266WiFi.h>
#include <PubSubClient.h>

// ========= Wi-Fi =========
const char* ssid     = "SEU_SSID";
const char* password = "SUA_SENHA";

// ========= MQTT =========
const char* mqtt_host = "192.168.1.10";
const uint16_t mqtt_port = 1883;

// Se precisar autenticação, preencha:
const char* mqtt_user = nullptr;   // ex.: "user"
const char* mqtt_pass = nullptr;   // ex.: "pass"

// Identidade do dispositivo (use algo único)
const char* device_id = "node1";

// Tópicos
String topic_pub = String("sensors/") + device_id + "/telemetry";
String topic_sub = String("sensors/") + device_id + "/cmd";

// ========= UART (única do ESP-01S) =========
const uint32_t UART_BAUD = 115200; // se tiver ruído/cabo longo, tente 57600 ou 38400

// ========= Clientes =========
WiFiClient wifiClient;
PubSubClient mqtt(wifiClient);

// Buffer de linha UART
String lineBuf;

// Timers de reconexão (não-bloqueante)
unsigned long lastWiFiAttempt = 0;
unsigned long lastMqttAttempt = 0;

// Estado para avisos
wl_status_t lastWiFiStatus = WL_DISCONNECTED;
bool lastMqttConnected = false;

static void sendToMCU(const String& s) {
  // Canal de controle/telemetria para o MCU.
  // Se você quiser “canal de dados puro”, remova/controle essas mensagens.
  Serial.println(s);
}

static String ipToString(const IPAddress& ip) {
  return String(ip[0]) + "." + String(ip[1]) + "." + String(ip[2]) + "." + String(ip[3]);
}

void onMqttMessage(char* topic, byte* payload, unsigned int len) {
  // Encaminha comando MQTT para o MCU como linha:
  // CMD:<payload>
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

bool mqttConnectOnce() {
  // clientId único reduz colisões
  String clientId = String("esp01s-") + device_id;

  bool ok;
  if (mqtt_user && mqtt_pass) {
    ok = mqtt.connect(clientId.c_str(), mqtt_user, mqtt_pass);
  } else {
    ok = mqtt.connect(clientId.c_str());
  }
  return ok;
}

void setupMqttNonBlocking() {
  if (WiFi.status() != WL_CONNECTED) return;
  if (mqtt.connected()) return;

  unsigned long now = millis();
  if (now - lastMqttAttempt < 3000) return; // tenta a cada 3s
  lastMqttAttempt = now;

  sendToMCU("MQTT_CONNECTING");
  if (mqttConnectOnce()) {
    mqtt.subscribe(topic_sub.c_str());
    sendToMCU("MQTT_CONNECTED");
    sendToMCU("TOPIC_PUB:" + topic_pub);
  } else {
    sendToMCU("MQTT_CONNECT_FAIL");
  }
}

void publishLineToMqtt(const String& payload) {
  if (!mqtt.connected()) {
    sendToMCU("ERR:MQTT_DOWN");
    return;
  }

  // QoS no PubSubClient é basicamente QoS0 (padrão). Há variações com publish retain.
  bool ok = mqtt.publish(topic_pub.c_str(), payload.c_str());
  if (!ok) sendToMCU("ERR:PUB_FAIL");
}

void setup() {
  Serial.begin(UART_BAUD);
  // Evita prints de debug do core na UART (pode poluir seu protocolo)
  Serial.setDebugOutput(false);

  sendToMCU("ESP01S_BOOT");

  mqtt.setServer(mqtt_host, mqtt_port);
  mqtt.setCallback(onMqttMessage);

  // Opcional: força Wi-Fi não dormir para reduzir latência e quedas (custa consumo)
  WiFi.setSleepMode(WIFI_NONE_SLEEP);
}

void loop() {
  // State machines
  setupWiFiNonBlocking();
  setupMqttNonBlocking();

  // Mantém MQTT vivo
  if (mqtt.connected()) mqtt.loop();

  // Mudança de estado Wi-Fi → avisos
  wl_status_t curWiFi = WiFi.status();
  if (curWiFi != lastWiFiStatus) {
    lastWiFiStatus = curWiFi;
    if (curWiFi == WL_CONNECTED) {
      sendToMCU("WIFI_CONNECTED");
      sendToMCU("IP:" + ipToString(WiFi.localIP()));
    } else {
      sendToMCU("WIFI_LOST");
    }
  }

  // Mudança de estado MQTT → avisos
  bool curMqtt = mqtt.connected();
  if (curMqtt != lastMqttConnected) {
    lastMqttConnected = curMqtt;
    if (!curMqtt) sendToMCU("MQTT_LOST");
  }

  // UART RX: lê linha do MCU e publica
  while (Serial.available()) {
    char c = (char)Serial.read();

    if (c == '\n') {
      if (lineBuf.length()) {
        lineBuf.trim();
        if (lineBuf.length()) publishLineToMqtt(lineBuf);
        lineBuf = "";
      }
    } else if (c != '\r') {
      lineBuf += c;
      if (lineBuf.length() > 512) {
        lineBuf = "";
        sendToMCU("ERR:LINE_TOO_LONG");
      }
    }
  }
}
