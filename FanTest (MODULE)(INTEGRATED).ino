//Code and circuit by: Evita Kanaan
//sources: https://lastminuteengineers.com/l293d-dc-motor-arduino-tutorial/
// Motor A connections
int enA = 9;
int in1 = 8;
int in2 = 7;

void setup() {
  // Set all the motor control pins to outputs
  pinMode(enA, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
}

float temp = 10.3;
void loop() {
  while (temp >= 2) {
    // Turn on the fan
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
    analogWrite(enA, 255); // Set the fan to maximum speed
    delay(25);
  }
}

// This function lets you control spinning direction of motors
void directionControl() {
  // Set motor to maximum speed
  // For PWM maximum possible values are 0 to 255
  analogWrite(enA, 255);

  // Turn on motor A
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  delay(2000);

  // Now change motor direction
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  delay(2000);

  // Turn off motor
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
}

// This function lets you control speed of the motor
void speedControl() {
  // Turn on motor
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);

  // Accelerate from zero to maximum speed
  for (int i = 0; i < 256; i++) {
    analogWrite(enA, i);
    delay(20);
  }

  // Decelerate from maximum speed to zero
  for (int i = 255; i >= 0; --i) {
    analogWrite(enA, i);
    delay(20);
  }

  // Now turn off motor
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
}
