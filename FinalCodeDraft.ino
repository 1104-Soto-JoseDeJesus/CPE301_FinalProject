//Use of previous lab code is involved in sections

//Disclaimers:
//GPIO is used for the reset button, Green, Red, and Blue LEDS, due to time constrains and circuit complications Yellow LED and start stop button use library functions no ISR
//Instead of a button or potentiometer, a switch is used for the stepper motor control
//

//Temp and Humidity Sensor: Kyla Trotter
//Start Stop button / Yellow LED: Evita Kanaan
//RTC: Kyla Trotter
//Fan: Evita Kanaan
//Overview: Evita Kanaan, Kyla Trotter & Jose Soto
//Water Sensor: Jose Soto
//Error to Idle Reset buttton: Jose Soto
//Stepper Motor: Jose Soto
//LCD Screen w/potentiometer: Jose Soto
//Blue, Green, Red LED integration: Jose Soto
//Code Integrating: Jose Soto

//Temp and humidity sensor Libraries
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
//

//Includes the Arduino Stepper Library
#include <Stepper.h>
//

//LCD Librar7
#include <LiquidCrystal.h>
// 

//RTC library
#include <RTClib.h>
RTC_DS1307 rtc;

//Defining temp sensor
#define DHTTYPE    DHT11     // DHT 11
#define DHTPIN 7
DHT_Unified dht(DHTPIN, DHTTYPE);
// 

//Defining functions
#define RDA 0x80
#define TBE 0x20  
//

//Steps per sterpper motor rotation
const int stepsPerRevolution = 2000;
//

//Stepper motor pins
Stepper myStepper = Stepper(stepsPerRevolution, 13, 10, 9, 8);
//

//Motor control value
int switchvalue = 0;
//

//PK2 registers
volatile unsigned char* pin_k  = (unsigned char*) 0x106;
volatile unsigned char* ddr_k  = (unsigned char*) 0x0107;
volatile unsigned char* port_k = (unsigned char*) 0x108;
//

//PE3 registers
volatile unsigned char* port_e = (unsigned char*) 0x2E;
volatile unsigned char* ddr_e  = (unsigned char*) 0x2D;
//

//reset button
int reset = 0;
int buttonState = 0;
int saveval = 0;
//

//Display temp timer
unsigned long previousMillis = 0;
const long interval = 60000;
//

//Timer Pointers
volatile unsigned char *myTCCR1A = (unsigned char *) 0x80;
volatile unsigned char *myTCCR1B = (unsigned char *) 0x81;
volatile unsigned char *myTCCR1C = (unsigned char *) 0x82;
volatile unsigned char *myTIMSK1 = (unsigned char *) 0x6F;
volatile unsigned int  *myTCNT1  = (unsigned  int *) 0x84;
volatile unsigned char *myTIFR1 =  (unsigned char *) 0x36;
//

//PDO - Red LED Registers
volatile unsigned char* port_d = (unsigned char*) 0x2B;
volatile unsigned char* ddr_d  = (unsigned char*) 0x2A;
//

//LCD pins
const int RS = 11, EN = 12, D4 = 2, D5 = 3, D6 = 4, D7 = 6;

LiquidCrystal lcd(RS, EN, D4, D5, D6, D7);
//

//FanPinout
int enA = 45;
int in1 = 43;
int in2 = 41;
//

//distance/wateramount variable
float distance_cm;
//

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

