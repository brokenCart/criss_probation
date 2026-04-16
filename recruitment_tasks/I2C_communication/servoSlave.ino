// SERVO SLAVE
#include <Wire.h>
#include <Servo.h>

#define SLAVE_ADDRESS 68
#define SERVO_PIN 13

Servo servo;
int prevAngle = 0;
int curAngle = 0;

void setup()
{
  Wire.begin(SLAVE_ADDRESS);
  Wire.onReceive(receiveEvent);
  servo.attach(SERVO_PIN);
}

void receiveEvent(int howMany)
{
  if (Wire.available())
  {
    curAngle = Wire.read();
  }
}

void loop()
{
  while (prevAngle != curAngle)
  {
    servo.write(prevAngle);
  	if (prevAngle > curAngle)
    {
      prevAngle--;
    }
    else
    {
      prevAngle++;
    }
  }
  delay(50);
}
