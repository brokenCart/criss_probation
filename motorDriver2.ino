// Cytron MDD10A motor driver
// To operator DC Motor using serial input

#define PWM_PIN 13
#define DIR_PIN 12

int value = 0;

void setup()
{
    pinMode(PWM_PIN, OUTPUT);
    pinMode(DIR_PIN, OUTPUT);
    Serial.begin(9600);
}

void loop()
{
    if (Serial.available())
    {
        String input = Serial.readStringUntil('\n');
        value = input.toInt();
        value = max(0, min(255, value));
    }

    if (value > 127)
    {
        digitalWrite(DIR_PIN, HIGH);
        analogWrite(PWM_PIN, map(value, 128, 255, 1, 255));
    }
    else
    {
        digitalWrite(DIR_PIN, LOW);
        analogWrite(PWM_PIN, map(value, 0, 127, 255, 0));
    }
}
