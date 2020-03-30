#include "plant.h"
#include "main.h"
RTC_DATA_ATTR bool deactivate_pump = false;

void Plant::init(){
    pinMode(_moistureSensorPin, INPUT);
	pinMode(_relayPin, OUTPUT);
	digitalWrite(_relayPin, relayOut);
}
float Plant::handle()
{
    float moisture = readMoistureSensor();
	ActivateLedIfWaterNeeded(moisture);
	if(MoistureToLow(moisture))
		wasserMarsch(moisture);
    return moisture;
}

bool Plant::MoistureToLow(float moisture)
{
	if(moisture < 25)
		return true;
	else
		return false;
}
void Plant::ActivateLedIfWaterNeeded(float bodenFeuchte)
{
#ifdef LEDPIN
	uint8_t ledOn = HIGH, ledOff = LOW;
	#ifdef REVERTLEDONHIGGROW
		ledOn = LOW;
		ledOff = HIGH;
	#endif
	if(MoistureToLow(bodenFeuchte))
		digitalWrite(LEDPIN, ledOn);
	else
		digitalWrite(LEDPIN, ledOff);
#endif
}

float Plant::readMoistureSensor()
{
	const int untergrenze = MOISTLOWERLIMIT;
	const int obergrenze = MOISTUPPERLIMIT;
	// returns value between 0 and 4095
	// 0 is super wet, 4095 is super dry
	double temp = analogRead(_moistureSensorPin);
	// conversion into %
	// 1- because 0% is defined as dry
	float bodenFeuchte = (1.0 - ((temp - untergrenze) / (obergrenze - untergrenze))) * 100;
	Serial.println("bodenfeuchte: " + String(bodenFeuchte) + ", raw value: " + String(temp));
	return bodenFeuchte;
}

void Plant::wasserMarsch(float moisture)
{
	// Idea: activate pump 15 seconds, wait 15 more seconds, and look if water arrived.
	// If there is no increase in moisture deactivate the pump
	// to ensure the pump is not running constantly if the water stock is empty
	if(!deactivate_pump)
	{
		digitalWrite(_relayPin, relayOn);
		delay(15*1000);
		digitalWrite(_relayPin, relayOut);
		delay(60*1000);
		float moistureAfterPump = readMoistureSensor();
		if(moistureAfterPump <= moisture)
			deactivate_pump = true;
	}
}
