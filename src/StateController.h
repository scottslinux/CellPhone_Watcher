#pragma once

#include <Arduino.h>
#include <ESP_I2S.h>
#include "Sensor.h"

enum class STATES {initializing, monitoring, alarming};

class StateController
{
private:
    //give StateController access to the objects from main
    I2SClass& player;
    Sensor& sensors;

    STATES Program_State=STATES::initializing;  //start off in the initialize state


    ulong lastScan;
    ulong scanInterval=200;     //update scan 5xsecond

    bool startupFlag=false;

    bool doorstate=false;   //doorstate true when open
    bool traystate=false;   //traystate true when phone in place
    bool phonePlaced=false; //only respond to phone placement once (reset when phone is lifted
    bool warningMSGflag=false; //phone left behind=true




public:
    StateController(I2SClass& i2S, Sensor& snsrs);
    ~StateController();

    void manageStates();

};

