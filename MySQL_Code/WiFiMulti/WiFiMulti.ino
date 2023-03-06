#include <ESP8266WiFiMulti.h>
ESP8266WiFiMulti wifiMulti;
// WiFi connect timeout per AP. Increase when connecting takes longer.
const uint32_t connectTimeoutMs = 5000;

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
  if (wifiMulti.run(connectTimeoutMs) == WL_CONNECTED) {
    Serial.print("WiFi connected: ");
    Serial.print(WiFi.SSID());
    Serial.print(" ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("WiFi not connected!");
  }
  delay(1000);
  //int temp = random(25, 35);
}
