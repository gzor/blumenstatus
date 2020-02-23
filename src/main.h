
#pragma once
#include <cmath>
#include <DHT.h>
#include <DHT_U.h>
#include "../.pio/libdeps/esp32doit-devkit-v1/Adafruit Unified Sensor_ID31/Adafruit_Sensor.h"
#include "../lib/runningfastmedian.h"

void ReadCO2Sensor10times();
void GetDHTSensorData();

void readBodenfeuchte();


const int mhZ14aPIN = 23;
const int dhtPin = 17;
const int feuchtigkeistSensorPin = 36;

const int relayPin1 = 15;
const int relayPin2 = 2;
const int relayPin3 = 4;
const int relayPin4 = 16;

RTC_DATA_ATTR int currentRelayTest = 0;



FastRunningMedian<unsigned long, 10, 0> co2ppmMedian;
DHT_Unified dht(dhtPin, DHT22);
float Temp = 0;
float Hum = 0;
float bodenFeuchte = 0;

#define uS_TO_S_FACTOR 1000000 /* Conversion factor for micro seconds to seconds */
#define TIME_TO_SLEEP 45       /* Time ESP32 will go to sleep (in seconds) */
