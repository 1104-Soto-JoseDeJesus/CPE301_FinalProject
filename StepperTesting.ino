//Jose De Jesus Soto

//Includes the Arduino Stepper Library
#include <Stepper.h>

// Defines the number of steps per rotation
const int stepsPerRevolution = 2000;

// Creates an instance of stepper class
// Pins entered in sequence IN1-IN3-IN2-IN4 for proper step sequence
Stepper myStepper = Stepper(stepsPerRevolution, 13, 10, 9, 8);

int switchvalue = 0;

void setup() {
    Serial.begin(9600);

    for (int pin = 30; pin <= 39; pin++) {
    pinMode(pin, INPUT);
  }
}

void loop() {
    int savevalue = switchvalue;
    int pin30 = digitalRead(30);
    int pin31 = digitalRead(31);
    int pin32 = digitalRead(32);
    int pin33 = digitalRead(33);
    int pin34 = digitalRead(34);
    int pin35 = digitalRead(35);
    int pin36 = digitalRead(36);
    int pin37 = digitalRead(37);
    int pin38 = digitalRead(38);
    int pin39 = digitalRead(39);
    if(pin30 ==  HIGH){
      switchvalue = 2000;
    }
    else if(pin31 ==  HIGH){
      switchvalue = 1600;
    }
    else if(pin32 ==  HIGH){
      switchvalue = 1200;
    }
    else if(pin33 ==  HIGH){
      switchvalue = 800;
    }
    else if(pin34 ==  HIGH){
      switchvalue = 400;
    }
    else if(pin35 ==  HIGH){
      switchvalue = -400;
    }
    else if(pin36 ==  HIGH){
      switchvalue = -800;
    }
    else if(pin37 ==  HIGH){
      switchvalue = -1200;
    }
    else if(pin38 ==  HIGH){
      switchvalue = -1600;
    }
    else if(pin39 ==  HIGH){
      switchvalue = -2000;
    }
    else{ 
      switchvalue = 0;
    }
    if(switchvalue != savevalue){
    myStepper.setSpeed(10);

	  myStepper.step(switchvalue);
    }
    else if(switchvalue == savevalue){
    myStepper.setSpeed(10);

	  myStepper.step(0);
    }
}


