#include "connect.h"
void setupConnection()
{
  ConnectToWiFi();
  ThingSpeak.begin(client);
}
void SendData(float temp,float hum, float moist,long co2ppm)
{
  setupConnection();
  int writeToCloudReturnValue = 0;
  Serial.print("Sending Data to Thingspeak: ");
  if (temp != 0)
  {
    ThingSpeak.setField(1, temp);
    ThingSpeak.setField(2, hum);
  }
  if (moist != 0)
  {
    ThingSpeak.setField(3, moist);
  }
  ThingSpeak.setField(4, long(co2ppm));
  writeToCloudReturnValue = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
  if (temp != 0)
  {
    Serial.print(String(temp) + ", ");
    Serial.print(String(hum) + ", ");
  }
  if (moist != 0)
  {
    Serial.print(String(moist) + ", ");
  }
  Serial.print(String(co2ppm) + " ");
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