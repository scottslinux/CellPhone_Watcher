#pragma once
#include <Adafruit_VL6180X.h>
#include<VL53L4CD.h>
#include <Arduino.h>




class Sensor
{
private:

    Adafruit_VL6180X& traySensor;
    VL53L4CD& doorSensor;
    uint8_t phoneDistance=0;
    uint8_t doorDistance=0;

    const uint16_t DOOR_OPEN_THRESHOLD=900*1.2; //120% of the door closed position is open
    const uint16_t TRAY_EMPTY_THRESHOLD=150/2;  //half of the tray distance...something is in there

public:
    Sensor(Adafruit_VL6180X& ts, VL53L4CD& ds);
    ~Sensor();
    bool readPhonedist();   //true when phone present in tray
    bool readDoordist();    //true when door is open



};

