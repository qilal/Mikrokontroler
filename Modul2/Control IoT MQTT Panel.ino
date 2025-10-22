#include <WiFi.h>
#include <PubSubClient.h>

// ===============================
// Konfigurasi WiFi (Hotspot HP)
// ===============================
const char* ssid = "BlackBerry DTEK50";       // ganti dengan SSID WiFi kamu
const char* password = "HELL BOY"; // ganti dengan password WiFi kamu

// ===============================
// Konfigurasi MQTT
// ===============================
const char* mqtt_server = "broker.hivemq.com";
const int mqtt_port = 1883;
const char* topic_control = "iot/motor/control";
const char* topic_speed   = "iot/motor/speed";

// ===============================
// Konfigurasi pin motor
// ===============================
int motor1Pin1 = 27;
int motor1Pin2 = 26;
int enable1Pin = 12;

const int freq = 30000;
const int pwmChannel = 0;
const int resolution = 8;
int dutyCycle = 0;  // nilai PWM 0–255

// LED indikator
int ledPin = 2;

// Objek WiFi & MQTT
WiFiClient espClient;
PubSubClient client(espClient);

// ===============================
// Fungsi koneksi WiFi
// ===============================
void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Menghubungkan ke ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("✅ WiFi terhubung!");
  Serial.print("Alamat IP: ");
  Serial.println(WiFi.localIP());
}

// ===============================
// Callback MQTT
// ===============================
void callback(char* topic, byte* payload, unsigned int length) {
  String message = "";
  for (int i = 0; i < length; i++) {
    message += (char)payload[i];
  }

  Serial.print("Pesan dari [");
  Serial.print(topic);
  Serial.print("]: ");
  Serial.println(message);

  // ==============
  // ON/OFF Control
  // ==============
  if (String(topic) == topic_control) {
    if (message == "1") {
      digitalWrite(ledPin, HIGH);
      digitalWrite(motor1Pin1, HIGH);
      digitalWrite(motor1Pin2, LOW);
      ledcWrite(pwmChannel, dutyCycle);
      Serial.println("🚀 Motor ON");
    } else if (message == "0") {
      digitalWrite(ledPin, LOW);
      digitalWrite(motor1Pin1, LOW);
      digitalWrite(motor1Pin2, LOW);
      ledcWrite(pwmChannel, 0);
      Serial.println("🛑 Motor OFF");
    }
  }

  // ==========================
  // Atur kecepatan via slider
  // ==========================
  if (String(topic) == topic_speed) {
    int speedVal = message.toInt();
    if (speedVal >= 0 && speedVal <= 255) {
      dutyCycle = speedVal;
      ledcWrite(pwmChannel, dutyCycle);
      Serial.print("⚙️ Kecepatan motor: ");
      Serial.println(dutyCycle);
    }
  }
}

// ===============================
// Reconnect MQTT jika terputus
// ===============================
void reconnect() {
  while (!client.connected()) {
    Serial.print("Menghubungkan ke MQTT...");
    if (client.connect("ESP32MotorClient")) {
      Serial.println("terhubung!");
      client.subscribe(topic_control);
      client.subscribe(topic_speed);
    } else {
      Serial.print("Gagal (rc=");
      Serial.print(client.state());
      Serial.println("). Coba lagi dalam 5 detik...");
      delay(5000);
    }
  }
}

// ===============================
// Setup
// ===============================
void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
  pinMode(motor1Pin1, OUTPUT);
  pinMode(motor1Pin2, OUTPUT);

  ledcSetup(pwmChannel, freq, resolution);
  ledcAttachPin(enable1Pin, pwmChannel);

  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
}

// ===============================
// Loop
// ===============================
void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}
