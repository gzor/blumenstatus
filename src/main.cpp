#include "main.h"

void setup()
{
  Serial.begin(115200);
  dht.begin();
  pinMode(mhZ14aPIN, INPUT);
  pinMode(feuchtigkeistSensorPin, INPUT);

  ConnectToWiFi();
  ThingSpeak.begin(client);
}

void loop()
{
  GetDHTSensorData();
  //ReadCO2Sensor();
  ReadCO2Sensor10times();
  readBodenfeuchte();
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
}
void ConnectToWiFi()
{
  WiFi.begin(ssid, password);
  int i = 0;
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
    i++;
    if (i > 50)
    {
      // needs another library
      ESP.restart();
    }
  }
  Serial.println("WiFi connected");
}
void SendData()
{
  if (WiFi.status() != WL_CONNECTED)
  {
    ConnectToWiFi();
  }
  int writeToCloudReturnValue = 0;
  Serial.print("Sending Data to Thingspeak: ");
  if (Temp != 0)
  {
    ThingSpeak.setField(1, Temp);
    ThingSpeak.setField(2, Hum);
  }
  if (bodenFeuchte != 0)
  {
    ThingSpeak.setField(3, bodenFeuchte);
  }
  ThingSpeak.setField(4, long(co2ppmMedian.getMedian()));
  writeToCloudReturnValue = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
  if (Temp != 0)
  {
    Serial.print(String(Temp) + ", ");
    Serial.print(String(Hum) + ", ");
  }
  if (bodenFeuchte != 0)
  {
    Serial.print(String(bodenFeuchte) + ", ");
  }
  Serial.print(String(co2ppmMedian.getMedian()) + " ");
  Serial.println("Fields sended");
  InterpretWriteToCloudReturnValue(writeToCloudReturnValue);

  delay(500);
}
void InterpretWriteToCloudReturnValue(int writeToCloudReturnValue)
{
  // https://github.com/mathworks/thingspeak-arduino/blob/master/src/ThingSpeak.h
  Serial.print("ThingSpeak.writeFields was: ");
  switch (writeToCloudReturnValue)
  {
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
  if (writeToCloudReturnValue != 200)
  {
    if (WiFi.status() == WL_CONNECTED)
    {
      Serial.println("");
      Serial.println("WiFi connected");
      Serial.println("IP address: ");
      Serial.println(WiFi.localIP());
    }
    else
    {
      Serial.println("Wifi not connected!");
    }
  }
}
void readBodenfeuchte()
{
  bodenFeuchte = analogRead(feuchtigkeistSensorPin);
  Serial.println("bodenfeuchte: " + String(bodenFeuchte));
}