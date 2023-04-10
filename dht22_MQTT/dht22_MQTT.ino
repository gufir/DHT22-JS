#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <DHT.h>

#define DHTPIN 2          // DHT22 signal pin
#define DHTTYPE DHT11      // DHT22 sensor type

// MQTT broker information
const char* mqtt_server = "broker.hivemq.com";
//const char* mqtt_username = "emqx";
//const char* mqtt_password = "public";
const char* mqtt_topic = "sensorrr/suhu";
const char* mqtt_topic1 = "sensorrr/klmbbn";

// WiFi network information
const char* ssid = "ASDE";
const char* password = "1234566678";

WiFiClient espClient;
PubSubClient client(espClient);

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(115200);
  delay(10);
  
  // Connect to WiFi network
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("");
  Serial.println("WiFi connected");
  
  // Connect to MQTT broker
  client.setServer(mqtt_server,1883);
  client.setCallback(callback);
  
  while (!client.connected()) {
    Serial.println("Connecting to MQTT broker...");
    if (client.connect("ESP8266Client")) {
      Serial.println("Connected to MQTT broker");
    } else {
      Serial.print("Failed with state ");
      Serial.print(client.state());
      delay(2000);
    }
  }
  
  // Initialize DHT sensor
  dht.begin();
}

void loop() {

  if (!client.connected()){
    reconnect();
  }
  
  // Read data from DHT sensor
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();
//  Serial.println(temperature);
//  Serial.println(humidity);
  
  // Check if any reading failed
  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("Failed to read data from DHT sensor");
    return;
  }
  
  // Publish data to MQTT broker
  if(client.connected()){
    char tempStr[10];
    char humStr[10];
    dtostrf(temperature, 2, 2, tempStr);
    dtostrf(humidity, 2, 2, humStr);
    client.publish(mqtt_topic, tempStr);
    client.publish(mqtt_topic1, humStr);
    Serial.println("Data suhu terkirim: " + String(tempStr));
    Serial.println("Data kelembaban terkirim: " + String(humStr)); 
  }
  
//  client.publish("temperature", temperatureStr);
//  client.publish("humidity", humidityStr);
}

void callback(char* topic, byte* payload, unsigned int length) {
  // Show Message received
  Serial.println("Pesan yang diterima: ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}

void reconnect() {
  Serial.println("Connecting to MQTT Broker...");
  while (!client.connected()) {
      Serial.println("Reconnecting to MQTT Broker..");
      String clientId = "ESP32Client-";
      clientId += String(random(0xffff), HEX);
      
      if (client.connect(clientId.c_str())) {
        Serial.println("Connected.");
        // subscribe to topic
      }
      
  }
}
