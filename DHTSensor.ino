#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
#define DHTTYPE    DHT11     // DHT 11
#define DHTPIN 7
DHT_Unified dht(DHTPIN, DHTTYPE);

unsigned long previousMillis = 0;
const long interval = 60000;



void setup() {
  Serial.begin(9600);
  dht.begin();

}

void loop()
{
  unsigned long currentMillis = millis();
  if(currentMillis - previousMillis >= interval){
    previousMillis = currentMillis;

    sensors_event_t event;
    dht.temperature().getEvent(&event);
    Serial.print(F("Temperature: "));
    Serial.print(event.temperature);
    Serial.println(F("°C"));
    dht.humidity().getEvent(&event);
    Serial.print(F("Humidity: "));
    Serial.print(event.relative_humidity);
    Serial.println(F("%"));
  }

}



