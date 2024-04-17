
#define SIGNAL_PIN A0
int value = 0; // variable to store the sensor value
void setup() {
Serial.begin(9600);
digitalWrite (POWER_PIN, LOW); // turn the sensor OFF
}
void loop() {

value = analogRead (SIGNAL_PIN); // read the analog value from sensor
Serial.print("Sensor value: " );
Serial.println (value);
delay(1000);
}
