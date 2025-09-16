#include <Arduino.h>

const int BAUD_RATE = 9600;

const int PIEZO_PIN = 8;
const int SENSOR_PIN = A0;

const int C_HERTZ = 262;
const int D_HERTZ = 294;
const int E_HERTZ = 330;
const int F_HERTZ = 349;

const int PITCH_MIN = 50;
const int PITCH_MAX = 4000;
const int TONE_DURATION = 20;

int SENSOR_MIN = 0;
int SENSOR_MAX = 1023;

int keyVal;

void setup() {
  Serial.begin(BAUD_RATE);
}

void loop() {
  keyVal = analogRead(SENSOR_PIN);

  Serial.print("keyVal: ");
  Serial.println(keyVal);

  if (keyVal == SENSOR_MAX)
    tone(PIEZO_PIN, C_HERTZ);
  else if (keyVal >= 990 && keyVal <= 1110)
    tone(PIEZO_PIN, D_HERTZ);
  else if (keyVal >= 505 && keyVal <= 515)
    tone(PIEZO_PIN, E_HERTZ);
  else if (keyVal >= 5 && keyVal <= 10)
    tone(PIEZO_PIN, F_HERTZ);
  else
    noTone(PIEZO_PIN);
}
