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
    //sensor measures every 200ms but we scan every 10ms...only act on fresh measurements
    if(!doorSensor.dataReady())
        return(lastDoorState);

    uint16_t doorDistance=doorSensor.read(false);

    //range status=0 for valid....everything else an error state
    uint8_t rangeStatus=doorSensor.ranging_data.range_status;

    //library stores ambient*8 in a uint16_t which wraps above 65535 in strong sun.
    //read the raw ambient register and scale it ourselves
    uint32_t Sunshine=(uint32_t)doorSensor.readReg16Bit(VL53L4CD::RESULT__AMBIENT_RATE)*8;

    bool sunny=(Sunshine>SUN_AMBIENT_THRESHOLD);

    Serial.printf("Door Distance: %d || range_status: %i || Sunlight: %lu %s\n",
                  doorDistance,rangeStatus,(unsigned long)Sunshine,sunny?"(SUNNY)":"");

    if(rangeStatus!=0 && sunny)
    {
        //error reading in sunlight tells us nothing...keep what we had
        Serial.printf("  untrusted reading -- holding %s\n",lastDoorState?"OPEN":"CLOSED");
        Serial.println("------------------------------------");
        return(lastDoorState);
    }

    //normal light (or a valid reading in sun): distance decides, same as the original logic
    if(doorDistance==0) doorDistance=9999;
    bool rawOpen=(doorDistance>DOOR_OPEN_THRESHOLD);

    if(rawOpen)
    {
        if(openCount<DOOR_OPEN_CONFIRM) openCount++;
        if(openCount>=DOOR_OPEN_CONFIRM) lastDoorState=true;   //confirmed open
    }
    else
    {
        openCount=0;            //any trusted CLOSED reading resets the count
        lastDoorState=false;
    }

    Serial.printf("  reading: %s  (%d/%d)  => door %s\n",rawOpen?"OPEN":"CLOSED",openCount,DOOR_OPEN_CONFIRM,
                  lastDoorState?"OPEN":"CLOSED");
    Serial.println("------------------------------------");

    return(lastDoorState);
}
