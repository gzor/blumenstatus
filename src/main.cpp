#include "main.h"
#include "connect.h"
#include "plant.h"
#include "planthandler.cpp"

#ifdef CO2SENSOR
    FastRunningMedian<unsigned long, 10, 0> co2ppmMedian;
#endif

DHT_Unified dht(dhtPin, DHTTYPE);
float Temp = 0;
float Hum = 0;

Plant plant1(MOISTPIN,relayPin1);

void setup()
{
	Serial.begin(115200);
	dht.begin();
	plant1.init();
	#ifdef CO2SENSOR
		pinMode(mhZ14aPIN, INPUT);
	#endif
	#ifdef LEDPIN
		pinMode(LEDPIN, OUTPUT);
	#endif
}

void loop()
{
	GetDHTSensorData();
	//ReadCO2Sensor();
	#ifdef CO2SENSOR
		ReadCO2Sensor10times();
	#endif
	float moisture = plant1.handle();
	// readMoistureSensor();
	// ActivateLedIfWaterNeeded(moisture);
	// if(MoistureToLow(moisture))
	// 	wasserMarsch(moisture);
	Connect T{};
	#ifdef CO2SENSOR
		T.SendData(Temp, Hum, moisture, co2ppmMedian.getMedian());
	#else
		T.SendData(Temp, Hum, moisture, 0);
	#endif
	//delay(60000);
	esp_sleep_enable_timer_wakeup(SECONDS_TO_SLEEP * uS_TO_S_FACTOR);
	//esp_deep_sleep_start();
	delay(500);
}

#ifdef CO2SENSOR
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
#endif

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
