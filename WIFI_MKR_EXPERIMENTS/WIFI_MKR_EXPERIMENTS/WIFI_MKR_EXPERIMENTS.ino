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
boolean IS_DEBUG = false;

int api_key = 1337;
String credentials = "BAJS";
int num_pins = NUM_DIGITAL_PINS;

boolean isDynamic = false;
boolean hasStartTime = false;
boolean eachHasStartTime = false;
long currentTime = -1; //Will be sent on hasStartTime as a field in the json.
byte numRepeats = -2;
unsigned long totalTimeOn = 0;


long arduinosClock = -1; //Can use this and set it using a function (on update).
//This to support offline and more frequent updates with fever httpRequests.
//But I do not have time for that right now.

long httpIntervall = 4 * 1000; //Every 4 seconds i will http...

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
boolean wifiConnected = false;
boolean serverConnected = false;
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
HttpClient client = HttpClient(wifi, server, port);//It has around 18 second timeout.
//On some errors it will try to send error.
//So it takes around 20+20 seconds before it tries to reestablish wifi and server.


void setup() {
  //Initialize serial and wait for port to open:

  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  if (IS_DEBUG)Serial.println(NUM_DIGITAL_PINS);

  wifiConnected = establishWIFI();
  serverConnected = establishServer();
}


void loop() {
  if (isDynamic) {
    Relays_Dynamic();
    resetAll();
    boolean shize = false;
    if (!hasStartTime) shize = loopDynamic();
    else {
      Serial.println("hej1");
      resetAll();
      Serial.println("hej2");
      resetRepeatsArr();
      resetActiveArr();
      shize = loopStartTimes();
    }
    if (!shize) {
      //if (IS_DEBUG)Serial.println("LOOP FOREVER");
      //if (IS_DEBUG)Serial.println("LOOP FOREVER");

      
      resolveConnection();
    }
  } else {
    boolean shize = Relays_Static();
    if (IS_DEBUG)Serial.println("The boolean: " + shize);
    if (!shize) {
      if (IS_DEBUG)Serial.println("LOOP FOREVER");
      resolveConnection();
    }
    if (!isDynamic) delay(1000);
  }
}
void resetAll() {
  Serial.println("Hallå i resetta");//TODO REMOVE
  /* 
   for (int i = 0; i < numRelays; i++) {
    pinMode(pinIds[i], OUTPUT);
    digitalWrite(pinIds[i], LOW);
    */
    for (int i = 0; i < NUM_DIGITAL_PINS; i++) {
      Serial.println("JÄVLAR I: "+String(i));//TODO REMOVE
    pinMode(i, OUTPUT);
    digitalWrite(i, LOW);
  }
}
/**
   Connection will be made to wifi, then try to post to server.
*/
boolean resolveConnection() {
  wifiConnected = false;
  serverConnected = false;
  resetAll();
  
  status = WL_IDLE_STATUS;
  delay(1000);

  while (!wifiConnected) {
    delay(1000);
    wifiConnected = establishWIFI();
  }
  while (!serverConnected) {
    delay(1000);
    serverConnected = establishServer();
  }
  String msg = "Re-established connection to server.";
  sendErrorMessage(msg);
  return true;
}

