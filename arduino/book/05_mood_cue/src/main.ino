#include <Arduino.h>
#include <Servo.h>

Servo servo;

int const MIN_VAL = 0;
int const MAX_VAL = 1023;

int const MIN_ANGLE = 0;
int const MAX_ANGLE = 179;

int const POT_PIN = A0;
int potVal;
int angle;

void setup() {
  servo.attach(5);
  Serial.begin(9600);
}

void loop() {
  potVal = analogRead(POT_PIN);
  angle = map(potVal, MIN_VAL, MAX_VAL, MIN_ANGLE, MAX_ANGLE);

  Serial.print("potVal: ");
  Serial.print(potVal);
  Serial.print(", angle: ");
  Serial.println(angle);

  servo.write(angle);
  delay(15);
}
