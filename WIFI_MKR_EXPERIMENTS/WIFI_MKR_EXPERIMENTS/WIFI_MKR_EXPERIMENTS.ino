/*
  Web client

  This sketch connects to a website (http://www.google.com)
  using the WiFi module.

  This example is written for a network using WPA encryption. For
  WEP or WPA, change the Wifi.begin() call accordingly.

  This example is written for a network using WPA encryption. For
  WEP or WPA, change the Wifi.begin() call accordingly.

  Circuit:
   Board with NINA module (Arduino MKR WiFi 1010, MKR VIDOR 4000 and UNO WiFi Rev.2)

  created 13 July 2010
  by dlf (Metodo2 srl)
  modified 31 May 2012
  by Tom Igoe

  fubared xx February 2020
  by David Carlsson.
*/

#define ARDUINOJSON_DECODE_UNICODE 1

#include <SPI.h>
#include <WiFiNINA.h>
#include <ArduinoHttpClient.h>
#include <ArduinoJson.h>
//StaticJsonDocument<256> doc;//Earlier "jsonBuffer"
//const size_t capacity = JSON_ARRAY_SIZE(20) + 20*JSON_OBJECT_SIZE(3) + JSON_OBJECT_SIZE(4) + 640;

int api_key = 1337;
String credentials = "BAJS";
int num_pins = NUM_DIGITAL_PINS;

boolean isDynamic = false;
boolean hasStartTime = false;
boolean eachHasStartTime = false;
long currentTime=-1;//Will be sent on hasStartTime as a field in the json.
const long maxTimePerDay = 86400000;  

//long startTimes [NUM_DIGITAL_PINS];
//long durations [NUM_DIGITAL_PINS];
//boolean activeArr [NUM_DIGITAL_PINS];
//boolean isOnArr [NUM_DIGITAL_PINS];

//Variables for static and dynamic:
const int numRelays = 4;//SPENT 2 hours with realising arrays needs constants to know on compile time.FUCK U
byte pinIds       [numRelays];//Can be -1?? if not used??
boolean activeArr [numRelays];

//Variables for dynamic:
long durations    [numRelays];
long startTimes   [numRelays];//For millis....
long startTimesTmp[numRelays];
byte repeatsArr   [numRelays];


//Variables for static:
boolean isOnArr   [numRelays];
/*
  byte pinIds[numRelays]={};//Can be -1?? if not used??
  boolean activeArr[numRelays]={};

  //Variables for dynamic:
  long startTimes[numRelays]={};
  long durations[numRelays]={};
  long startTimes[numRelays]={};//For millis....
  byte repeatsArr[numRelays]={};


  //Variables for static:
  boolean isOnArr[numRelays]={};
*/



#include "arduino_secrets.h"
///////please enter your sensitive data in the Secret tab/arduino_secrets.h
char ssid[] = SECRET_SSID;        // your network SSID (name)
char pass[] = SECRET_PASS;    // your network password (use for WPA, or use as key for WEP)
int keyIndex = 0;            // your network key Index number (needed only for WEP)

int status = WL_IDLE_STATUS;
// if you don't want to use DNS (and reduce your sketch size)
// use the numeric IP instead of the name for the server:
IPAddress server(85, 24, 161, 85); // numeric IP for My server.
int port = 5000;//My server
//char server[] = "www.google.com";    // name address for Google (using DNS)
//int port = 80; //For google.

// Initialize the Ethernet client library
// with the IP address and port of the server
// that you want to connect to (port 80 is default for HTTP):
WiFiClient wifi;
HttpClient client = HttpClient(wifi, server, port);