/**
   Returns true if connection is established
   else it will loop forever (e.g. if module not connnected)
*/
boolean establishWIFI() {
  // check for the WiFi module:
  if (WiFi.status() == WL_NO_MODULE) {
    if (IS_DEBUG)Serial.println("Communication with WiFi module failed!");
    // don't continue
    while (true);
  }

  String fv = WiFi.firmwareVersion();
  if (fv < WIFI_FIRMWARE_LATEST_VERSION) {
    if (IS_DEBUG)Serial.println("Please upgrade the firmware");
  }

  while (status != WL_CONNECTED) {
    if (IS_DEBUG) {
      Serial.print("Attempting to connect to SSID: ");
      Serial.println(ssid);
    }
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    status = WiFi.begin(ssid, pass);

    // wait 10 seconds for connection:
    delay(10000);
  }
  if (IS_DEBUG)Serial.println("Connected to wifi");
  printWifiStatus();
  return true;

}
/**
   Returns false if 404, else returns true
*/
boolean establishServer() {

  if (IS_DEBUG)Serial.println("\nStarting connection to server...");
  // if you get a connection, report back via serial:
  if (client.connect(server, port)) {
    if (IS_DEBUG)Serial.println("connected to server");
    client.beginRequest();
    client.get("/arduino_data");
    client.sendHeader("Credentials: " + String(credentials));
    client.endRequest();
    // read the status code and body of the response
    int statusCode = client.responseStatusCode();
    //statusCode = client.responseStatusCode();
    String response = client.responseBody();
    if (IS_DEBUG) {
      Serial.print("Status code: ");
      Serial.println(statusCode);
      Serial.print("Response: ");
      Serial.println(response);
    }
    if (statusCode == 404) {
      return false;
    }
    else return true;

  } else return false;
}
boolean loopStartTimes() {
  Serial.println("Skall vi hare trevligt!");
  if (IS_DEBUG)Serial.println("GRAZIAS SENIROREN!");
  boolean printOnce = true;
  while (true) {
    unsigned long currTime = getCurrTime();
    for (byte i = 0; i < numRelays; i++) {
      currTime = getCurrTime();

      unsigned long endT = getEndTime(startTimes[i], durations[i]);
      boolean isInInter = isInIntervall(startTimes[i], endT, currTime);
      if (IS_DEBUG && ((unsigned long)(millis() - (arduinosClock + currentTime))) >= 1000 && printOnce) {
        Serial.println("CurrT: " + String(currTime) + ", startT: " + String(startTimes[i]) + ", endT: " + String(endT) + ", Is in intervall?:" + String(isInInter));
        Serial.println("How bout number1?");
        long tempEndMilli = getEndTime(startTimes[0], durations[0]);
        boolean isInInter2 = isInIntervall(startTimes[0], tempEndMilli, currTime);
        Serial.println("CurrT: " + String(currTime) + ", startT: " + String(startTimes[0]) + ", endT: " + String(tempEndMilli) + ", Is in intervall?:" + String(isInInter2));

        printOnce = false;
      }

      if (isInInter) {
        //if (!activeArr[i]) {
        //if (numRepeats != 0) {
        repeatsArr[i] = 1;
        activeArr[i] = true;
        digitalWrite(pinIds[i], HIGH);
        //}§
        //}
      }//ELSE
      else {
        if (activeArr[i]) {
          activeArr[i] = false;
          totalTimeOn += durations[i];
          sendDurationOn(totalTimeOn);//Should handle this more precise, like currenttime in starttimesTmp or someshite
          //if (eachHasStartTime) activeArr[i] = false;

        }
        digitalWrite(pinIds[i], LOW);
      }
    } if (((unsigned long)(millis() - (arduinosClock + currentTime))) >= httpIntervall) {
          Serial.println("LOOPING IN STARTTIMES");

      if (IS_DEBUG) {
        Serial.println("Time before: " + String(currTime));
        Serial.println("##################################################################################################################");
      }
      boolean shize = Relays_Dynamic();
      printOnce = true;
      if (!isDynamic || !hasStartTime) {
        return true;
      }
      if (!shize && (!wifiConnected || !serverConnected))return false;
      if (IS_DEBUG) Serial.println("******************************************************************************************************************");
      currTime = getCurrTime();
      if (IS_DEBUG)Serial.println("Time after: " + String(currTime));

    }
  }
}
boolean isInIntervall(unsigned long startT, unsigned long endT, unsigned long currT) {
  if (startT == endT) return false;
  else if (startT < endT) {
    if (currT >= startT && currT <= endT)return true;
    else return false;
  } else if (startT > endT) {
    if (currT > maxTimePerDay) return false; //Should never happen since one should ask getTime();
    else if (currT >= startT) return true;
    else if (currT <= endT) return true;
    else return false;// should never happen!
  } else return false;
}
unsigned long getEndTime(unsigned long start, unsigned long duration) {
  unsigned long endTime = (unsigned long)(start + duration);
  if ( endTime >= maxTimePerDay) {
    endTime = currentTime - maxTimePerDay;
  }
  return endTime;
}
/**
   Offset is what was recived from server.
*/
void updateArdinoClock(long offset) {
  currentTime = offset;
  if (IS_DEBUG) Serial.println("Will here update arduinos clock."); //TODO IN FUTURE
  if (offset < 0) {
    String msg = "clock was tried to be updated when not real";
    sendErrorMessage(msg);
    return;
  }//else:
  unsigned long currentMillis = millis();
  arduinosClock = ((unsigned long)(currentMillis - offset));
}

