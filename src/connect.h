#include <ThingSpeak.h>
#include <WiFi.h>
#include <Arduino.h>

#include <ArduinoOTA.h>


class Connect
{
    private:
    // Change these variables ** start
    const char *ssid = "***REMOVED***";
    const char *password = "***REMOVED***";
    //thingspeak credentials
    const unsigned long myChannelNumber = ***REMOVED***;
    const char *myWriteAPIKey = "***REMOVED***";
    WiFiClient client;

    void ConnectToWiFi();
    void InterpretWriteToCloudReturnValue(int);
    void waitForOtaUpdate();
    
public: 
    void SendData(float,float,float,long);
};
