#include "main.h"
#include "connect.h"
#include "plant.h"
#include "pindefintion.h"

#ifdef CO2SENSOR
    FastRunningMedian<unsigned long, 10, 0> co2ppmMedian;
#endif

DHT_Unified dht(dhtPin, DHTTYPE);
float Temp = 0;
float Hum = 0;

void setup()
{
	Serial.begin(115200);
	dht.begin();
	initAllPlants();
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
	float* moistureArray = handleAllAvailablePlants();
	float moisture = moistureArray[0];
	delete(moistureArray);

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

#ifdef MOISTPIN1
    Plant plant1(MOISTPIN1,relayPin1);
#endif
#ifdef MOISTPIN2
    Plant plant2(MOISTPIN2,relayPin2);
#endif
#ifdef MOISTPIN2
    Plant plant3(MOISTPIN3,relayPin3);
#endif
#ifdef MOISTPIN2
    Plant plant4(MOISTPIN4,relayPin4);
#endif

void initAllPlants()
{
    #ifdef MOISTPIN1
        plant1.init();
    #endif
    #ifdef MOISTPIN2
        plant2.init();
    #endif
    #ifdef MOISTPIN3
        plant3.init();
    #endif
    #ifdef MOISTPIN4
        plant4.init();
    #endif
}
float* handleAllAvailablePlants()
{
    float* moistureValues = new float[4];
    #ifdef MOISTPIN1
        moistureValues[0] = plant1.handle();
    #endif
    #ifdef MOISTPIN2
        moistureValues[1] = plant2.handle();
    #endif
    #ifdef MOISTPIN3
        moistureValues[2] = plant3.handle();
    #endif
    #ifdef MOISTPIN4
        moistureValues[3] = plant4.handle();
    #endif
    return moistureValues;
}