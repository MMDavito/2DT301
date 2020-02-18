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

#include "NTPClient.h"
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



const long utcOffsetInSeconds = 3600;
char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
// A UDP instance to let us send and receive packets over UDP
WiFiEspUDP Udp;
NTPClient timeClient(Udp, "pool.ntp.org", utcOffsetInSeconds);


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
  Serial.println("\n");
  Serial.println(timeClient.getFormattedDate());
  Serial.println("\n");
  Serial.print(daysOfTheWeek[timeClient.getDay()]);
  Serial.print(", ");
  Serial.print(timeClient.getHours());
  Serial.print(":");
  Serial.print(timeClient.getMinutes());
  Serial.print(":");
  Serial.println(timeClient.getSeconds());
  //Serial.println(timeClient.getFormattedTime());

  delay(1000);
}
