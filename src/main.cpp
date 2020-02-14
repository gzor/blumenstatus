#include <Arduino.h>
#include "../lib/runningfastmedian.h"
#include <DHT.h>
#include <ThingSpeak.h>
#include <WiFi.h>
#include <DHT_U.h>
#include "main.h"

// Change these variables ** start
const char* ssid     = "***REMOVED***";
const char* password = "***REMOVED***";
//thingspeak credentials
const unsigned long myChannelNumber = ***REMOVED***;
const char * myWriteAPIKey = "***REMOVED***";
const int mhZ14aPIN = 23;
const int dhtPin = 17;

// Change these variables ** stop

WiFiClient client;
FastRunningMedian<unsigned long,10, 0> co2ppmMedian;
DHT_Unified dht(dhtPin, DHT22);
float Temp = 0;
float Hum = 0;

#define uS_TO_S_FACTOR 1000000  /* Conversion factor for micro seconds to seconds */
#define TIME_TO_SLEEP  45        /* Time ESP32 will go to sleep (in seconds) */

void setup()
{
  Serial.begin(115200);
  pinMode(mhZ14aPIN, INPUT);
  ConnectToWiFi();
  ThingSpeak.begin(client);

   Serial.println(F("DHTxx Unified Sensor Example"));
  // Print temperature sensor details.
  sensor_t sensor;
  dht.temperature().getSensor(&sensor);
  Serial.println(F("------------------------------------"));
  Serial.println(F("Temperature Sensor"));
  Serial.print  (F("Sensor Type: ")); Serial.println(sensor.name);
  Serial.print  (F("Driver Ver:  ")); Serial.println(sensor.version);
  Serial.print  (F("Unique ID:   ")); Serial.println(sensor.sensor_id);
  Serial.print  (F("Max Value:   ")); Serial.print(sensor.max_value); Serial.println(F("°C"));
  Serial.print  (F("Min Value:   ")); Serial.print(sensor.min_value); Serial.println(F("°C"));
  Serial.print  (F("Resolution:  ")); Serial.print(sensor.resolution); Serial.println(F("°C"));
  Serial.println(F("------------------------------------"));
  // Print humidity sensor details.
  dht.humidity().getSensor(&sensor);
  Serial.println(F("Humidity Sensor"));
  Serial.print  (F("Sensor Type: ")); Serial.println(sensor.name);
  Serial.print  (F("Driver Ver:  ")); Serial.println(sensor.version);
  Serial.print  (F("Unique ID:   ")); Serial.println(sensor.sensor_id);
  Serial.print  (F("Max Value:   ")); Serial.print(sensor.max_value); Serial.println(F("%"));
  Serial.print  (F("Min Value:   ")); Serial.print(sensor.min_value); Serial.println(F("%"));
  Serial.print  (F("Resolution:  ")); Serial.print(sensor.resolution); Serial.println(F("%"));
  Serial.println(F("------------------------------------"));
}


void loop()
{
  GetDHTSensorData();
  //ReadCO2Sensor();
  ReadCO2Sensor10times();
  SendData();
  //delay(60000);
  esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);
  esp_deep_sleep_start();
  delay(500);
}
void ReadCO2Sensor10times()
{
  unsigned long durationHigh = 0;
  unsigned long durationLow = 0;
  unsigned long co2ppm_med[10];
  for(int i=0;i<10;i++){
    // Serial.print(".");
    while(digitalRead(mhZ14aPIN)==LOW);
    //Serial.print("-");
    long startTime = micros();
    while(digitalRead(mhZ14aPIN)==HIGH);
    long durationHigh = micros() - startTime;
    while(digitalRead(mhZ14aPIN)==LOW);
    long durationLow = micros() - startTime - durationHigh;
    unsigned long co2ppmTemp1 = 2000 * (durationHigh - 2000) / (durationHigh + durationLow - 4000);
    //Serial.print(String(co2ppmTemp1)+", ");
    co2ppmMedian.addValue(co2ppmTemp1);
  }
  //Serial.println("Co2ppm: " + String(co2ppmMedian.getMedian())+" ");
}

void GetDHTSensorData()
{

  sensors_event_t event;
  dht.temperature().getEvent(&event);
    if (isnan(event.temperature)) {
      Serial.println(F("Error reading temperature!"));
    }
    else {
      Serial.print(F("Temperature: "));
      Serial.print(event.temperature);
      Temp = event.temperature;
      Serial.println(F("°C"));
    }
    // Get humidity event and print its value.
    dht.humidity().getEvent(&event);
    if (isnan(event.relative_humidity)) {
      Serial.println(F("Error reading humidity!"));
    }
    else {
      Serial.print(F("Humidity: "));
      Serial.print(event.relative_humidity);
      Hum = event.relative_humidity;
      Serial.println(F("%"));
    }
  
  // Serial.println("Got DHT22  Data");
  // Serial.print("Temperature: " + String(lastValues.temperature) + ", ");
  // Serial.println("Humidity: " + String(lastValues.humidity));
}
void ConnectToWiFi()
{
  WiFi.begin(ssid, password);
  int i = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    i++;
    if(i>50)
    {
      // needs another library
      ESP.restart();
    }
  }
  Serial.println("WiFi connected");
}
void SendData()
{
  if(WiFi.status() != WL_CONNECTED)
  {
    ConnectToWiFi();
  }
  int writeToCloudReturnValue = 0;
  Serial.print("Sending Data to Thingspeak: ");
  if(Temp != 0)
  {
    ThingSpeak.setField(1, Temp);
    ThingSpeak.setField(2, Hum);
  }
  ThingSpeak.setField(4, long(co2ppmMedian.getMedian()));
  writeToCloudReturnValue = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
  if(Temp != 0)
  {
    Serial.print(String(Temp)+", ");
    Serial.print(String(Hum)+", ");
  }
  Serial.print(String(co2ppmMedian.getMedian())+" ");
  Serial.println("Fields sended");  
  InterpretWriteToCloudReturnValue(writeToCloudReturnValue);
  
 delay(500);
}
void InterpretWriteToCloudReturnValue(int writeToCloudReturnValue)
{
  // https://github.com/mathworks/thingspeak-arduino/blob/master/src/ThingSpeak.h
  Serial.print("ThingSpeak.writeFields was: ");
  switch(writeToCloudReturnValue) {
    case 200:
      Serial.println("successful");
      break;
    case 404:
      Serial.println("Incorrect API key (or invalid ThingSpeak server address)");
      break;
    case -101:
      Serial.println("Value is out of range or string is too long (> 255 characters)");
      break;
    case -201:
      Serial.println("Invalid field number specified");
      break;
    case -210:
      Serial.println("setField() was not called before writeFields()");
      break;
    case -301:
      Serial.println("Failed to connect to ThingSpeak");
      break;
    case -302:
      Serial.println("Unexpected failure during write to ThingSpeak");
      break;
    case -303:
      Serial.println("Unable to parse response");
      break;
    case -304:
      Serial.println("Timeout waiting for server to respond");
      break;
    case -401:
      Serial.println("Point was not inserted (most probable cause is the rate limit of once every 15 seconds)");
      break;
    default:
      Serial.println("Returned not what expected: int = " + String(writeToCloudReturnValue));
      break;  
  }
  if(writeToCloudReturnValue != 200)
  {
      if(WiFi.status() == WL_CONNECTED) {
        Serial.println("");
        Serial.println("WiFi connected");
        Serial.println("IP address: ");
        Serial.println(WiFi.localIP());
      }else{
        Serial.println("Wifi not connected!");
      }
  }
}