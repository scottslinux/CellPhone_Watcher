#include "Sensor.h"
#include "Arduino.h"

//-------------------------------------------------
// pass in the sensor object from main
Sensor::Sensor(Adafruit_VL6180X& ts, VL53L4CD& ds):traySensor(ts), doorSensor(ds)
{

}
//-------------------------------------------------

Sensor::~Sensor()
{

}
//-------------------------------------------------
//   ⁡⁣⁢⁣𝗥𝗲𝗮𝗱 𝘁𝗵𝗲 𝗣𝗵𝗼𝗻𝗲 𝗗𝗶𝘀𝘁𝗮𝗻𝗰𝗲 𝗶𝗻 𝘁𝗵𝗲 𝘁𝗿𝗮𝘆⁡

bool Sensor::readPhonedist()
{
    uint8_t vlOffset=-25;   //set distance correction


    uint8_t phoneDistance=traySensor.readRange()+ vlOffset;

    uint8_t status=traySensor.readRangeStatus();



    if (status==VL6180X_ERROR_NONE || status==7)  //if out of range assume there is no phone
    {
        Serial.printf("Tray Distance: %d mm  %s\n",phoneDistance,(phoneDistance<TRAY_EMPTY_THRESHOLD)?"PRESENT":"ABSCENT");
        return((phoneDistance<TRAY_EMPTY_THRESHOLD)); //⁡⁢⁣⁣𝘁𝗵𝗶𝘀 𝗲𝘃𝗮𝗹𝘂𝗮𝘁𝗲𝘀 𝘁𝗼 𝘁𝗿𝘂𝗲 𝗼𝗿 𝗳𝗮𝗹𝘀𝗲⁡
    }
    else
    {

        Serial.printf("Error reading range, status %d\n",status);
        return (false);
    }
}
//-------------------------------------------------
//   ⁡⁣⁢⁣𝗥𝗲𝗮𝗱 𝘁𝗵𝗲 𝗗𝗼𝗼𝗿 𝗗𝗶𝘀𝘁𝗮𝗻𝗰𝗲 

bool Sensor::readDoordist()
{
    uint16_t doorDistance=doorSensor.read(false);

    //range status=0 for valid....everything else an error state
    uint8_t rangeStatus=doorSensor.ranging_data.range_status;
    uint16_t Sunshine=doorSensor.ranging_data.ambient_rate_kcps; //check the sunlight effect

    
    
    if(doorDistance==0) doorDistance=9999;

    Serial.printf("Door Distance: %d   %s  || range_status: %i ||Sunlight: %i\n",doorDistance,(doorDistance>DOOR_OPEN_THRESHOLD)?"OPEN":"CLOSED",rangeStatus,Sunshine);
    Serial.println("------------------------------------");

   // digitalWrite(indicatorLED,HIGH);
    //status 2 = signal too low. In direct sun the reading is useless, so keep the last trusted state
    if(rangeStatus==2 && Sunshine>SUN_AMBIENT_THRESHOLD)
    {
        Serial.printf("Sunlight swamped door sensor -- holding last state: %s\n",lastDoorState?"OPEN":"CLOSED");
        return(lastDoorState);
    }

    bool doorStatus=(doorDistance>DOOR_OPEN_THRESHOLD); //is the door open (true)
    lastDoorState=doorStatus;
    return(doorStatus);
    
    
    
    
   




}

