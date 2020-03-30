#pragma once
#include <cmath>
#include <esp32-hal-gpio.h>

class Plant
{
    private:
        int _moistureSensorPin;
        int _relayPin;
        

        uint8_t relayOut;
        uint8_t relayOn;

        bool MoistureToLow(float moisture);
        void ActivateLedIfWaterNeeded(float bodenFeuchte);
        float readMoistureSensor();
        void wasserMarsch(float moisture);
  

    public:
        Plant(int moistureSensorPin, int relayPin )
        {
            _moistureSensorPin = moistureSensorPin;
            _relayPin = relayPin;

            relayOut = HIGH;
            relayOn= LOW;
            #ifdef relayHighTrigger
                relayOut = LOW;
                relayOn = HIGH;
            #endif
        }
        void init();
        // Returns the moisture
        float handle();
};