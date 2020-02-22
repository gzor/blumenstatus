#include "main.h"



void setup()
{
	Serial.begin(115200);
	dht.begin();
	pinMode(mhZ14aPIN, INPUT);
	pinMode(feuchtigkeistSensorPin, INPUT);
}

void loop()
{
	GetDHTSensorData();
	//ReadCO2Sensor();
	ReadCO2Sensor10times();
	readBodenfeuchte();
	Connect T{};
	T.SendData(Temp, Hum, bodenFeuchte, co2ppmMedian.getMedian());
	//delay(60000);
	esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);
	esp_deep_sleep_start();
	delay(500);
}

void ReadCO2Sensor10times()
{
	for (int i = 0; i < 10; i++)
	{
		// Serial.print(".");
		while (digitalRead(mhZ14aPIN) == LOW);
		//Serial.print("-");
		long startTime = micros();
		while (digitalRead(mhZ14aPIN) == HIGH)
		{
		}
		long durationHigh = micros() - startTime;
		while (digitalRead(mhZ14aPIN) == LOW)
		{
		}
		long durationLow = micros() - startTime - durationHigh;
		unsigned long co2ppmTemp1 = 2000 * (durationHigh - 2000) / (durationHigh + durationLow - 4000);
		//Serial.print(String(co2ppmTemp1)+", ");
		co2ppmMedian.addValue(co2ppmTemp1);
	}
}

void GetDHTSensorData()
{
	sensors_event_t event;
	dht.temperature().getEvent(&event);
	if (!isnan(event.temperature))
	{
		Temp = event.temperature;
	}
	dht.humidity().getEvent(&event);
	if (!isnan(event.relative_humidity))
	{
		Hum = event.relative_humidity;
	}
}

void readBodenfeuchte()
{
	const int untergrenze = 1650;
	const int obergrenze = 3390;
	// returns value between 0 and 4095
	// 0 is super wet, 4095 is super dry
	double temp = analogRead(feuchtigkeistSensorPin);
	// conversion into %
	// 1- because 0% is defined as dry
	bodenFeuchte = (1.0 - ((temp - untergrenze) / (obergrenze - untergrenze))) * 100;
	Serial.println("bodenfeuchte: " + String(bodenFeuchte) + ", raw value: " + String(temp));
}
