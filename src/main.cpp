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
  SendData(Temp,Hum,bodenFeuchte,co2ppmMedian.getMedian());
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
  for (int i = 0; i < 10; i++)
  {
    // Serial.print(".");
    while (digitalRead(mhZ14aPIN) == LOW)
      ;
    //Serial.print("-");
    long startTime = micros();
    while (digitalRead(mhZ14aPIN) == HIGH)
      ;
    long durationHigh = micros() - startTime;
    while (digitalRead(mhZ14aPIN) == LOW)
      ;
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
  if (isnan(event.temperature))
  {
    Serial.println(F("Error reading temperature!"));
  }
  else
  {
    // Serial.print(F("Temperature: "));
    // Serial.print(event.temperature);
    Temp = event.temperature;
    // Serial.println(F("°C"));
  }
  // Get humidity event and print its value.
  dht.humidity().getEvent(&event);
  if (isnan(event.relative_humidity))
  {
    Serial.println(F("Error reading humidity!"));
  }
  else
  {
    // Serial.print(F("Humidity: "));
    // Serial.print(event.relative_humidity);
    Hum = event.relative_humidity;
    // Serial.println(F("%"));
  }

  // Serial.println("Got DHT22  Data");
  // Serial.print("Temperature: " + String(lastValues.temperature) + ", ");
  // Serial.println("Humidity: " + String(lastValues.humidity));
}


void readBodenfeuchte()
{
  bodenFeuchte = analogRead(feuchtigkeistSensorPin);
  Serial.println("bodenfeuchte: " + String(bodenFeuchte));
}