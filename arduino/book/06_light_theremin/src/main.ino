#include <Arduino.h>

const int BAUD_RATE = 9600;

const int LED_PIN = 13;
const int PIEZO_PIN = 8;
const int SENSOR_PIN = A0;

const int PITCH_MIN = 50;
const int PITCH_MAX = 4000;
const int TONE_DURATION = 20;

int sensorMin = 0;
int sensorMax = 1023;
int sensorVal;

int pitch;

void setup() {
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, HIGH);

  while (millis() < 5000) {
    sensorVal = analogRead(SENSOR_PIN);

    if (sensorVal > sensorMax) sensorMax = sensorVal;
    if (sensorVal < sensorMin) sensorMin = sensorVal;
  }

  digitalWrite(SENSOR_PIN, LOW);

  Serial.begin(BAUD_RATE);
}

void loop() {
  sensorVal = analogRead(SENSOR_PIN);

  pitch = map(sensorVal, sensorMin, sensorMax, PITCH_MIN, PITCH_MAX);

  tone(PIEZO_PIN, pitch, TONE_DURATION);

  Serial.print("sensorVal: ");
  Serial.print(sensorVal);
  Serial.print("\tpitch: ");
  Serial.println(pitch);

  delay(10);
}
