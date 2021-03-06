#pragma once

#include <WiFi.h>

class Connect
{
    private:
    // Change these variables ** start
    const char *ssid = WIFI_SSID;
    const char *password = WIFI_PASS;
    //thingspeak credentials
    const unsigned long myChannelNumber = ThingSpeakChannelNumber;
    const char *myWriteAPIKey = ThingSpeakWriteAPIKey;
    WiFiClient client;
    int sendfailed;

    void InterpretWriteToCloudReturnValue(int);
    
    
public: 
    void ConnectToWiFi();
    void SendData(float,float,float,long);
    void waitForOtaUpdate();
};
