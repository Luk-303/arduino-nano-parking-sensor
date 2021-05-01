/*
Author: Luk-303

a small and simple project to practice some basics with an Arduino Nano,
an ultrasonic sensor and an infrared movement detector.

August 2020
*/

#include <Arduino.h>
#include <avr/sleep.h>


const unsigned int PIN_LED_RED=12;
const unsigned int PIN_LED_YELLOW=11;
const unsigned int PIN_LED_GREEN=10;
const unsigned int PIN_LED_BLUE=9;

const unsigned int PIN_TRIGGER=8;                                                         //Pin connected with sensor for sending an ultrasonic pulse
const unsigned int PIN_ECHO=7;                                                            //Pin for reciving the ultrasonic pulse

const unsigned int PIN_MOVEMENT=2;                                                        //Pin connected with movement sensor

unsigned long nTimestamp=0;                                                                //timestamp for millis() function
unsigned long nRuntime=60000;                                                              //variable for how long the whole process is running

bool bState=false;                                                                         // state for InerruptServieRoutine

int measureUp(){                                                                          //function to messure the distance and calculate it to centimeters
  int nDuration=0;
  int nDistance=0;

  digitalWrite(PIN_TRIGGER,LOW);          
  delay(5);
  digitalWrite(PIN_TRIGGER,HIGH);
  delay(10);
  digitalWrite(PIN_TRIGGER,LOW);

  nDuration=pulseIn(PIN_ECHO,HIGH);        
  nDistance=(nDuration/2) * 0.03432;

  return nDistance;
}

void ledBlink(){
 if ((measureUp()<4||measureUp()>=400)){                                                   //blue LED will flash if the object is not in the sensors detection range
    digitalWrite(PIN_LED_BLUE,HIGH);
    delay(200);
    digitalWrite(PIN_LED_BLUE,LOW);
    delay(200);  
   }
  else if((measureUp()<400&&measureUp()>50)){                                             //green LED will flash if the objekt is between 400 cm and 100 cm away
    digitalWrite(PIN_LED_GREEN,HIGH);
    delay(200);
    digitalWrite(PIN_LED_GREEN,LOW);
    delay(200);  
  }
  else if((measureUp()<=50&&measureUp()>=10)){                                              //yellow LED will flash if the object is between 100cm and 30cm away
    digitalWrite(PIN_LED_YELLOW,HIGH);
    delay(200);
    digitalWrite(PIN_LED_YELLOW,LOW);
    delay(200);  
  }
  else if((measureUp()<10&&measureUp()>=4)){                                               //red LED will flash if the objekt is between 30 cm and 4 cm away
    digitalWrite(PIN_LED_RED,HIGH);
    delay(200);
    digitalWrite(PIN_LED_RED,LOW);
    delay(200);  
  }
}

void pinMovementISR(){                                                                    //InterruptServiceRoutine to wake up the Arduino Nano when a movement is detected
  bState=true;
}

void enterSleep(){                                                                        //function to set sleep mode to highest power saving setting
  attachInterrupt(digitalPinToInterrupt(PIN_MOVEMENT), pinMovementISR, RISING);           //attach Interrupt to an rising edge
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);                                            
  sleep_enable();
  sleep_mode();
}

void setup() {
  pinMode(PIN_LED_BLUE,OUTPUT);
  pinMode(PIN_LED_RED,OUTPUT);
  pinMode(PIN_LED_YELLOW,OUTPUT);
  pinMode(PIN_LED_GREEN,OUTPUT);

  pinMode(PIN_TRIGGER,OUTPUT);
  pinMode(PIN_ECHO,INPUT);

  pinMode(PIN_MOVEMENT,INPUT);
  
  nTimestamp=millis();

  Serial.begin(115200);
}

void loop() {
   
  enterSleep();                                                                         //enter sleep mode and wait for an interrupt
  
  if (bState){
    sleep_disable();                                                                    //stop sleep mode
    while(millis()-nTimestamp<=nRuntime){                                               //run programm for duration set in runTime
      ledBlink();
      }
    nTimestamp=millis();
    bState=false;                                                                       //stop loop and enter sleep mode again
  } 

}