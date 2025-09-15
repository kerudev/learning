#include <Arduino.h>

const int BAUD_RATE = 9600;

const int R_LED_PIN = 10;
const int G_LED_PIN = 9;
const int B_LED_PIN = 11;

const int R_SENSOR_PIN = A0;
const int G_SENSOR_PIN = A1;
const int B_SENSOR_PIN = A2;

int rSensorVal;
int gSensorVal;
int bSensorVal;

int rVal;
int gVal;
int bVal;

const int LEDS[] = {2, 3, 4};
const int LEDS_LEN = sizeof(LEDS) / sizeof(LEDS[0]);

void setup() {
  Serial.begin(BAUD_RATE);

  pinMode(G_LED_PIN, OUTPUT);
  pinMode(R_LED_PIN, OUTPUT);
  pinMode(B_LED_PIN, OUTPUT);
}

void loop() {
  rSensorVal = analogRead(R_SENSOR_PIN);
  delay(5);
  gSensorVal = analogRead(G_SENSOR_PIN);
  delay(5);
  bSensorVal = analogRead(B_SENSOR_PIN);
  delay(5);

  rVal = rSensorVal / 4;
  gVal = gSensorVal / 4;
  bVal = bSensorVal / 4;

  Serial.print("R: ");
  Serial.print(rSensorVal);
  Serial.print("\tG: ");
  Serial.print(gSensorVal);
  Serial.print("\tB: ");
  Serial.print(bSensorVal);
  Serial.println("\t(sensor)");
  Serial.print("R: ");
  Serial.print(rVal);
  Serial.print("\tG: ");
  Serial.print(gVal);
  Serial.print("\tB: ");
  Serial.print(bVal);
  Serial.println("\t(color)");
  Serial.println("");

  analogWrite(R_LED_PIN, rVal);
  analogWrite(G_LED_PIN, gVal);
  analogWrite(B_LED_PIN, bVal);
}
