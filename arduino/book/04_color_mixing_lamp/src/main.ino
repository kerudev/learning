#include <Arduino.h>

const int R_LED_PIN = 10;
const int G_LED_PIN = 9;
const int B_LED_PIN = 11;

const int R_SENSOR_PIN = A0;
const int G_SENSOR_PIN = A1;
const int B_SENSOR_PIN = A2;

int redSensorVal = 0;
int greenSensorVal = 0;
int blueSensorVal = 0;

int redVal = 0;
int greenVal = 0;
int blueVal = 0;

const int LEDS[] = {2, 3, 4};
const int LEDS_LEN = sizeof(LEDS) / sizeof(LEDS[0]);

void setup() {
  Serial.begin(9600);

  pinMode(G_LED_PIN, OUTPUT);
  pinMode(R_LED_PIN, OUTPUT);
  pinMode(B_LED_PIN, OUTPUT);
}

void loop() {
  redSensorVal = analogRead(R_SENSOR_PIN);
  delay(5);
  greenSensorVal = analogRead(G_SENSOR_PIN);
  delay(5);
  blueSensorVal = analogRead(B_SENSOR_PIN);
  delay(5);

  redVal = redSensorVal / 4;
  greenVal = greenSensorVal / 4;
  blueVal = blueSensorVal / 4;

  Serial.print("R: ");
  Serial.print(redSensorVal);
  Serial.print("\tG: ");
  Serial.print(greenSensorVal);
  Serial.print("\tB: ");
  Serial.print(blueSensorVal);
  Serial.println("\t(sensor)");
  Serial.print("R: ");
  Serial.print(redVal);
  Serial.print("\tG: ");
  Serial.print(greenVal);
  Serial.print("\tB: ");
  Serial.print(blueVal);
  Serial.println("\t(color)");
  Serial.println("");

  analogWrite(R_LED_PIN, redVal);
  analogWrite(G_LED_PIN, greenVal);
  analogWrite(B_LED_PIN, blueVal);
}
