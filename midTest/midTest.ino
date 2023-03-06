#include <DHT.h>

#define DHTPIN D2
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

void setup() 
{
  Serial.begin(115200);
  dht.begin();
  pinMode(D5, OUTPUT); //blue led
  pinMode(D6, OUTPUT); //red led
}

void loop() 
{
    float temp = dht.readTemperature();
    float humid = dht.readHumidity();
    
    Serial.print("Temperature : ");
    Serial.println(temp);
    Serial.print("Humidity: ");
    Serial.println(humid);

    if(temp > 33)
    {
      digitalWrite(D6, 1);
    }
    else
    {
      digitalWrite(D6, 0);
    }

    if(humid > 55)
    {
      digitalWrite(D5, 1);
    }
    else
    {
      digitalWrite(D5, 0);
    }

    delay(1000);
}