void setup() {
  //Initialize serial and wait for port to open:

  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  Serial.println(NUM_DIGITAL_PINS);

  // check for the WiFi module:
  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Communication with WiFi module failed!");
    // don't continue
    while (true);
  }

  String fv = WiFi.firmwareVersion();
  if (fv < WIFI_FIRMWARE_LATEST_VERSION) {
    Serial.println("Please upgrade the firmware");
  }

  // attempt to connect to Wifi network:
  //Serial.print("Attempting to connect to SSID: ");
  //status = WiFi.begin(ssid, pass);
  //delay(2000);
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    status = WiFi.begin(ssid, pass);

    // wait 10 seconds for connection:
    delay(10000);
  }
  Serial.println("Connected to wifi");
  printWifiStatus();

  Serial.println("\nStarting connection to server...");
  // if you get a connection, report back via serial:
  if (client.connect(server, port)) {
    Serial.println("connected to server");
    client.beginRequest();
    client.get("/arduino_data");
    client.sendHeader("Credentials: " + String(credentials));
    client.endRequest();
    // read the status code and body of the response
    int statusCode = client.responseStatusCode();
    //statusCode = client.responseStatusCode();
    String response = client.responseBody();

    Serial.print("Status code: ");
    Serial.println(statusCode);
    Serial.print("Response: ");
    Serial.println(response);


    /*
      // Make a HTTP request:
      //client.println("GET /arduino_data HTTP/1.1");
      client.println("GET /search?q=arduino HTTP/1.1");
      client.println("Host: "+String(server));
      client.println("Connection: close");
      client.println();
    */
  }
}

void loop() {
  // if there are incoming bytes available
  // from the server, read them and print them:
  //boolean shize = getRelayInfo(indexes,isOn);
  /*
    for(int i = 0; i<4;i++){
        boolean shize = changeRelays();
        Serial.println("The boolean: "+shize);
        delay(4000);
    }
  */
  Serial.println("Will now http forever");
  // do nothing forevermore:
  //while (true);
  if (isDynamic) {
    Relays_Dynamic();
    boolean shize = loopDynamic();
    Serial.println("WTF!!!!!!!!!!!!!");
    if (!shize) {
      Serial.println("LOOP FOREVER");
      while (true) {}
      Serial.println("The boolean: " + shize);
      if (isDynamic) delay(1000);
    }
  } else {
    boolean shize = Relays_Static();
    Serial.println("The boolean: " + shize);
    if (!shize) {
      Serial.println("LOOP FOREVER");
      while (true) {}
    }
    if (!isDynamic) delay(1000);
  }
}
boolean loopDynamic() {
  for(int i = 0;i<numRelays;i++){
    pinMode(pinIds[i],OUTPUT);
    digitalWrite(pinIds[i],LOW);
    }
  
  byte i = 0;
  startTimesTmp[i] = millis();
  while (true) {
    Serial.println("LOOPING");

    if (!isDynamic) return true;
    //if (!shize) return false;
    //Start of program:
      pinMode(pinIds[i], OUTPUT);
      Serial.println("I: " + String(i) + ", gives duration: " + String(durations[i]));
      Serial.println("I: " + String(i) + ", gives start: " + String(startTimesTmp[i]));
      Serial.println("I: " + String(i) + ", gives runtime: " + String((millis() - startTimesTmp[i])));

      if ((millis() - startTimesTmp[i]) < durations[i]) {
        digitalWrite(pinIds[i], HIGH);
      } else {//Not initlized or duration overflow
        digitalWrite(pinIds[i], LOW);
        if (i >= (numRelays - 1)) {
            startTimesTmp[0] = millis();
            i=0;
            continue;//jump to while
          //i = numRelays;
        } else{ 
          Serial.println("i+1 is: "+String(i+1));
          startTimesTmp[i + 1] = millis();
          Serial.println("With value: "+String(startTimesTmp[i+1]));
          i++; 
          continue;//jump to while.
        }
      }
    delay(1000); //Should replace/refactor this logic of halting execution.....
    boolean shize = Relays_Dynamic();//TODO UNCOMMENT

    //But i dont have any listeners, except maybe should have a button for "axeServerForUpdate"
  }
}
void resetStartTimes() {
  for (byte i = 0; i < numRelays; i++) {
    startTimesTmp[i] = 0;
  }
}

