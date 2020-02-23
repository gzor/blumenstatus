# Retrieving Sensord data and sending it to thingspeak
![CI](https://github.com/gzor/blumenstatus/workflows/CI/badge.svg)

This project uses an ESP32 to retrieve data from a different sensors and post them to thingspeak.
Things you need to adjust, if you want to use this code:
 - Wifi and thingspeak credentials. They can be modified in the file platformio.ini (build_flags)
 - Sensor Pins. Currently located in pindefinition.h

 The plan for this project is to make a plant watering station, which waters depending on the current need. 


Currently working sensors: 
 - MH-Z14a - CO2 Sensor (PWM)
 - DHT22 (One wire)
 - Moisture Sensor (Analog)

