#include "connect.h"

void Connect::SendData(float temp,float hum, float moist,long co2ppm)
{
  ConnectToWiFi();
  ThingSpeak.begin(client);
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
  waitForOtaUpdate();
}
void  Connect::InterpretWriteToCloudReturnValue(int writeToCloudReturnValue)
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

void  Connect::ConnectToWiFi()
{
  WiFi.begin(ssid, password);
  Serial.print("Connect to SSID: " + String(ssid));
  Serial.print("With password: " + String(password));
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
void Connect::waitForOtaUpdate(){
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

  Serial.println("Ready");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  
  ArduinoOTA.handle();
}