unsigned long getCurrTime() {
  unsigned long currentTime = (unsigned long)(millis() - arduinosClock);
  if ( currentTime >= maxTimePerDay) {
    currentTime = currentTime - maxTimePerDay;
  }
  //Serial.println(String(currentTime));
  return currentTime;
}
void resetRepeatsArr() {
  for (byte i = 0; i < numRelays; i++) {
    repeatsArr[i] = 0;
  }
  return;
}
void resetActiveArr() {
  for (byte i = 0; i < numRelays; i++) {
    activeArr[i] = false;
  }
  return;
}

boolean loopDynamic() {
  for (int i = 0; i < numRelays; i++) {
    pinMode(pinIds[i], OUTPUT);
    digitalWrite(pinIds[i], LOW);
  }

  byte i = 0;
  startTimesTmp[i] = millis();
  while (true) {
    Serial.println("LOOPING IN DYNAMIC");
    if (IS_DEBUG)Serial.println("LOOPING");

    if (!isDynamic || hasStartTime) return true;
    //Start of program:
    pinMode(pinIds[i], OUTPUT);
    if (IS_DEBUG) {
      Serial.println("I: " + String(i) + ", gives duration: " + String(durations[i]));
      Serial.println("I: " + String(i) + ", gives start: " + String(startTimesTmp[i]));
      Serial.println("I: " + String(i) + ", gives runtime: " + String((millis() - startTimesTmp[i])));
    }
    if ((millis() - startTimesTmp[i]) < durations[i]) {
      digitalWrite(pinIds[i], HIGH);
    } else {//Not initlized or duration overflow
      digitalWrite(pinIds[i], LOW);
      if (i >= (numRelays - 1)) {
        startTimesTmp[0] = millis();
        i = 0;
        continue;//jump to while
        //i = numRelays;
      } else {
        if (IS_DEBUG)   Serial.println("i+1 is: " + String(i + 1));
        startTimesTmp[i + 1] = millis();
        if(IS_DEBUG)Serial.println("With value: " + String(startTimesTmp[i + 1]));
        i++;
        continue;//jump to while.
      }
    }
    delay(1000); //Should replace/refactor this logic of halting execution.....
    boolean shize = Relays_Dynamic();//TODO UNCOMMENT
    if (!shize && (!wifiConnected || !serverConnected))return false;
    //But i dont have any listeners, except maybe should have a button for "axeServerForUpdate"
  }
}
void resetStartTimes() {
  for (byte i = 0; i < numRelays; i++) {
    startTimesTmp[i] = 0;
  }
}

