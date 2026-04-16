// Cytron MDD10A motor driver
// To operator DC Motor

#define PWM_PIN 13
#define DIR_PIN 12

int state = 0;

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
        state = input.toInt();
    }

    switch (state)
    {
        case 0:
            digitalWrite(PWM_PIN, LOW);
            break;
        case 1:
            // STOP motor -> Set direction -> Enable PWM
            // Setting direction before enabling PWM prevents some jerks
            digitalWrite(PWM_PIN, LOW); // To prevent abrupt change from reverse to forward
            delay(100);

            digitalWrite(DIR_PIN, HIGH);
            digitalWrite(PWM_PIN, HIGH);
            break;
        case 2:
            digitalWrite(PWM_PIN, LOW); // To prevent abrupt change from forward to reverse
            delay(100);

            digitalWrite(DIR_PIN, LOW);
            digitalWrite(PWM_PIN, HIGH);
            break;
        default:
            Serial.println("Invalid state chosen. Choose again from 0 to 2.");
    }
}
