#include <ThingSpeak.h>
#include <WiFi.h>

void ConnectToWiFi();
void SendData(float,float,float,long);
void InterpretWriteToCloudReturnValue(int);

// Change these variables ** start
const char *ssid = "***REMOVED***";
const char *password = "***REMOVED***";
//thingspeak credentials
const unsigned long myChannelNumber = ***REMOVED***;
const char *myWriteAPIKey = "***REMOVED***";
WiFiClient client;