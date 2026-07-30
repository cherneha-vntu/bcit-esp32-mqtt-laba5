// ESP32 Firmware for Lab 5 (Variant 24)
// Analog Temperature Sensor (NTC) on GPIO 34
// Photoresistor (LDR) on GPIO 35
// Publishes to broker.hivemq.com

#include <WiFi.h>
#include <PubSubClient.h>

const char* ssid = "Wokwi-GUEST";
const char* password = "";
const char* mqtt_server = "broker.hivemq.com";

WiFiClient espClient;
PubSubClient client(espClient);

const int ntcPin = 34;
const int ldrPin = 35;
const float BETA = 3950; // Beta coefficient of NTC thermistor

void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected");
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Generate unique client ID to prevent collisions
    String clientId = "ESP32Client-Chernega-";
    clientId += String(random(0xffff), HEX);
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  pinMode(ntcPin, INPUT);
  pinMode(ldrPin, INPUT);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  // Read NTC Temperature sensor
  int ntcVal = analogRead(ntcPin);
  float temp = 0;
  if (ntcVal > 0 && ntcVal < 4095) {
    temp = 1 / (log(1 / (4095.0 / ntcVal - 1)) / BETA + 1.0 / 298.15) - 273.15;
  }

  // Read LDR Photoresistor sensor
  int ldrVal = analogRead(ldrPin);
  float light = ldrVal * (100.0 / 4095.0); // Map to 0-100%

  Serial.print("NTC Temp: "); Serial.print(temp);
  Serial.print(" C, LDR Light: "); Serial.print(light);
  Serial.println(" %");

  // Publish readings to corresponding topics
  client.publish("vntu/chernega/temperature", String(temp, 2).c_str());
  client.publish("vntu/chernega/light", String(light, 2).c_str());

  delay(1000);
}
