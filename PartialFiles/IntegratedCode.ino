//Code By Jose Soto 
//Use of previous lab code is involved in sections



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
//

//RTC library
#include <RTClib.h>
RTC_DS1307 rtc;


//Temp and humidity sensor
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
#define DHTTYPE    DHT11     // DHT 11
#define DHTPIN 7
DHT_Unified dht(DHTPIN, DHTTYPE);

unsigned long previousMillis = 0;
const long interval = 6000;
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


//LCD
#include <LiquidCrystal.h>

const int RS = 11, EN = 12, D4 = 2, D5 = 3, D6 = 4, D7 = 6;

LiquidCrystal lcd(RS, EN, D4, D5, D6, D7);

float distance_cm;
//


//Definitions
#define RDA 0x80
#define TBE 0x20  
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
  // setup the UART
  U0init(9600);
  //setup the RTC Module
  setupRTC();
  // setup the ADC
  adc_init();

  //PD0 set to Output
  *ddr_d |= 0x01;

  //Addition
  //PK2 set to Input
  *ddr_k &= 0xFD;
  //PK2 Internal Pullup enables
  *port_k |= 0x04;
  //PE3 set to Output
  *ddr_e |= 0x08;
  //


  //Initializing LCD
  lcd.begin(16, 2);
  //

  //Additon
  dht.begin();
  //
}
//


//Reading the analog signal and displaying the result
void loop() {

  //read voltage from ADC
  unsigned int Vreading = adc_read(0); 

  //send voltage over serial
  send_WL(Vreading);

  //Addition
  if(reset == 0 && distance_cm >= 1.00){
  unsigned long currentMillis = millis();
  if(currentMillis - previousMillis >= interval){
    previousMillis = currentMillis;

    sensors_event_t event;
    dht.temperature().getEvent(&event);
    lcd.setCursor(0, 0);
    lcd.print(F("Temp: "));
    lcd.print(event.temperature);
    lcd.print(F("C"));
    dht.humidity().getEvent(&event);
    lcd.setCursor(0, 1);
    lcd.print(F("Humidity: "));
    lcd.print(event.relative_humidity);
    lcd.print(F("%"));
    *port_d &= 0x2A;
  }
  //

  *port_e |= 0x08;
  }

//LCD Error Message
  if(distance_cm <= 1.00){
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Water Level is");
    lcd.setCursor(0, 1);
    lcd.print("too Low");
    *port_e &= 0x26;
    *port_d |= 0x01;
    reset = 1;
   }
    if(distance_cm >= 1.00){
      if(*pin_k &= 0x04){
        reset = 0;
       *port_d &= 0x2A;
        lcd.clear();
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
  U0putchar((unsigned char)(integer_part / 10) + '0');
  U0putchar((unsigned char)(integer_part % 10) + '0');

  // Send decimal point
  U0putchar('.');

  // Calculate and send fractional part of distance
  unsigned int fractional_part = (distance_cm - integer_part) * 100;
  U0putchar((unsigned char)(fractional_part / 10) + '0');
  U0putchar((unsigned char)(fractional_part % 10) + '0');

  U0putchar('c');
  U0putchar('m');
  // Send newline character
  U0putchar('\n');
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
    while (1) delay(10);
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
    rtc.adjust(DateTime(2024, 4, 27, 2, 19, 0)); // Adjust as needed
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
