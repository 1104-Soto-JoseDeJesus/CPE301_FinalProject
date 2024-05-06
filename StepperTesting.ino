//Jose De Jesus Soto

//Includes the Arduino Stepper Library
#include <Stepper.h>

// Defines the number of steps per rotation
const int stepsPerRevolution = 2000;

int mappedValue;

// Creates an instance of stepper class
// Pins entered in sequence IN1-IN3-IN2-IN4 for proper step sequence
Stepper myStepper = Stepper(stepsPerRevolution, 8, 10, 9, 11);


#define POTENTIOMETER_PIN A1

void setup() {
    Serial.begin(9600);
}

void loop() {

    // Read the analog value from the potentiometer
    int potValue = analogRead(POTENTIOMETER_PIN);

    // Map the value to the desired range (0 to 2000)
    mappedValue = map(potValue, 0, 1023, -2000, 2000);

    // Print the mapped value to the Serial Monitor
    Serial.println(mappedValue);

    delay(1000); // Add a small delay for stability

    myStepper.setSpeed(10);

	  myStepper.step(mappedValue);
}


