#include <Wire.h>
#include <WiFi.h>
#include <PubSubClient.h>

// Configuración WiFi y MQTT
const char* ssid = "POCO X3 NFC";
const char* password = "wzdig263";
const char* mqttServer = "mqtt.eclipseprojects.io";
const int mqttPort = 1883;

WiFiClient espClient;
PubSubClient client(espClient);

// Configuración I2C
#define I2C_SDA 21
#define I2C_SCL 22
#define ARDUINO_ADDRESS 8

void setup() {
  Serial.begin(115200);

  Wire.begin(I2C_SDA, I2C_SCL);

  // Configurar WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Conectando al WiFi...");
  }
  Serial.println("WiFi conectado");

  // Configurar MQTT
  client.setServer(mqttServer, mqttPort);
  client.setCallback(mqttCallback);
  reconnect();
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  // Leer datos del Arduino
  readFromArduino();
  delay(2000);
}

// Leer datos del Arduino por I2C
void readFromArduino() {
  Wire.requestFrom(ARDUINO_ADDRESS, 32);
  String data = "";
  while (Wire.available()) {
    char c = Wire.read();
    data += c;
  }

  if (data.length() > 0) {
    Serial.println("Datos recibidos del Arduino: " + data);
    client.publish("feeding-data", data.c_str());
  }
}

// Manejar comandos MQTT
void mqttCallback(char* topic, byte* payload, unsigned int length) {
  String message = "";
  for (int i = 0; i < length; i++) {
    message += (char)payload[i];
  }
  Serial.println("Comando recibido por MQTT: " + message);

  // Enviar comando al Arduino
  Wire.beginTransmission(ARDUINO_ADDRESS);
  Wire.write((const uint8_t*)message.c_str(), message.length());
  Wire.endTransmission();
}

// Reconectar a MQTT
void reconnect() {
  while (!client.connected()) {
    Serial.println("Conectando a MQTT...");
    if (client.connect("ESP32Client")) {
      Serial.println("Conectado a MQTT");
      client.subscribe("comandos");
    } else {
      delay(5000);
    }
  }
}