//Setting up the program
void setup() {

  //Setting pins 30-39 as inputs for stepper motor
  for (int pin = 30; pin <= 39; pin++) {
    pinMode(pin, INPUT);
  }
  //

  //Fan Pin Modes
  pinMode(enA, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  //

  // setup the UART
  U0init(9600);
  //

  // setup the ADC
  adc_init();
  //

  //Blue LED as output
  DDRD |= (1 << PD1);
  //

  //PD0 set to Output
  *ddr_d |= 0x01;
  //

  //PK2 set to Input
  *ddr_k &= 0xFD;
  //

  //PK2 Internal Pullup enables
  *port_k |= 0x04;
  //

  //PE3 set to Output
  *ddr_e |= 0x08;
  //

  //Addition
  pinMode(24, OUTPUT);
  pinMode(22, INPUT);
  //

  //Initializing LCD
  lcd.begin(16, 2);
  //

  //Temp sensor initializing
  dht.begin();
  //

  setupRTC();
}
//

//Loop begins
void loop() {

  //Reading all digitial inputs from switch to control stepper motor
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
  //

  //Cascading if statement for switch value to determine steps motor should take
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
  //

  //If change in switch rotate the stepper motor
  if(switchvalue != savevalue){
    myStepper.setSpeed(10);

	  myStepper.step(switchvalue);
  }
  //

  //If no change in switch do not rotate stepper motor
  else if(switchvalue == savevalue){
    myStepper.setSpeed(10);

	  myStepper.step(0);
  }
  //

if(saveval == 0){
    digitalWrite(24, HIGH);
    //Turn off Blue LED
    PORTD &= ~(1 << PD1);
    //
    //Turning off RED Light
    *port_d &= 0x2A;
    //
    *port_e &= 0x26;
    //Fan OFF
    digitalWrite(in1, LOW);
    digitalWrite(in2, LOW);
    //
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("SYSTEM DISABLED");
    my_delay(500);
  buttonState = digitalRead(22);
  if(buttonState == HIGH){
    saveval = 1;
    lcd.clear();
    my_delay(250);
  }

}

if(saveval == 1){
  buttonState = digitalRead(22);
  if(buttonState == HIGH){
    saveval = 0;
    my_delay(250);
  }
  my_delay(500);
  digitalWrite(24, LOW);

  //read voltage from ADC
  unsigned int Vreading = adc_read(0); 
  //

  //send voltage over serial
  send_WL(Vreading);
  //

  sensors_event_t event;
  dht.temperature().getEvent(&event);
  double temp = event.temperature;

  //Reset functionality
  if(reset == 0 && distance_cm >= 1.00 && temp <= 24){

  //Temp sensor timer
  unsigned long currentMillis = millis();
  if(currentMillis - previousMillis >= interval){
    previousMillis = currentMillis;
  //

    //Displaying temperature to lcd
    lcd.setCursor(0, 0);
    lcd.print(F("Temp: "));
    lcd.print(event.temperature);
    lcd.print(F("C"));
    dht.humidity().getEvent(&event);
    lcd.setCursor(0, 1);
    lcd.print(F("Humidity: "));
    lcd.print(event.relative_humidity);
    lcd.print(F("%"));
    //
  }

    //Fan OFF
    digitalWrite(in1, LOW);
    digitalWrite(in2, LOW);
    //
    printTime();

    //Turning on Green light
    *port_e |= 0x08;
    //
    //Turn off Blue LED
    PORTD &= ~(1 << PD1);
    //
    //Turning off RED Light
    *port_d &= 0x2A;
    //

  }


  else if(reset == 0 && distance_cm >= 1.00 && temp >= 24){
    printTime();
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
    analogWrite(enA, 255); // Set the fan to maximum speed
    my_delay(25);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("FAN RUNNING");
    PORTD |= (1 << PD1);
    *port_d &= 0x2A;
    *port_e &= 0x26;
  }

//LCD Error Message
  if(distance_cm <= 1.00){
    printTime();

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Water Level is");
    lcd.setCursor(0, 1);
    lcd.print("too Low");
    *port_e &= 0x26;
    *port_d |= 0x01;
    PORTD &= ~(1 << PD1);
    reset = 1;

    digitalWrite(in1, LOW);
    digitalWrite(in2, LOW);
  }

    if(distance_cm >= 1.00){
      if(*pin_k &= 0x04){
        reset = 0;
       *port_d &= 0x2A;
        lcd.clear();
        printTime();
      }
  }
}

  //delay
  my_delay(1);
}
//

//ADC Initiliazing 
void adc_init()
{
  // setup the A register
  *my_ADCSRA |= 0b10000000; // set bit   7 to 1 to enable the ADC
  *my_ADCSRA &= 0b11011111; // clear bit 6 to 0 to disable the ADC trigger mode
  *my_ADCSRA &= 0b11110111; // clear bit 5 to 0 to disable the ADC interrupt
  *my_ADCSRA &= 0b11111000; // clear bit 0-2 to 0 to set prescaler selection to slow reading
  // setup the B register
  *my_ADCSRB &= 0b11110111; // clear bit 3 to 0 to reset the channel and gain bits
  *my_ADCSRB &= 0b11111000; // clear bit 2-0 to 0 to set free running mode
  // setup the MUX Register
  *my_ADMUX  &= 0b01111111; // clear bit 7 to 0 for AVCC analog reference
  *my_ADMUX  |= 0b01000000; // set bit   6 to 1 for AVCC analog reference
  *my_ADMUX  &= 0b11011111; // clear bit 5 to 0 for right adjust result
  *my_ADMUX  &= 0b11100000; // clear bit 4-0 to 0 to reset the channel and gain bits
}
//

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

//Display Functions
void U0init(int U0baud)
{
 unsigned long FCPU = 16000000;
 unsigned int tbaud;
 tbaud = (FCPU / 16 / U0baud - 1);
 // Same as (FCPU / (16 * U0baud)) - 1;
 *myUCSR0A = 0x20;
 *myUCSR0B = 0x18;
 *myUCSR0C = 0x06;
 *myUBRR0  = tbaud;
}
unsigned char U0kbhit()
{
  return *myUCSR0A & RDA;
}
unsigned char U0getchar()
{
  return *myUDR0;
}
void U0putchar(unsigned char U0pdata)
{
  while((*myUCSR0A & TBE)==0);
  *myUDR0 = U0pdata;
}
//

//Converting reading to water level in centimeters
void send_WL(unsigned int Vreading) {
  // Convert ADC reading to voltage (assuming 5V reference and 10-bit ADC)
  float voltage = Vreading * (5.0 / 1023.0);

  // Convert voltage to centimeters (assuming a linear sensor as described earlier)
  distance_cm = (voltage / 5.0) * 10;

  // Extract the integer part of the distance
  unsigned int integer_part = (unsigned int)distance_cm;

  // Send the integer part
  //U0putchar((unsigned char)(integer_part / 10) + '0');
  //U0putchar((unsigned char)(integer_part % 10) + '0');

  // Send decimal point
  //U0putchar('.');

  // Calculate and send fractional part of distance
  unsigned int fractional_part = (distance_cm - integer_part) * 100;
  //U0putchar((unsigned char)(fractional_part / 10) + '0');
  //U0putchar((unsigned char)(fractional_part % 10) + '0');

 // U0putchar('c');
  //U0putchar('m');
  // Send newline character
  //U0putchar('\n');
}
//

//Custom Delay
void my_delay(unsigned int freq)
{
  // calc period
  double period = 1.0/double(freq);
  // 50% duty cycle
  double half_period = period/ 2.0f;
  // clock period def
  double clk_period = 0.0000000625;
  // calc ticks
  unsigned int ticks = half_period / clk_period;
  // stop the timer
  *myTCCR1B &= 0xF8;
  // set the counts
  *myTCNT1 = (unsigned int) (65536 - ticks);
  // start the timer
  * myTCCR1A = 0x0;
  * myTCCR1B |= 0b00000001;
  // wait for overflow
  while((*myTIFR1 & 0x01)==0); // 0b 0000 0000
  // stop the timer
  *myTCCR1B &= 0xF8;   // 0b 0000 0000
  // reset TOV           
  *myTIFR1 |= 0x01;
}//




//RTC Functions

void setupRTC() {

  if (!rtc.begin()) {
    U0putchar('N');
    U0putchar('o');
    U0putchar(' ');
    U0putchar('R');
    U0putchar('T');
    U0putchar('C');
    U0putchar('\n');
    while (1) my_delay(10);
  }

  if (!rtc.isrunning()) {
    U0putchar('R');
    U0putchar('T');
    U0putchar('C');
    U0putchar(' ');
    U0putchar('N');
    U0putchar('O');
    U0putchar('T');
    U0putchar(' ');
    U0putchar('r');
    U0putchar('u');
    U0putchar('n');
    U0putchar('n');
    U0putchar('i');
    U0putchar('n');
    U0putchar('g');
    U0putchar('\n');
    rtc.adjust(DateTime(2024, 5, 8, 10, 15, 0)); // Adjust as needed
  }
}

void printTime() {
    DateTime now = rtc.now();

    // Extract time components
    int hour = now.hour();
    int minute = now.minute();
    int second = now.second();

    // Output hour (tens digit)
    U0putchar('0' + hour / 10);

    // Output hour (ones digit)
    U0putchar('0' + hour % 10);

    // Output colon separator
    U0putchar(':');

    // Output minute (tens digit)
    U0putchar('0' + minute / 10);

    // Output minute (ones digit)
    U0putchar('0' + minute % 10);

    // Output colon separator
    U0putchar(':');

    // Output second (tens digit)
    U0putchar('0' + second / 10);

    // Output second (ones digit)
    U0putchar('0' + second % 10);

    // Output newline character (optional)
    U0putchar('\n');
}
