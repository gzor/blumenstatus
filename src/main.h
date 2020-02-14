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



// Change these variables ** start
const char* ssid     = "***REMOVED***";
const char* password = "***REMOVED***";
//thingspeak credentials
const unsigned long myChannelNumber = ***REMOVED***;
const char * myWriteAPIKey = "***REMOVED***";
const int mhZ14aPIN = 23;
const int dhtPin = 17;

// Change these variables ** stop

WiFiClient client;
FastRunningMedian<unsigned long,10, 0> co2ppmMedian;
DHT_Unified dht(dhtPin, DHT22);
float Temp = 0;
float Hum = 0;

#define uS_TO_S_FACTOR 1000000  /* Conversion factor for micro seconds to seconds */
#define TIME_TO_SLEEP  45        /* Time ESP32 will go to sleep (in seconds) */
