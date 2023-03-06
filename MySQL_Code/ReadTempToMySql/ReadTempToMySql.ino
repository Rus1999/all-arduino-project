#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
ESP8266WiFiMulti wifiMulti;
#include <ESP8266HTTPClient.h>    //ไลบารีสำหรับใช้ HTTPClient

// WiFi connect timeout per AP. Increase when connecting takes longer.
const uint32_t connectTimeoutMs = 5000;

#include "DHT.h"
#define DHTPIN D3     // Digital pin connected to the DHT sensor
#define DHTTYPE DHT11   // DHT 11
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  // Don't save WiFi configuration in flash - optional
  WiFi.persistent(false);
  Serial.begin(115200);
  Serial.println("\nESP8266 Multi WiFi example");
  // Set WiFi to station mode
  WiFi.mode(WIFI_STA);

  // Register multi WiFi networks
  //HomeYN_2.4GHz  ชื่อ WiFi(SSID)
  //02179987   รหัสเข้าใช้งาน WiFi
  wifiMulti.addAP("HomeYN_2.4GHz", "02179987");
  // More is possible
}
void loop() {
  // Maintain WiFi connection
  if (wifiMulti.run() == WL_CONNECTED) {
    // Read temperature as Celsius (the default)
    float temp = dht.readTemperature();
    Serial.print("Temperature : ");
    Serial.println(temp);

    //int temp1 = random(25, 35);

    //--------------------------------------------
    HTTPClient http;
    //192.168.1.26 คือ ip ของเครื่องคอมพิวเตอร์ ที่จำลองเป็น Server วิธีตรวจสอบ (ipconfig)
    //ArduinoToMySql คือ โฟเดอร์เก็บ Code php (เปลี่ยนแปลงได้)
    //add_temp.php คือ ไฟล์ Code php
    //temp = + String(temp); คือ ตัวแปร และค่าข้อมูล
    
    String url = "http://192.168.1.26/ArduinoToMySql/add_temp.php?temp=" + String(temp);
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

  } else {
    Serial.println("WiFi not connected!");
  }
  delay(1000);
}
