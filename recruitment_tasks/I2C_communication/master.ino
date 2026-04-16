// MASTER
#include <Wire.h>

#define DC_SLAVE_ADDRESS 67
#define SERVO_SLAVE_ADDRESS 68
#define SENSOR_SLAVE_ADDRESS 69
#define POT_PIN A0
#define SLIDE_SWITCH_PIN 7
#define BUTTON_PIN 6

bool mode = true;
int prevButtonState = HIGH;
int curButtonState = HIGH;
int value;
unsigned long lastPressTime;
int debounceDelay = 50;

void setup()
{
  Wire.begin();
  pinMode(POT_PIN, INPUT);
  pinMode(SLIDE_SWITCH_PIN, INPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  Serial.begin(9600);
}

void loop()
{
  curButtonState = digitalRead(BUTTON_PIN);
  if (prevButtonState == HIGH && curButtonState == LOW)
  {
    if (millis() - lastPressTime > debounceDelay)
    {
      mode = !mode;
      lastPressTime = millis();
    }
  }
  prevButtonState = curButtonState;
  
  if (mode)
  {
  	value = analogRead(POT_PIN);
  }
  else if (Serial.available())
  {
    value = Serial.parseInt();
    value = max(0, value);
    value = min(1023, value);
  }
  
  Wire.requestFrom(SENSOR_SLAVE_ADDRESS, 4);
  if (Wire.available() == 4)
  {
    int tempHighByte = Wire.read();
    int tempLowByte = Wire.read();
    int tempValue = map(word(tempHighByte, tempLowByte), 20, 358, -40, 125);
    
    int durationHighByte = Wire.read();
    int durationLowByte = Wire.read();
    int duration = word(durationHighByte, durationLowByte);
    float distance = duration * 0.0343 / 2.0;
    
    Serial.print("Temperature = ");
    Serial.print(tempValue);
    Serial.print(" deg C, ");
    
    Serial.print("Distance = ");
    if (distance >= 333.98 || distance <= 2.6)
    {
      Serial.println("Out of range");
    }
    else
    {
      Serial.print(distance);
      Serial.println(" cm");
    }
  }

  if (digitalRead(SLIDE_SWITCH_PIN))
  {
    Wire.beginTransmission(DC_SLAVE_ADDRESS);
    Wire.write(map(value, 0, 1023, 0, 255));
    Wire.endTransmission();
  }
  else
  {
    Wire.beginTransmission(DC_SLAVE_ADDRESS);
    Wire.write(128);
    Wire.endTransmission();
    
    Wire.beginTransmission(SERVO_SLAVE_ADDRESS);
    Wire.write(map(value, 0, 1023, 0, 180));
    Wire.endTransmission();
  }
  
  delay(50);
}