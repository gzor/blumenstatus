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
Connect T{};
TaskHandle_t OTATask;

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
	T.ConnectToWiFi();
	xTaskCreatePinnedToCore(
      waitForOtaUpdateInfinitly, /* Function to implement the task */
      "OTATask", /* Name of the task */
      150000,  /* Stack size in words */
      NULL,  /* Task input parameter */
      3,  /* Priority of the task */
      &OTATask,  /* Task handle. */
      0); /* Core where the task should run */
}

void loop()
{
	Serial.print("loop() running on core ");
	Serial.println(xPortGetCoreID());
	GetDHTSensorData();
	
	unsigned long co2ppm = 0;
	#ifdef CO2SENSOR
		co2ppm = ReadCO2Sensor10times();
	#endif
	float* moistureArray = handleAllAvailablePlants();
	float moisture = moistureArray[0];
	delete(moistureArray);

	T.SendData(Temp, Hum, moisture, co2ppm);
	//delay(60000);
	esp_sleep_enable_timer_wakeup(SECONDS_TO_SLEEP * uS_TO_S_FACTOR);
	//esp_deep_sleep_start();
	// delay(500);
  vTaskDelay(500/portTICK_PERIOD_MS);
}

#ifdef CO2SENSOR
unsigned long ReadCO2Sensor10times()
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
	return co2ppmMedian.getMedian();
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

#include <ArduinoOTA.h>

void waitForOtaUpdateInfinitly(void * parameter){
  ArduinoOTA
    .onStart([]() {
      String type;
      if (ArduinoOTA.getCommand() == U_FLASH)
        type = "sketch";
      else // U_SPIFFS
        type = "filesystem";
    // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
      Serial.println("Start updating " + type);
    })
    .onEnd([]() {
      Serial.println("\nEnd");
    })
    .onProgress([](unsigned int progress, unsigned int total) {
      Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
    })
    .onError([](ota_error_t error) {
      Serial.printf("Error[%u]: ", error);
      if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
      else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
      else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
      else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
      else if (error == OTA_END_ERROR) Serial.println("End Failed");
    });

  ArduinoOTA.begin();
  Serial.print("OTA() running on core ");
  Serial.println(xPortGetCoreID());
  
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.println("Waiting for OTA");
  
	// wait for firmware update, instead of deepsleep
  for (;;)
  {
      ArduinoOTA.handle();
	  //vtaskDelay is not blocking... core 0 needs to run kernel things like 
	  // wifi handling
	  vTaskDelay(1000/portTICK_PERIOD_MS);

  }
}