boolean Relays_Dynamic() {
  Serial.println("Will http for dynamic.");
  //Could also print the information to EEPROM to allow for more secure things (useable turning on intervall on disconnect, but that is when i create that!)
  //allows for up to 20 relays(no card have that many)


  if (client.connect(server, port)) {
    Serial.println("connected to server");
    client.beginRequest();
    client.get("/arduino_relay");
    client.sendHeader("Credentials: " + String(credentials));
    client.sendHeader("x-api-key: " + String(api_key));
    client.sendHeader("is_arduino: " + (true));
    client.endRequest();
    // read the status code and body of the response
    int statusCode = client.responseStatusCode();
    //statusCode = client.responseStatusCode();
    String response = client.responseBody();

    Serial.print("Status code: ");
    Serial.println(statusCode);
    Serial.print("Response: ");
    Serial.println(response);
    DynamicJsonDocument doc(1024);//Local variables are destroyed/released when exiting scope..

    auto error = deserializeJson(doc, response);

    if (error) {

      Serial.print(F("deserializeJson() failed: "));
      Serial.println(error.c_str());
      String eMsg = "deserializeJson() failed";
      sendErrorMessage(eMsg);
      return false;
    }//ELSE SUCCESS
    if (!doc["data"]["is_dynamic"]) {
      isDynamic = false;
      return true;//MAYBE RETURN TRUE????TODO
    } //Why this comes after return is reason it needs to be initilized first time
    //Switching from static to dynamic.
    if (doc["data"]["has_start_time"]) {
      hasStartTime = true;
    } else hasStartTime = false;
    if (doc["data"]["each_has_start_time"]) {
      eachHasStartTime = true;
    } else eachHasStartTime = false;
    String bajs = doc["data"]["relays"];
    String skit = doc["data"]["relays"][2];

    Serial.println("DATA length:");
    Serial.println(skit);
    Serial.println("DATA?:");
    Serial.println(bajs);
    Serial.println("Will now print the array:");
    Serial.println(String(sizeof(doc["data"]["relays"]) / sizeof(doc["data"]["relays"][0])));
    Serial.println(String(sizeof(doc["data"]["relays"])));
    Serial.println(String(sizeof(doc["data"]["relays"][0])));

    byte size = 0;
    for (byte i = 0; i < 20; i++) {
      Serial.println("I: " + String(i) + ", gives:");
      String skit = doc["data"]["relays"][i];
      Serial.println(skit);
      if (skit == "null" || i >= numRelays) {
        if (skit != "null" && i >= numRelays) {
          String msg = "Can only use: " + String(numRelays) + ", num of relays";
          sendErrorMessage(msg);
        }
        size = i - 1;
        break;
      }//ELSE IS VALID:
      int pin = doc["data"]["relays"][i]["id"];
      if (pin < 0 || pin > NUM_DIGITAL_PINS) {
        String eMsg = "Pin: " + String(pin) + " does not exist. Pin must be between 0 and " + String(NUM_DIGITAL_PINS - 1);
        sendErrorMessage(eMsg);//then just keep going....
        return false;
      }
      pinIds[i] = pin;
      //TODO UNCOMMENT FOLLOWING!
      //startTimes[i]=doc["data"]["start_time"];//IF EACH HAS NOT STARTTIME IT IS i==0 or nada!
      //TODO CONTINUE
      durations[i]  = doc["data"]["relays"][i]["duration"];
      Serial.println("HERE IS MY DURATION::::: " + String(durations[i]));
      String temp = doc["data"]["relays"][i]["duration"];
      Serial.println("HERE IS MY DURATION2:::::" + temp);
      //repeatsArr[i] = doc["data"]["relays"][i]["repeats"];
      //isActiveArr[i = doc["data"]["relays"][i]["is_active"];//From frontend??
    }
  } else {
    Serial.println("FAIL AT CONNECTING");
    return false;
  }
  return true;
}

