#include <DHT.h>
/*
  I used this for pinouts: http://www.circuitbasics.com/how-to-set-up-the-dht11-humidity-sensor-on-an-arduino/
  And this for code and library: https://github.com/adafruit/DHT-sensor-library

*/
//sdht DHT;

#define DHT11_PIN 8
#define DHTTYPE DHT11
DHT dht(DHT11_PIN, DHTTYPE);

class Temperature {
public:
  Temperature(){//SETUP:
    dht.begin();
  }

/*
 * Returns all info (timestamp,temperature,humdity,relativehumidity).
*/
String getTemperature(){

  float h = dht.readHumidity();
  float t = dht.readTemperature();//Celcius
  float f = dht.readTemperature(true);//Fahrenheight
  if (isnan(h) || isnan(t) || isnan(f)) {
    return (F("Failed to read from DHT sensor!"));
  }

  float hif = dht.computeHeatIndex(f, h);
  float hic = dht.computeHeatIndex(t, h, false);

  String retStr = (F("Temperature  = "));
  retStr += (t);
  retStr += (F("°C"));

  retStr += (", Humidity  = ");
  retStr += (h);

  retStr += (F("%\nHeat index: "));
  retStr += (hic);
  retStr += (F("°C "));
  delay(1000);
  return retStr;
  }
};
