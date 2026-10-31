#include <Arduino.h>
#include "StateController.h"
#include "speech-phone-charging.h"
#include "Chime.h"
#include "startup.h"
#include "BritishPhonePrompt.h"






//--------------------------------------------------------------------------
//  Bring in a reference to the I2S music object initialized in main

StateController::StateController(I2SClass& i2S, Sensor& snsrs):player(i2S), sensors(snsrs)
{
    lastScan=millis();  // initialize scan time


}


//--------------------------------------------------------------------------

StateController::~StateController()
{

}
//--------------------------------------------------------------------------

//                  ​‌‌‌‍⁡⁣⁢⁣𝕊𝕋𝔸𝕋𝔼 ℂ𝕆ℕ𝕋ℝ𝕆𝕃𝕃𝔼ℝ⁡​


void StateController::manageStates()
{

    

    if(millis()-lastScan > scanInterval)    //gate the iterations
    {
        traystate=sensors.readPhonedist();
        doorstate=sensors.readDoordist();
        lastScan=millis();


        switch (Program_State)
        {
        //::::::::::::::::::::::::::::   

        case STATES::initializing:
        {
            if(!startupFlag)
            {
                startupFlag=true;
                player.write(startup_raw, sizeof(startup_raw));
                
            
            }

            Program_State=STATES::monitoring;

            break;
        }
        
        //::::::::::::::::::::::::::::   
        case   STATES::monitoring:
        {
            //Serial.printf("Door is %s.     Phone %s.\n", (doorstate)?"Open":"Closed",(traystate)?"In Tray":"Not in Tray");
            
            if(traystate)
            {
                if (!phonePlaced)
                {
                    digitalWrite(indicatorLED,HIGH);    //turn on blue LED
                    player.write(Chime_raw, sizeof(Chime_raw)); //play the phone is in the tray sound
                    
                    phonePlaced=true;
                }
            }
            else
            {
                phonePlaced=false;  //phone is no longer in tray reset for chime sound
                warningMSGflag=false; //re-arm for next warning
                digitalWrite(indicatorLED,LOW);
            }


            if(traystate && doorstate)
            {

                if(!warningMSGflag)
                {
                    warningMSGflag=true;    //so it only plays once
                    player.write(BritishPhonePrompt_raw,sizeof(BritishPhonePrompt_raw));

                }
            }


        }
        
        //::::::::::::::::::::::::::::   
       
        
        
        
        
        
        default:
            break;


        

        }



        //::::::::::::::::::::::::::::   







    }


    

/*

*/

    



}