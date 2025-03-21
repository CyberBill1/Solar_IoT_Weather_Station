#include <WiFi.h>
#include <PubSubClient.h>
#include <DHT.h>

// WiFi credentials
const char* ssid = "your_wifi_ssid";
const char* password = "your_wifi_password";

// AWS IoT Core
const char* mqtt_server = "your-iot-endpoint.iot.region.amazonaws.com";
const int mqtt_port = 8883;
const char* mqtt_topic = "weather/data";
const char* client_id = "WeatherStation1";  // Change for scalability test

// Certificates (replace with your AWS IoT Core certs)
const char* ca_cert = "-----BEGIN CERTIFICATE-----\n...AmazonRootCA1.pem...\n-----END CERTIFICATE-----";
const char* client_cert = "-----BEGIN CERTIFICATE-----\n...your-cert.pem.crt...\n-----END CERTIFICATE-----";
const char* client_key = "-----BEGIN RSA PRIVATE KEY-----\n...your-private.pem.key...\n-----END RSA PRIVATE KEY-----";

// DHT22 setup
#define DHTPIN 4
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

WiFiClientSecure espClient;
PubSubClient client(espClient);

// Sleep duration (in microseconds, 15 minutes)
#define SLEEP_DURATION 15 * 60 * 1000000

void connectWiFi() {
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connected");
}

void connectMQTT() {
  espClient.setCACert(ca_cert);
  espClient.setCertificate(client_cert);
  espClient.setPrivateKey(client_key);
  client.setServer(mqtt_server, mqtt_port);
  
  while (!client.connect(client_id)) {
    Serial.print("MQTT connection failed, retrying...");
    delay(5000);
  }
  Serial.println("MQTT connected");
}

void setup() {
  Serial.begin(115200);
  dht.begin();

  connectWiFi();
  connectMQTT();

  // Read sensor data
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();
  unsigned long uptime = millis() / 1000;  // Uptime in seconds

  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("Failed to read from DHT sensor!");
    client.publish(mqtt_topic, "{\"error\": \"Sensor failure\"}");
  } else {
    // Create JSON payload
    String payload = "{\"timestamp\": \"" + String(millis()) + 
                     "\", \"temperature\": " + String(temperature, 1) + 
                     ", \"humidity\": " + String(humidity, 1) + 
                     ", \"uptime\": " + String(uptime) + "}";
    client.publish(mqtt_topic, payload.c_str(), true);  // QoS 1 for reliability
    Serial.println("Published: " + payload);
  }

  // Disconnect and sleep
  client.disconnect();
  WiFi.disconnect();
  esp_sleep_enable_timer_wakeup(SLEEP_DURATION);
  esp_deep_sleep_start();
}

void loop() {
  // Empty: ESP32 sleeps after setup
}
