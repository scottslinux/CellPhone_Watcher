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
    
    
    if(doorDistance==0) doorDistance=9999;

    Serial.printf("Door Distance: %d   %s\n",doorDistance,(doorDistance>DOOR_OPEN_THRESHOLD)?"OPEN":"CLOSED");
    Serial.println("------------------------------------");

   // digitalWrite(indicatorLED,HIGH);
    bool doorStatus=(doorDistance>DOOR_OPEN_THRESHOLD); //is the door open (true)

    if(doorStatus)  
        return(true);

        else
            return (false);
    
    
    
    
    //if the door was open in one reading take 5 more to confirm
   /*     {
            int certaintyIndex=0;
            for(int i=0;i<3;i++)
                {   unsigned long t0=millis();
                    while(!doorSensor.dataReady());  //don't hang if sensor stops responding
                    if(doorSensor.read(false)>DOOR_OPEN_THRESHOLD)
                        certaintyIndex++;
                    delay(10);  //wait 50ms between readings
                }
            return((certaintyIndex>=1)? true:false);




        }
*/




}

