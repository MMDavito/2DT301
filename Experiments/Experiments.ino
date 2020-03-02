/*
 WiFiEsp example: UdpNTPClient

 Get the time from a Network Time Protocol (NTP) time server.
 Demonstrates use of UDP to send and receive data packets
 For more on NTP time servers and the messages needed to communicate with them,
 see http://en.wikipedia.org/wiki/Network_Time_Protocol

 NOTE: The serial buffer size must be larger than 36 + packet size
 In this example we use an UDP packet of 48 bytes so the buffer must be
 at least 36+48=84 bytes that exceeds the default buffer size (64).
 You must modify the serial buffer size to 128
 For HardwareSerial modify _SS_MAX_RX_BUFF in
   Arduino\hardware\arduino\avr\cores\arduino\SoftwareSerial.h
 For SoftwareSerial modify _SS_MAX_RX_BUFF in
   Arduino\hardware\arduino\avr\libraries\SoftwareSerial\SoftwareSerial.h
*/

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

  retStr += (F("%, Heat index: "));
  retStr += (hic);
  retStr += (F("°C "));
  //delay(1000);
  return retStr;
  }
};
//END OF TEMPERATURE CLASS
//START OF DATETIME:
#include "NTPClient.h"
#include <ArduinoHttpClient.h>
#include "WiFiEsp.h"
#include "WiFiEspUdp.h"

// Emulate Serial1 on pins 6/7 if not present
#ifndef HAVE_HWSERIAL1
#include "SoftwareSerial.h"
SoftwareSerial Serial1(6, 7); // RX, TX
#endif

#include "arduino_secrets.h"

char ssid[] = SECRET_SSID;
char pass[] = SECRET_PASS;

int status = WL_IDLE_STATUS;     // the Wifi radio's status

char timeServer[] = "time.nist.gov";  // NTP server
char serverAddress[] = "85.24.161.85";  // server address
int port = 5000;

WiFiEspClient wifi;
HttpClient client = HttpClient(wifi, serverAddress, port);


const long utcOffsetInSeconds = 3600;
char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
// A UDP instance to let us send and receive packets over UDP
WiFiEspUDP Udp;
NTPClient timeClient(Udp, "pool.ntp.org", utcOffsetInSeconds);
Temperature t1 = Temperature();

void setup()
{
  // initialize serial for debugging
  Serial.begin(115200);
  // initialize serial for ESP module
  Serial1.begin(9600);
  // initialize ESP module
  WiFi.init(&Serial1);

  // check for the presence of the shield
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    // don't continue
    while (true);
  }

  // attempt to connect to WiFi network
  while ( status != WL_CONNECTED) {
    Serial.print("Attempting to connect to WPA SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network
    status = WiFi.begin(ssid, pass);
  }

  // you're connected now, so print out the data
  Serial.println("You're connected to the network");
  
  //Udp.begin(localPort);
  timeClient.begin();
  
}

void loop()
{  timeClient.update();
  String temp = t1.getTemperature();
  Serial.println("\n");
  Serial.print(timeClient.getFormattedTime());
  Serial.print("||");
  Serial.print(temp);
  Serial.println("\n");
  String content = "Credentials=ARDUINO_BAJS&data=";
  content += timeClient.getFormattedTime();
  content += "||";
  content += temp;
  Serial.println("CONTENT:");
  Serial.println(content);
  
  client.beginRequest();
  Serial.println("Status before post:");
  Serial.println(wifi.status());
  client.post("/arduino_data");

  //client.post("/");
  Serial.println("DID ERROR HAPPEN?");
  Serial.println(wifi.status());
  
  client.sendHeader("Content-Type", "application/x-www-form-urlencoded");
  client.sendHeader("Content-Length", content.length());
  client.sendHeader("X-Custom-Header", "custom-header-value");
  client.beginBody();
  client.print(content);
  client.endRequest();

  // read the status code and body of the response
  int statusCode = client.responseStatusCode();
  //statusCode = client.responseStatusCode();
  String response = client.responseBody();

  Serial.print("Status code: ");
  Serial.println(statusCode);
  Serial.print("Response: ");
  Serial.println(response);
  
  Serial.println("Wait ten seconds");
  delay(10000);//10 seconds
}
