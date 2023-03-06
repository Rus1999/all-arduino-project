#include <ESP8266WiFi.h>          //https://github.com/esp8266/Arduino

//needed for library
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>         //https://github.com/tzapu/WiFiManager

#include <ESP8266HTTPClient.h>    //ไลบารีสำหรับใช้ HTTPClient

#include "DHT.h"
#define DHTPIN D3     // Digital pin connected to the DHT sensor
#define DHTTYPE DHT11   // DHT 11
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  //WiFiManager
  WiFiManager wifiManager;

  //reset settings - for testing
  //wifiManager.resetSettings();

  wifiManager.autoConnect("AutoConnectAP");
  Serial.println("connected...yeey :)");
}

void loop() {
  // Read temperature as Celsius (the default)
  float temp = dht.readTemperature();
  float humidity = dht.readHumidity();
  Serial.print("Temperature : ");
  Serial.println(temp);
  Serial.print("Humidity : ");
  Serial.println(humidity);

  //--------------------------------------------
  HTTPClient http;
  //192.168.1.26 คือ ip ของเครื่องคอมพิวเตอร์ ที่จำลองเป็น Server วิธีตรวจสอบ (ipconfig)
  //ArduinoToMySql คือ โฟเดอร์เก็บ Code php (เปลี่ยนแปลงได้)
  //add_2data.php คือ ไฟล์ Code php
  //temp = + String(temp); คือ ตัวแปร และค่าข้อมูล
  //"&humidity=" + String(humidity);  คือ ตัวแปร และค่าข้อมูลตัวที่ 2

  String url = "http://192.168.1.26/ArduinoToMySql/add_2data.php?temp=" + String(temp) + "&humidity=" + String(humidity);
  Serial.println(url);
  http.begin(url); //HTTP //http เริ่มทำงาน

  int httpCode = http.GET(); //ส่งข้อมูลแบบ GET

  if (httpCode > 0) {
    Serial.printf("[HTTP] GET... code: %d\n", httpCode);
    if (httpCode == HTTP_CODE_OK) {
      String payload = http.getString();
      Serial.println(payload);
    }
  } else {
    //กรณีติดต่อกันไม่ได้ เวลาติดต่อต้องอยู่วงเดียวกัน
    Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
  }
  http.end(); //http จบการทำงาน
  delay(5000);

}
