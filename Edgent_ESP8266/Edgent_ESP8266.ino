
// Fill-in information from your Blynk Template here
#define BLYNK_TEMPLATE_ID "TMPLOyYVsAme"
#define BLYNK_DEVICE_NAME "Exampile"

#define BLYNK_FIRMWARE_VERSION        "0.1.0"

#define BLYNK_PRINT Serial
//#define BLYNK_DEBUG

#define APP_DEBUG

// Uncomment your board, or configure a custom board in Settings.h
//#define USE_SPARKFUN_BLYNK_BOARD
//#define USE_NODE_MCU_BOARD
//#define USE_WITTY_CLOUD_BOARD
//#define USE_WEMOS_D1_MINI

#include "BlynkEdgent.h"
#include "DHT.h"
#define DHTPIN 14 
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

BLYNK_WRITE(V0)
{
  int val = param.asInt();
  digitalWrite(D1, val);
}
void setup()
{
  pinMode(D1, OUTPUT);
  Serial.begin(115200);
  delay(100);
  dht.setup(D5);
  BlynkEdgent.begin();
}

void loop() {
  BlynkEdgent.run();
  delay(dht.getMinimumSamplingPeriod());
  float temperature = dht.getTemperature();
  Serial.print(dht.getStatusString());
  Serial.print(" ");
  Serial.print(temperature, 1);
  Serial.println(" ");
  Blynk.virtualWrite(V1,temperature);
  delay(1000);
}

