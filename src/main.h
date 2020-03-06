
#pragma once
#include "pindefintion.h"
#include <cmath>
#include <DHT.h>
#include <DHT_U.h>
#ifdef CO2SENSOR
    #include "../lib/runningfastmedian.h"
#endif

void ReadCO2Sensor10times();
void GetDHTSensorData();

void readBodenfeuchte();




RTC_DATA_ATTR int currentRelayTest = 0;

#ifdef CO2SENSOR
    FastRunningMedian<unsigned long, 10, 0> co2ppmMedian;
#endif

DHT_Unified dht(dhtPin, DHTTYPE);
float Temp = 0;
float Hum = 0;
float bodenFeuchte = 0;

#define uS_TO_S_FACTOR 1000000 /* Conversion factor for micro seconds to seconds */
//#define TIME_TO_SLEEP 45       /* Time ESP32 will go to sleep (in seconds) */
