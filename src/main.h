#include <Arduino.h>
#include "../lib/runningfastmedian.h"
#include <DHT.h>
#include <ThingSpeak.h>
#include <WiFi.h>
#include <DHT_U.h>


void InterpretWriteToCloudReturnValue(int );
void ReadCO2Sensor10times();
void GetDHTSensorData();
void ConnectToWiFi();
void SendData();
void InterpretWriteToCloudReturnValue(int );
void readBodenfeuchte();



// Change these variables ** start
const char* ssid     = "***REMOVED***";
const char* password = "***REMOVED***";
//thingspeak credentials
const unsigned long myChannelNumber = ***REMOVED***;
const char * myWriteAPIKey = "***REMOVED***";
const int mhZ14aPIN = 23;
const int dhtPin = 17;
const int feuchtigkeistSensorPin = 36;

const int relayPin1 = 15;
const int relayPin2 = 2;
const int relayPin3 = 4;
const int relayPin4 = 16;

RTC_DATA_ATTR int currentRelayTest = 0;
// Change these variables ** stop

WiFiClient client;
FastRunningMedian<unsigned long,10, 0> co2ppmMedian;
DHT_Unified dht(dhtPin, DHT22);
float Temp = 0;
float Hum = 0;
float bodenFeuchte = 0;

#define uS_TO_S_FACTOR 1000000  /* Conversion factor for micro seconds to seconds */
#define TIME_TO_SLEEP  45        /* Time ESP32 will go to sleep (in seconds) */
