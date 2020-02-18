/*
  POST with headers client for ArduinoHttpClient library
  Connects to server once every five seconds, sends a POST request
  with custome headers and a request body

  created 14 Feb 2016
  by Tom Igoe
  modified 18 Mar 2017
  by Sandeep Mistry
  modified 22 Jan 2019
  by Tom Igoe
  
  this example is in the public domain
 */
#include <ArduinoHttpClient.h>
#include <WiFiEsp.h>
#ifndef HAVE_HWSERIAL1
#include "SoftwareSerial.h"
SoftwareSerial Serial1(6, 7); // RX, TX
#endif

#include "arduino_secrets.h"

///////please enter your sensitive data in the Secret tab/arduino_secrets.h
/////// Wifi Settings ///////
char ssid[] = SECRET_SSID;
char pass[] = SECRET_PASS;


char serverAddress[] = "85.24.161.85";  // server address
int port = 5000;

WiFiEspClient wifi;
HttpClient client = HttpClient(wifi, serverAddress, port);
int status = WL_IDLE_STATUS;

void setup() {  
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

  while ( status != WL_CONNECTED) {
    Serial.print("Attempting to connect to Network named: ");
    Serial.println(ssid);                   // print the network name (SSID);

    // Connect to WPA/WPA2 network:
    status = WiFi.begin(ssid, pass);
  }

  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);
}

void loop() {
  Serial.println("making POST request");
  //String postData = "name=Alice&age=12";
  String content = "Avföring från ett bibliotek";
  content = "Credentials=ARDUINO_BAJS&data="+String(content);
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

  Serial.println("Wait five seconds");
  delay(5000);
}
