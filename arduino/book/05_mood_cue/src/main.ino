#include <Arduino.h>
#include <Servo.h>

Servo servo;

const int BAUD_RATE = 9600;

const int MIN_VAL = 0;
const int MAX_VAL = 1023;

const int MIN_ANGLE = 0;
const int MAX_ANGLE = 179;

const int POT_PIN = A0;
const int SERVO_PIN = 5;

int potVal;
int angle;

void setup() {
  servo.attach(SERVO_PIN);
  Serial.begin(BAUD_RATE);
}

void loop() {
  potVal = analogRead(POT_PIN);
  angle = map(potVal, MIN_VAL, MAX_VAL, MIN_ANGLE, MAX_ANGLE);

  Serial.print("potVal: ");
  Serial.print(potVal);
  Serial.print("\tangle: ");
  Serial.println(angle);

  servo.write(angle);
  delay(15);
}
