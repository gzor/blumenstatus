#pragma once
#include <cmath>
#include <DHT.h>
#include <DHT_U.h>
#ifdef CO2SENSOR
    #include "../lib/runningfastmedian.h"
#endif

unsigned long ReadCO2Sensor10times();
void GetDHTSensorData();

void initAllPlants();
float* handleAllAvailablePlants();

void waitForOtaUpdateInfinitly(void * parameter);

#define uS_TO_S_FACTOR 1000000 /* Conversion factor for micro seconds to seconds */
//#define TIME_TO_SLEEP 45       /* Time ESP32 will go to sleep (in seconds) */
