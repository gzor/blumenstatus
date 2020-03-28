#pragma once
class Plant
{
    private:
        int _moistureSensorPin;
        int _relayPin;

        bool MoistureToLow(float moisture);
        void ActivateLedIfWaterNeeded(float bodenFeuchte);
        float readMoistureSensor();
        void wasserMarsch(float moisture);
  

    public:
        Plant(int moistureSensorPin, int relayPin )
        {
            _moistureSensorPin = moistureSensorPin;
            _relayPin = relayPin;
        }
        void init();
        // Returns the moisture
        float handle();
};