/**
   This sets and changes relays acording to relays..
*/
boolean Relays_Static() {
  Serial.println("Will http for static");
  //Could also print the information to EEPROM to allow for more secure things (useable turning on intervall on disconnect, but that is when i create that!)
  //allows for up to 20 relays(no card have that many)


  if (client.connect(server, port)) {
    Serial.println("connected to server");
    client.beginRequest();
    client.get("/arduino_relay");
    client.sendHeader("Credentials: " + String(credentials));
    client.sendHeader("x-api-key: " + String(api_key));
    client.sendHeader("is_arduino: " + (true));
    client.endRequest();
    // read the status code and body of the response
    int statusCode = client.responseStatusCode();
    //statusCode = client.responseStatusCode();
    String response = client.responseBody();

    Serial.print("Status code: ");
    Serial.println(statusCode);
    Serial.print("Response: ");
    Serial.println(response);
    DynamicJsonDocument doc(1024);//Local variables are destroyed/released when exiting scope..

    auto error = deserializeJson(doc, response);

    if (error) {

      Serial.print(F("deserializeJson() failed: "));
      Serial.println(error.c_str());
      String eMsg = "deserializeJson() failed";
      sendErrorMessage(eMsg);
      return false;
    }//ELSE SUCCESS
    if (doc["data"]["is_dynamic"]) {
      isDynamic = true;
      return true;//MAYBE RETURN TRUE????TODO
    }//ELSE:
    String bajs = doc["data"]["relays"];
    String skit = doc["data"]["relays"][2];

    Serial.println("DATA length:");
    Serial.println(skit);
    Serial.println("DATA?:");
    Serial.println(bajs);
    Serial.println("Will now print the array:");
    Serial.println(String(sizeof(doc["data"]["relays"]) / sizeof(doc["data"]["relays"][0])));
    Serial.println(String(sizeof(doc["data"]["relays"])));
    Serial.println(String(sizeof(doc["data"]["relays"][0])));

    byte size = 0;
    for (byte i = 0; i < 20; i++) {
      Serial.println("I: " + String(i) + ", gives:");
      String skit = doc["data"]["relays"][i];
      Serial.println(skit);
      if (skit == "null" || i >= numRelays) {
        if (skit != "null" && i >= numRelays) {
          String msg = "Can only use: " + String(numRelays) + ", num of relays";
          sendErrorMessage(msg);//Could have used for loop until numrelays.
          //But nice to get feedback, and stuffs.
          //Also maybe create some sort of dynamic shit not requirering reflashing device in future...
        }
        size = i - 1;
        break;
      }//ELSE IS VALID:
      int pin = doc["data"]["relays"][i]["id"];
      if (pin < 0 || pin > NUM_DIGITAL_PINS) {
        String eMsg = "Pin: " + String(pin) + " does not exist. Pin must be between 0 and " + String(NUM_DIGITAL_PINS - 1);
        sendErrorMessage(eMsg);//then just keep going....
      }
      //Serial.println("Before pin" + String(pin) + " is initilized: ");
      Serial.println(digitalRead(pin)); //Reads bit pin of register PORTD which contains the current state (high/low) of pin pin.
      pinIds[i] = pin;
      //pinActive
      isOnArr[i] = doc["data"]["relays"][i]["relay_is_on"];

      pinMode(pin, OUTPUT);
      if (doc["data"]["relays"][i]["relay_is_on"]) {
        digitalWrite(pin, HIGH);
      } else {
        digitalWrite(pin, LOW);
      }

      //Serial.println("After pin" + String(pin) + " is initilized: ");
      Serial.println(digitalRead(pin)); //Reads bit pin of register PORTD which contains the current state (high/low) of pin pin.

    }
  } else {
    Serial.println("FAIL AT CONNECTING");
    return false;
  }
  return true;
}

void sendErrorMessage(String msg) {
  String content = "Credentials=ARDUINO_BAJS&data=";
  content += msg;
  client.post("/arduino_data");

  client.sendHeader("Content-Type", "application/x-www-form-urlencoded");
  client.sendHeader("Content-Length", content.length());
  client.sendHeader("X-Custom-Header", "custom-header-value");
  client.beginBody();
  client.print(content);
  client.endRequest();
}
void printWifiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your board's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}
