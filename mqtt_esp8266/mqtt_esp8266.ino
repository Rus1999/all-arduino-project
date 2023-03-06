#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "DHT.h"

const char* ssid = "rus";
const char* password = "77777777";
const char* mqtt_server = "broker.hivemq.com";

WiFiClient espClient;
PubSubClient client(espClient);
unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE	(50)
char msg[MSG_BUFFER_SIZE];
int value = 0;

#define DHTTYPE DHT11 
const int DHT1Pin = D1; // D1 5
const int DHT2Pin = D2; // D5 14
DHT dht1(DHT1Pin, DHTTYPE);
DHT dht2(DHT2Pin, DHTTYPE);

void setup_wifi() {

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  // Switch on the LED if an 1 was received as first character
  if ((char)payload[0] == '1') {
    digitalWrite(D0, HIGH);   // Turn the LED on (Note that LOW is the voltage level
    // but actually the LED is on; this is because
    // it is active low on the ESP-01)
  } else {
    digitalWrite(D0, LOW);  // Turn the LED off by making the voltage HIGH
  }

}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish("rm/led", "hello world");
      // ... and resubscribe
      client.subscribe("rm/led");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup() {
  pinMode(D0, OUTPUT);
  pinMode(D1, INPUT);
  pinMode(D2, INPUT);
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  dht1.begin();
  dht2.begin();
}

void loop() {
  delay(1000);

  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  unsigned long now = millis();
  
  if (now - lastMsg > 2000) {
    lastMsg = now;
    Serial.println("--------------------------");

    Serial.println("Publish message:");
    // Serial.println(msg);

    float h1 = dht1.readHumidity();
    float t1 = dht1.readTemperature();

    // Computes temperature values in Celsius
    float hic1 = dht1.computeHeatIndex(t1, h1, false);
    static char temperatureTemp1[7];
    dtostrf(hic1, 6, 2, temperatureTemp1);
    
    static char humidityTemp1[7];
    dtostrf(h1, 6, 2, humidityTemp1);

    // Publishes Temperature and Humidity values
    client.publish("rm/temp1", temperatureTemp1);
    client.publish("rm/humid1", humidityTemp1);


    delay(1000);

    
    // *****************************
    float h2 = dht2.readHumidity();
    float t2 = dht2.readTemperature();

    // Computes temperature values in Celsius
    float hic2 = dht2.computeHeatIndex(t2, h2, false);
    static char temperatureTemp2[7];
    dtostrf(hic2, 6, 2, temperatureTemp2);
    
    static char humidityTemp2[7];
    dtostrf(h2, 6, 2, humidityTemp2);

    // Publishes Temperature and Humidity values
    client.publish("rm/temp2", temperatureTemp2);
    client.publish("rm/humid2", humidityTemp2);
    
    Serial.print("Sensor 1: ");
    Serial.print("Humidity: ");
    Serial.print(h1);
    Serial.print(" %\t Temperature: ");
    Serial.print(t1);
    Serial.println(" *C ");

    Serial.print("Sensor 2: ");
    Serial.print("Humidity: ");
    Serial.print(h2);
    Serial.print(" %\t Temperature: ");
    Serial.print(t2);
    Serial.println(" *C ");
  }
  
  delay(1000);
}
