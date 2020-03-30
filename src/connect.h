#pragma once
#include <ThingSpeak.h>
#include <WiFi.h>
#include <Arduino.h>

#include <ArduinoOTA.h>

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

    void InterpretWriteToCloudReturnValue(int);
    void waitForOtaUpdate();
    
public: 
    void ConnectToWiFi();
    void SendData(float,float,float,long);
};
