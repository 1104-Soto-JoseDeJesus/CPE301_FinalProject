//code by: Evita Kanaan
#include <Stepper.h>
#include <LiquidCrystal.h>

// Define constants for the stepper motor and fan
#define STEPS_PER_REVOLUTION 200
#define FAN_PIN 9

// Create an instance of the stepper class
Stepper myStepper(STEPS_PER_REVOLUTION, 8, 9, 10, 11);

// Define variables for temperature range
int tempMin = 20; // minimum temperature to start the fan
int tempMax = 30; // maximum temperature to stop the fan

// Define variable for fan state
bool isFanOn = false;

void setup() {
  // Set the speed of the motor
  myStepper.setSpeed(60);
  
  // Set the fan pin as output
  pinMode(FAN_PIN, OUTPUT);
  
  // TODO: Initialize temperature sensor,etc
  // TODO: Initialize LCD screen
  // TODO: Initialize UART and ADC
}

void loop() {
  // Get the current temperature
  int currentTemp = getTemperature();

  // Check if the temperature is out of the specified range
  if (currentTemp < tempMin || currentTemp > tempMax) {
    // If the fan is off, turn it on
    if (!isFanOn) {
      digitalWrite(FAN_PIN, HIGH);
      isFanOn = true;
      // Record the time and date the fan was turned on
      recordTimeDate();
    }
  } else {
    // If the fan is on, turn it off
    if (isFanOn) {
      digitalWrite(FAN_PIN, LOW);
      isFanOn = false;
      // Record the time and date the fan was turned off
      recordTimeDate();
    }
  }

  // Check if the user has adjusted the vent position
  if (isVentPositionChanged()) {
    // Move the stepper motor to adjust the vent position
    myStepper.step(getStepsToMove());
  }
  
  // TODO: Implement the DISABLED state and control of the BLUE LED as per your project's specifications
  // TODO: Implement the LCD error message when the water level is too low
  // TODO: Implement the delay function
}

// Function to get the current temperature
int getTemperature() {
  // TODO: Replace this with actual code to read the temperature sensor
  return 25; // placeholder
}

// Function to record the time and date the fan was turned on or off
void recordTimeDate() {
  // TODO: Replace this with actual code to record the time and date
}

// Function to check if the vent position has been changed
bool isVentPositionChanged() {
  // TODO: Replace this with actual code to read the input from the buttons or potentiometer
  return false; // placeholder
}

// Function to get the number of steps to move the stepper motor
int getStepsToMove() {
  // TODO: Replace this with actual code to calculate the number of steps to move
  // based on the input from the buttons or potentiometer
  return 0; // placeholder
}
