#include "DHT.h"

DHT dht;

#define DHTPIN 0 
#define DHTTYPE DHT11 

const int TRIG = 3;
const int ECHO = 2;

void setup() {
Serial.begin(9600); 
Serial.println();
Serial.println("Status\tHumidity (%)\tTemperature (C)\t(F)");
dht.setup(12);
// HC-SR04
pinMode(TRIG, OUTPUT);
pinMode(ECHO, INPUT);
// Relay
pinMode(7, OUTPUT);
pinMode(6, OUTPUT);

}

void loop() {

  delay(dht.getMinimumSamplingPeriod());

  float humidity = dht.getHumidity(); // ดึงค่าความชื้น
  float temperature = dht.getTemperature(); // ดึงค่าอุณหภูมิ

  Serial.print(dht.getStatusString());
  Serial.print("\t");
  Serial.print(humidity, 1);
  Serial.print("\t\t");
  Serial.print(temperature, 1);
  Serial.print("\t\t");
  Serial.println(dht.toFahrenheit(temperature), 1);

  delay(1000);
  digitalWrite(7, HIGH);
  digitalWrite(6, HIGH);

  // ultra sonic
  long duration, cm;

  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(5);
  digitalWrite(TRIG, LOW);

  duration = pulseIn(ECHO, HIGH);
  
  cm = microsecondsToCentimeters(duration);
  
  Serial.print(cm);
  Serial.print("cm");
  Serial.println();

  // condition
  if (cm < 10)
  {
    digitalWrite(6, LOW);
    Serial.println("LED2 ON");
  }
  else
  {
    digitalWrite(6, HIGH);
    Serial.println("LED2 OFF");
  }

  if (temperature > 25)
  {
    digitalWrite(7, LOW);
    Serial.println("LED1 ON");
  }
  else
  {
    digitalWrite(7, HIGH);
    Serial.println("LED1 OFF");
  }
}
 
long microsecondsToCentimeters(long microseconds)
{
// The speed of sound is 340 m/s or 29 microseconds per centimeter.
// The ping travels out and back, so to find the distance of the
// object we take half of the distance travelled.
return microseconds / 29 / 2;
}