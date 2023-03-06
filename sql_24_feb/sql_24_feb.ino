#include <FS.h>                   //this needs to be first, or it all crashes and burns...
#include <ESP8266WiFi.h>          //https://github.com/esp8266/Arduino
//needed for library
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>          //https://github.com/tzapu/WiFiManager
#include <WiFiClient.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
#include "DHT.h"

WiFiClient espClient;
ESP8266WiFiMulti wifiMulti;

#define DHTPIN D3     // Digital pin connected to the DHT sensor
#define DHTTYPE DHT11   // DHT 11
DHT dht(DHTPIN, DHTTYPE);

const int TRIG = D5;
int ECHO = D6;

const char* ssid = "rus";
const char* password = "77777777";

int light = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println();

  //WiFiManager
  //Local intialization. Once its business is done, there is no need to keep it around
  WiFiManager wifiManager;

  //exit after config instead of connecting
  wifiManager.setBreakAfterConfig(true);
  

  //reset settings - for testing
  wifiManager.resetSettings();


  //tries to connect to last known settings
  //if it does not connect it starts an access point with the specified name
  //here  "AutoConnectAP" with password "password"
  //and goes into a blocking loop awaiting configuration
  if (!wifiManager.autoConnect("AutoConnectAP", "password")) {
    Serial.println("failed to connect, we should reset as see if it connects");
    delay(3000);
    ESP.reset();
    delay(5000);
  }

  //if you get here you have connected to the WiFi
  Serial.println("connected...yeey :)");


  Serial.println("local ip");
  Serial.println(WiFi.localIP());

  pinMode(D3, INPUT);
  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);
  dht.begin();
}

void loop() {
  Serial.println("--------------------------");
  // ***********************************************************************************
  delay(1000);
  float temp = dht.readHumidity();
  float humidity = dht.readTemperature();
  // float temp=random(20,40);
  // float humidity=random(20,40);
  delay(1000);
  
  Serial.print("Sensor 1: ");
  Serial.print("Humidity: ");
  Serial.print(temp);
  Serial.print(" %\t Temperature: ");
  Serial.print(humidity);
  Serial.println(" *C ");

  // ***********************************************************************************
  Serial.println("--------------------------");
  long duration, cm;
  
  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(5);
  digitalWrite(TRIG, LOW);
  duration = pulseIn(ECHO, HIGH);
  
  cm = microsecondsToCentimeters(duration);
  
  Serial.print("Length: ");
  Serial.print(cm);
  Serial.print("cm");
  Serial.println();
  delay(100);

  // ***********************************************************************************
  Serial.println("--------------------------");
  light = analogRead(A0);
  Serial.print("Light: ");
  Serial.println(light);
  
  // ***********************************************************************************
  Serial.println("--------------------------");
  HTTPClient http;

  http.begin(espClient, "http://192.168.43.183/esp8266/dht_sonic_light/add.php");
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");

  int httpCode = http.POST("temp=" + String(temp) + "&humidity=" + String(humidity) + "&length=" + String(cm) + "&light=" + String(light));

  if (httpCode > 0) 
  {
    Serial.printf("[HTTP] GET... code: %d\n", httpCode);
    if (httpCode == HTTP_CODE_OK) 
    {
      String payload = http.getString();
      Serial.println(payload);
    }
  }
  else 
  {
    Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
  }

  http.end();
  delay(5000);
}

long microsecondsToCentimeters(long microseconds)
{
// The speed of sound is 340 m/s or 29 microseconds per centimeter.
// The ping travels out and back, so to find the distance of the
// object we take half of the distance travelled.
return microseconds / 29 / 2;
}