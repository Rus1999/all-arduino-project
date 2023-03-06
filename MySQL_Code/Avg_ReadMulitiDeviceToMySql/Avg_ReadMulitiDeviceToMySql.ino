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
  WiFiManager wm;
  //reset saved settings
  //wm.resetSettings();

  wm.autoConnect("AutoConnectAP");
  Serial.println("connected...yeey :)");
}

void loop() {
  String id_device = "D2"; //กำหนดรหัสอุปกรณ์ เพื่อใช้ในการส่งค่า
  // Read temperature as Celsius (the default)
  //float temp = dht.readTemperature();
  //float humidity = dht.readHumidity();
  float sum_t=0;      //ผลรวมอุณภูมิ
  float sum_h=0;      //ผลรวมความชื้น
  float avg_t=0;    //ค่าเฉลี่ยอุณภูมิ
  float avg_h=0;    //ค่าเฉลี่ยความชื้น
  int c=0;      //ตัวนับรอบสำหรับวนรอบอ่านค่าจาก Sensor
  float temp=0,humidity=0;   //เก็บค่าอุณหภูมิ และความชื้น
  for(c=0;c<10;c++)
  {
    temp=random(20,40);
    humidity=random(20,40);
    //นำค่าที่อ่านไปรวม
    sum_t = sum_t + temp;
    sum_h = sum_h + humidity;
  }
  avg_t = sum_t / 10;   //หาค่าเฉลี่ย  เลข 10 คือ จำนวนรอบในการอ่านข้อมูล
  avg_h = sum_h / 10;   //หาค่าเฉลี่ย  เลข 10 คือ จำนวนรอบในการอ่านข้อมูล
  temp = avg_t;
  humidity = avg_h;
  //------จบการหาค่าเฉลี่ย-----------------------
        
  Serial.print("Temperature : ");
  Serial.println(temp);
  Serial.print("Humidity : ");
  Serial.println(humidity);
  Serial.print("ID_Device : ");
  Serial.println(id_device);

  //---------------------------------------
  HTTPClient http;
  //192.168.1.26 คือ ip ของเครื่องคอมพิวเตอร์ ที่จำลองเป็น Server วิธีตรวจสอบ (ipconfig)
  //ArduinoToMySql คือ โฟเดอร์เก็บ Code php (เปลี่ยนแปลงได้)
  //add_multi.php คือ ไฟล์ Code php

  http.begin("http://192.168.1.26/ArduinoToMySql/add_multi.php");      //HTTP
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");
  int httpCode = http.POST("id_device=" + id_device + "&temp=" + temp + "&humidity=" + humidity);

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
