#pragma once
#include <Adafruit_VL6180X.h>
#include<VL53L4CD.h>
#include <Arduino.h>

#define indicatorLED D0




class Sensor
{
private:

    Adafruit_VL6180X& traySensor;
    VL53L4CD& doorSensor;
    uint8_t phoneDistance=0;
    uint8_t doorDistance=0;

    const uint16_t DOOR_OPEN_THRESHOLD=1150; //Measured the door partially open threshold with tape measure. Works in tight parking spots
    const uint16_t TRAY_EMPTY_THRESHOLD=150/2;  //half of the tray distance...something is in there
    const uint16_t SUN_AMBIENT_THRESHOLD=15000; //ambient kcps: direct sun >20k, shade <10k
    bool lastDoorState=false;   //last confirmed door state (true=open)
    const uint8_t DOOR_OPEN_CONFIRM=4;  //fresh OPEN readings in a row before we believe it (~0.8s at 200ms timing)
    uint8_t openCount=0;        //consecutive OPEN readings so far

public:
    Sensor(Adafruit_VL6180X& ts, VL53L4CD& ds);
    ~Sensor();
    bool readPhonedist();   //true when phone present in tray
    bool readDoordist();    //true when door is open



};

