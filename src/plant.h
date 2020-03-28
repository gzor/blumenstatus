class Plant
{
    private:
        int moistureSensorPin;
        int RelayPin;

        bool MoistureToLow(float moisture);
        void ActivateLedIfWaterNeeded(float bodenFeuchte);
        float readMoistureSensor();
        void wasserMarsch(float moisture);

    public:
        Plant()
        {
            
        }
        void init();
        void handle();
}