boolean Relays_Dynamic() {
  if (IS_DEBUG)Serial.println("Will http for dynamic.");
  //Could also print the information to EEPROM to allow for more secure things (useable turning on intervall on disconnect, but that is when i create that!)
  //allows for up to 20 relays(no card have that many)


  if (client.connect(server, port)) {
    if (IS_DEBUG)Serial.println("connected to server");
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
    if (IS_DEBUG) {
      Serial.print("Status code: ");
      Serial.println(statusCode);
      Serial.print("Response: ");
      Serial.println(response);
    }
    DynamicJsonDocument doc(2048);//Local variables are destroyed/released when exiting scope..

    auto error = deserializeJson(doc, response);

    if (error) {
      if (IS_DEBUG) {
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(error.c_str());
      }
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
    long tempTime = doc["data"]["current_time"];
    updateArdinoClock(tempTime);
    numRepeats = doc["data"]["num_repeats"];
    String bajs = doc["data"]["relays"];
    String skit = doc["data"]["relays"][2];
    if (IS_DEBUG) {
      Serial.println("DATA length:");
      Serial.println(skit);
      Serial.println("DATA?:");
      Serial.println(bajs);
      Serial.println("Will now print the array:");
      Serial.println(String(sizeof(doc["data"]["relays"]) / sizeof(doc["data"]["relays"][0])));
      Serial.println(String(sizeof(doc["data"]["relays"])));
      Serial.println(String(sizeof(doc["data"]["relays"][0])));
    }
    byte size = 0;
    for (byte i = 0; i < 20; i++) {
      //Serial.println("I: " + String(i) + ", gives:");
      String skit = doc["data"]["relays"][i];
      //Serial.println(skit);
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
      startTimes[i] = doc["data"]["relays"][i]["start_time"];

      //Serial.println("HERE IS MY DURATION::::: " + String(durations[i]));
      String temp = doc["data"]["relays"][i]["duration"];
      //Serial.println("HERE IS MY DURATION2:::::" + temp);
      //isActiveArr[i = doc["data"]["relays"][i]["is_active"];//From frontend??
    }
  } else {
    if (IS_DEBUG)   Serial.println("FAIL AT CONNECTING");
    wifiConnected = false;
    serverConnected = false;
    return false;
  }
  return true;
}

/**
   This sets and changes relays acording to relays..
*/
boolean Relays_Static() {
  if (IS_DEBUG)Serial.println("Will http for static");
  //Could also print the information to EEPROM to allow for more secure things (useable turning on intervall on disconnect, but that is when i create that!)
  //allows for up to 20 relays(no card have that many)


  if (client.connect(server, port)) {
    if (IS_DEBUG)Serial.println("connected to server");
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
    if (IS_DEBUG) {
      Serial.print("Status code: ");
      Serial.println(statusCode);
      Serial.print("Response: ");
      Serial.println(response);
    }
    DynamicJsonDocument doc(2048);//Local variables are destroyed/released when exiting scope..

    auto error = deserializeJson(doc, response);

    if (error) {
      if (IS_DEBUG) {
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(error.c_str());
      }
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
    if (IS_DEBUG) {
      Serial.println("DATA length:");
      Serial.println(skit);
      Serial.println("DATA?:");
      Serial.println(bajs);
      Serial.println("Will now print the array:");
      Serial.println(String(sizeof(doc["data"]["relays"]) / sizeof(doc["data"]["relays"][0])));
      Serial.println(String(sizeof(doc["data"]["relays"])));
      Serial.println(String(sizeof(doc["data"]["relays"][0])));
    }
    byte size = 0;
    for (byte i = 0; i < 20; i++) {
      if (IS_DEBUG)     Serial.println("I: " + String(i) + ", gives:");
      String skit = doc["data"]["relays"][i];
      if (IS_DEBUG)     Serial.println(skit);
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
      if (IS_DEBUG)Serial.println(digitalRead(pin)); //Reads bit pin of register PORTD which contains the current state (high/low) of pin pin.
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
      if (IS_DEBUG)Serial.println(digitalRead(pin)); //Reads bit pin of register PORTD which contains the current state (high/low) of pin pin.

    }
  } else {
    if (IS_DEBUG)Serial.println("FAIL AT CONNECTING");
    wifiConnected = false;
    serverConnected = false;
    return false;
  }
  return true;
}
/**
   Sends the duration in hours and seconds the relays have been on (for "testing") purposes around middnight (utc)
   makes millis into hours,minutes and seconds.
*/
void sendDurationOn(long onFor) {
  int sec = (int) (onFor / 1000) % 60;
  int minutes = (int)(onFor / (60 * 1000)) % 60;
  long hours = (int) (onFor / (1000 * 60 * 60));
  String msg = "On for: " + String(hours) + ":" + String(minutes) + ":" + String(sec);
  sendErrorMessage(msg);
  return;
}
boolean sendErrorMessage(String msg) {
  String content = "Credentials=ARDUINO_BAJS&data=";
  content += msg;

  client.beginRequest();
  client.post("/arduino_data");
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
  if (IS_DEBUG) {
    Serial.print("Status code: ");
    Serial.println(statusCode);
    Serial.print("Response: ");
    Serial.println(response);
  }
  if (statusCode != 201)return false;
  else return true;
}
void printWifiStatus() {
  // print the SSID of the network you're attached to:
  if (IS_DEBUG) {
    Serial.print("SSID: ");
    Serial.println(WiFi.SSID());
  }

  // print your board's IP address:
  IPAddress ip = WiFi.localIP();
  if (IS_DEBUG) {
    Serial.print("IP Address: ");
    Serial.println(ip);
  }
  // print the received signal strength:
  long rssi = WiFi.RSSI();
  if (IS_DEBUG) {
    Serial.print("signal strength (RSSI):");
    Serial.print(rssi);
    Serial.println(" dBm");
  }
}
