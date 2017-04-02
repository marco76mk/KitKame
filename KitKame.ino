#define BLYNK_PRINT Serial    // Comment this out to disable prints and save space
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

#include <Servo.h>
#include "minikame.h"
#include "sensoreyes.h"
MiniKame robot;
SensorEyes eyes;

char auth[] = "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx";
volatile int pin = -1;
volatile int buttonDown = 0;
const int AnalogIn  = A0;
int readingVIn = 0;
float leveBattery = 0;

void setup()
{
  Serial.begin(9600);
  Blynk.begin(auth, "yourWiFi", "passWiFi");
  robot.init();
  eyes.init();
}

void MoveItMoveIt() {
  if(buttonDown == 1) {
    switch (pin){
        case 1:
            Serial.println("misuro la distanza ese è inferiore a 20 procedo");
            eyes.measureDistance();
            Serial.println(eyes.getDistance());
            if (eyes.getDistance() == 0 ||  eyes.getDistance() > 20) {
                // just run, and for the moment, run into a wall if we get no values
                robot.walk(1,550);
                Serial.println("walk");
            } else {
                // something is in the way, try to go right
                robot.upDown(4,250);
                Serial.println("ops ... non posso andare avanti!! Misura:");
                Serial.println(eyes.getDistance());
                Serial.println("updown");
            }
            break;
        case 2:
            Serial.println("nothing");
            robot.home();
            break;
        case 3:
            robot.turnL(1,550);
            Serial.println("left");
            break;
        case 4:
            robot.turnR(1,550);
            Serial.println("right");
            break;
        case 5:
            //STOP
            Serial.println("stop");
            break;
        case 6: //heart
            robot.pushUp(2,5000);
            Serial.println("pushup");
            break;
        case 7: //fire
            robot.upDown(4,250);
            Serial.println("updown");
            break;
        case 8: //skull
            robot.jump();
            Serial.println("jump");
            break;
        case 9: //cross
            robot.hello();
            Serial.println("hello");
            break;
        case 10: //punch
            //robot.moonwalkL(4,2000);
            robot.frontBack(4,200);
            Serial.println("frontback");
            break;
        case 11: //mask
            robot.dance(2,1000);
            Serial.println("dance");
            break;
      }    
    } else {
      robot.home();
      //Serial.println("home");
          
    }

}

BLYNK_WRITE_DEFAULT() {
  pin = request.pin; 
  buttonDown = param.asInt();
}


BLYNK_READ(V15) {
  eyes.measureDistance();
  Blynk.virtualWrite(V15, eyes.getDistance());
  Serial.print("distanza: ");
  Serial.println(eyes.getDistance());
}

BLYNK_READ(V16) {
  readingVIn = analogRead(AnalogIn);
  //float leveBattery = fmap(readingVIn, 0, 1023, 0.0, 12.6);
  leveBattery = (float)readingVIn*12.6/1023;
  Blynk.virtualWrite(V16, leveBattery);
  Serial.print("Tensione: ");
  Serial.println(leveBattery);
}



void loop()
{
  Blynk.run();
  MoveItMoveIt();
}

