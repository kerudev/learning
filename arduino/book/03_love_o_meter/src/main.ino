#include <Arduino.h>

const int BAUD_RATE = 9600;

const int SENSOR_PIN = A0;

const float BASELINE_TEMP = 25.0;
const float VOLTAGE_IN = 5.0;

const int LEDS[] = {2, 3, 4};
const int LEDS_LEN = sizeof(LEDS) / sizeof(LEDS[0]);

void setup() {
  Serial.begin(BAUD_RATE);

  for (int n = 0; n < LEDS_LEN; n++) {
    pinMode(LEDS[n], OUTPUT);
    digitalWrite(LEDS[n], LOW);
  }
}

void loop() {
  int sensorVal = analogRead(SENSOR_PIN);
  float volts = (sensorVal / 1024.0) * VOLTAGE_IN;
  float temp = (volts - 0.5) * 100;

  Serial.print("Sensor value: ");
  Serial.print(sensorVal);
  Serial.print(", Volts: ");
  Serial.print(volts);
  Serial.print("V, Temperature: ");
  Serial.print(temp);
  Serial.println("C");

  int level = (temp - BASELINE_TEMP) / 2;

  // HIGH == 1, LOW == 0
  for (int n = 0; n < LEDS_LEN; n++)
    digitalWrite(LEDS[n], n < level);

  delay(1);
}
