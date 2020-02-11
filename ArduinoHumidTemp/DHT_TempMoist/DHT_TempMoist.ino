#include <DHT.h>
/*
  I used this for pinouts: http://www.circuitbasics.com/how-to-set-up-the-dht11-humidity-sensor-on-an-arduino/
  And this for code and library: https://github.com/adafruit/DHT-sensor-library

*/
//sdht DHT;

#define DHT11_PIN 7
#define DHTTYPE DHT11

DHT dht(DHT11_PIN, DHTTYPE);


void setup() {
  Serial.begin(9600);
  Serial.println(F("DHTxx test!"));
  dht.begin();
}

void loop()
{

  float h = dht.readHumidity();
  float t = dht.readTemperature();
  float f = dht.readTemperature(true);
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  float hif = dht.computeHeatIndex(f, h);
  float hic = dht.computeHeatIndex(t, h, false);

  Serial.print(F("Temperature  = "));
  Serial.print(t);
  Serial.print(F("°C"));
  Serial.print(F("\nTemperature  = "));
  Serial.print(f);
  Serial.print(F("°F"));

  Serial.print("\nHumidity  = ");
  Serial.print(h);

  Serial.print(F("%\nHeat index: "));
  Serial.print(hic);
  Serial.print(F("°C "));
  Serial.print(hif);
  Serial.println(F("°F\n*******************************************"));

  delay(1000);



}
