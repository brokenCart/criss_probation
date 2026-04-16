// DC SLAVE
#include <Wire.h>

#define SLAVE_ADDRESS 67
#define INPUT1_PIN 4
#define INPUT2_PIN 5
#define ENABLE_12_PIN 10

int speed;

void setup()
{
  Wire.begin(SLAVE_ADDRESS);
  Wire.onReceive(receiveEvent);
  pinMode(INPUT1_PIN, OUTPUT);
  pinMode(INPUT2_PIN, OUTPUT);
  pinMode(ENABLE_12_PIN, OUTPUT);
}

void receiveEvent(int howMany)
{
  if (Wire.available())
  {
    int value = Wire.read();
    if (value > 127)
    {
      speed = map(value, 128, 255, 0, 255);
      digitalWrite(INPUT1_PIN, LOW);
      digitalWrite(INPUT2_PIN, HIGH);
    }
    else
    {
      speed = map(value, 0, 127, 255, 1);
      digitalWrite(INPUT1_PIN, HIGH);
      digitalWrite(INPUT2_PIN, LOW);
    }
    analogWrite(ENABLE_12_PIN, speed);
  }
}

void loop()
{
  delay(50);
}
