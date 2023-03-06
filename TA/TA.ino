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

//hc-sr04-01
const int TRIG1 = D1;
const int ECHO1 = D0;
const int TRIG2 = D3;
const int ECHO2 = D2;

#define DHTPIN D8
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

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
  if ((char)payload[0] == '0') {
    digitalWrite(D7, HIGH); 
  } else if ((char)payload[0] == '1')  {
    digitalWrite(D7, LOW); 
  } else if ((char)payload[0] == '2')  {
    digitalWrite(D8, HIGH); 
  } else if ((char)payload[0] == '3')  {
    digitalWrite(D8, LOW); 
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
  pinMode(D4, OUTPUT); // r1
  pinMode(D5, OUTPUT); //r2
  pinMode(TRIG1, OUTPUT);
  pinMode(ECHO1, INPUT);
  pinMode(TRIG2, OUTPUT);
  pinMode(ECHO2, INPUT);
  pinMode(D7, OUTPUT); // yellow
  pinMode(D8, OUTPUT); // red
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void loop() {
  delay(500);

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

    // hc-sr04-1 ********************************
    digitalWrite(TRIG1, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIG1, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG1, LOW);
    float duration1 = pulseIn(ECHO1, HIGH);

    float distance1 = duration1 / 58.2;

    static char distanceCM1[7];
    dtostrf(distance1, 6, 2, distanceCM1);

    client.publish("rm/dis1", distanceCM1);

    Serial.print("Distance1: ");
    Serial.println(distance1);

    // hc-sr04-2 ******************************
    digitalWrite(TRIG2, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIG2, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG2, LOW);
    float duration2 = pulseIn(ECHO2, HIGH);

    float distance2 = duration2 / 58.2;

    static char distanceCM2[7];
    dtostrf(distance2, 6, 2, distanceCM2);

    client.publish("rm/dis2", distanceCM2);

    Serial.print("Distance2: ");
    Serial.println(distance2);

    if (distance1 < 10)
    {
      client.publish("rm/disSt1", "ON");
      digitalWrite(D4, 0);
    } else
    {
      client.publish("rm/disSt1", "OFF");
      digitalWrite(D4, 1);
    }

    if (distance2 < 10)
    {
      client.publish("rm/disSt2", "ON");
      digitalWrite(D5, 0);
    } else
    {
      client.publish("rm/disSt2", "OFF");
      digitalWrite(D5, 1);
    }
  }
}