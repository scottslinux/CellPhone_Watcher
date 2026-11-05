#include <Arduino.h>
#include <ESP_I2S.h>

#include <Adafruit_VL6180X.h>
#include <VL53L4CD.h>
#include <Wire.h>
#include "Sensor.h"
#include "StateController.h"


#define xshutPhone D3
#define xshutDoor D1
#define indicatorLED D0

I2SClass I2S;
Adafruit_VL6180X phoneSensor = Adafruit_VL6180X();
VL53L4CD doorSensor;


Sensor sensors(phoneSensor, doorSensor);   //⁡⁣⁣⁢initialize sensors object
StateController controller(I2S,sensors);




void setup() {

  Serial.begin(115200);
  Serial.setTxTimeoutMs(0);   // don't block on prints when USB host isn't reading

  Wire.begin(D4,D5); //intialize SDA,SCL
  Wire.setClock(400000); // use 400 kHz I2C
  doorSensor.setTimeout(500);

  delay(3000);


  pinMode(xshutPhone,OUTPUT);  //holding xshut pin LOW disables
  pinMode(xshutDoor,OUTPUT);  //
  pinMode(indicatorLED,OUTPUT);
  digitalWrite(indicatorLED,LOW);

  //  turn both sensors off to reset address changes
  digitalWrite(xshutPhone,LOW);
  digitalWrite(xshutDoor,LOW);
  delay(50);

  //      Now turn on just the door sensor and change the address.
  //      Without the brief xshut, changes from prior runs will persist.
  //      Keep the phonesensor off to allow the address change.
  digitalWrite(xshutDoor,HIGH);

  if (!doorSensor.init())
  {
    Serial.println("Failed to detect and initialize door sensor!");
    while (1);
  }
  else
  {
    doorSensor.setAddress(0x30);    //the secret sauce...change the address
    Serial.printf("Door Sensor Initialized...new address: %0x \n",doorSensor.getAddress());

    doorSensor.setTimeout(500);
    doorSensor.setRangeTiming(200, 0);
    doorSensor.startContinuous(); 
  }

  
  digitalWrite(xshutPhone,HIGH);  //turn the phoneSensor back on 


  
  

  I2S.setPins(D6, D7, D10); // BCLK, WS, DIN
  I2S.begin(I2S_MODE_STD, 22050,
              I2S_DATA_BIT_WIDTH_16BIT,
              I2S_SLOT_MODE_MONO);

  Serial.println("Adafruit phoneSensor6180x test!");
  if (! phoneSensor.begin()) {
    Serial.println("Failed to find sensor");
    
  }
  Serial.printf("Phone Sensor found at address: %0x",phoneSensor.getAddress());





  
 
 
 // I2S.write(Chime_raw, sizeof(Chime_raw));    //play the sound

 // delay(3000);
 // I2S.write(BritishPhonePrompt_raw,sizeof(BritishPhonePrompt_raw)); //play reminder
 
  Serial.println("Welcome to the Phone Minder!");



 
}

void loop() {

  controller.manageStates();
  
  
 
}

