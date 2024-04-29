//code by: Evita Kanaan
#include <Stepper.h>
#include <LiquidCrystal.h>

//joses ones v
//ADC and Serial direct registers
volatile unsigned char *myUCSR0A = (unsigned char *)0x00C0;
volatile unsigned char *myUCSR0B = (unsigned char *)0x00C1;
volatile unsigned char *myUCSR0C = (unsigned char *)0x00C2;
volatile unsigned int  *myUBRR0  = (unsigned int *) 0x00C4;
volatile unsigned char *myUDR0   = (unsigned char *)0x00C6;
 
volatile unsigned char* my_ADMUX = (unsigned char*) 0x7C;
volatile unsigned char* my_ADCSRB = (unsigned char*) 0x7B;
volatile unsigned char* my_ADCSRA = (unsigned char*) 0x7A;
volatile unsigned int* my_ADC_DATA = (unsigned int*) 0x78;
//

 
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

//idk if i can use this v
// Define variable for system state
enum SystemState { IDLE, ACTIVE, ERROR };
SystemState systemState = IDLE;

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
    // Set the system state to ACTIVE
    systemState = ACTIVE;
  } else {
    // If the fan is on, turn it off
    if (isFanOn) {
      digitalWrite(FAN_PIN, LOW);
      isFanOn = false;
      // Record the time and date the fan was turned off
      recordTimeDate();
    }
    // Set the system state to IDLE
    systemState = IDLE;
  }

  // Check if the user has adjusted the vent position
  if (isVentPositionChanged()) {
    // Move the stepper motor to adjust the vent position
    myStepper.step(getStepsToMove());
  }
  
  // TODO: Implement the DISABLED state and control of the BLUE LED 
  // TODO: Implement the LCD error message when the water level is too low
  // TODO: Implement the delay function
}

// Function to convert ADC reading to temperature
// assuming that the ADC is 10-bit (so maximum reading is 1023) and the reference voltage is 5V. replace these values
int convertReadingToTemperature(unsigned int reading) {
  // Convert the ADC reading to voltage (random values)
  float voltage = reading * (5.0 / 1023.0);

  // Convert the voltage to temperature based on sensor's specifications
  // This is a placeholder 
  int temperature = voltage * 100; // Replace this with the actual conversion formula
  // temperature = m * voltage + b, where m is the slope and b is the y-intercept

  return temperature;
}


// Function to get the current temperature
int getTemperature() {
  // Read the temperature sensor using the adc_read function
  unsigned int tempReading = adc_read(0); // Replace 0 with the actual ADC channel connected to temperature sensor

  // Convert the ADC reading to temperature based on your sensor's specifications
  int temperature = convertReadingToTemperature(tempReading);

  return temperature;
}

// Function to record the time and date the fan was turned on or off
void recordTimeDate() {
  // Get the current time and date
  // how system keeps track of time
  // read the time from the RTC

  // Send the time and date over serial
  // use the U0putchar function to send data over serial?
  // convert the time and date to a string first?
}

//temporarily put joses code for it to run
//Reading the analog signal directly function
unsigned int adc_read(unsigned char adc_channel_num)
{
  // clear the channel selection bits (MUX 4:0)
  *my_ADMUX  &= 0b11100000;
  // clear the channel selection bits (MUX 5)
  *my_ADCSRB &= 0b11110111;
  // set the channel number
  if(adc_channel_num > 7)
  {
    // set the channel selection bits, but remove the most significant bit (bit 3)
    adc_channel_num -= 8;
    // set MUX bit 5
    *my_ADCSRB |= 0b00001000;
  }
  // set the channel selection bits
  *my_ADMUX  += adc_channel_num;
  // set bit 6 of ADCSRA to 1 to start a conversion
  *my_ADCSRA |= 0x40;
  // wait for the conversion to complete
  while((*my_ADCSRA & 0x40) != 0);
  // return the result in the ADC data register
  return *my_ADC_DATA;
}
//


// Function to check if the vent position has been changed
bool isVentPositionChanged() {
  // Read the input from the potentiometer using the adc_read function
  unsigned int positionReading = adc_read(1); // Replace 1 with the actual ADC channel connected to potentiometer

  // Compare the current reading with the previous reading
  // If the difference is greater than a certain threshold, return true
  // Otherwise, return false

  // keep track of the previous reading and update it each time this function is called
}

// Function to get the number of steps to move the stepper motor
int getStepsToMove() {
  // Determine the number of steps to move based on the input from the potentiometer
}
