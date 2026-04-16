// SENSOR SLAVE
#include <Wire.h>

#define SLAVE_ADDRESS 69
#define TEMP_PIN A0
#define TRIGGER_PIN 10
#define ECHO_PIN 11

int tempValue = 0;
int duration;

void setup()
{
  Wire.begin(SLAVE_ADDRESS);
  pinMode(TEMP_PIN, INPUT);
  Wire.onRequest(requestEvent);
  pinMode(TRIGGER_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
}

void requestEvent()
{
  Wire.write(highByte(tempValue));
  Wire.write(lowByte(tempValue));
  Wire.write(highByte(duration));
  Wire.write(lowByte(duration));
}

void loop()
{
  tempValue = analogRead(TEMP_PIN);
  
  digitalWrite(TRIGGER_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIGGER_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGGER_PIN, LOW);
  
  duration = pulseIn(ECHO_PIN, HIGH); // Measures in microseconds
  
  delay(50);
}
