# Retrieving Sensord data and sending it to thingspeak

![CI](https://github.com/gzor/blumenstatus/workflows/CI/badge.svg)

This project uses an ESP32 to retrieve data from a different sensors and post them to thingspeak.
Things you need to adjust, if you want to use this code:

- Wifi and thingspeak credentials. They can be modified in the file platformio.ini (build_flags)
- Sensor Pins. Also Buildflags

 The plan for this project is to make a plant watering station, which waters depending on the current need.

I use a analog capacitive moisture sensor.
Calibrate its upper and lower limits per sensor, as they differ greatly from each other.
This programm emits the original read value on the Serial console. \
To calibrate it, submerge the sensor (only the part that can get wet) into water. Note this Value.\
The second value is emitted when the sensor is dry. \
Write the bigger number in MOISTUPPERLIMIT (Compile Flag in Platformio.ini), and the lower in MOISTLOWERLIMIT.

Currently working sensors:

- MH-Z14a - CO2 Sensor (PWM)
- DHT22 (One wire)
- Moisture Sensor (Analog)

To activate the Co2 Sensor you have to set the compile flag `DCO2SENSOR` like this: `'-DCO2SENSOR="ACTIVE"'`
