#define LED_PIN 5

int state = 0;
int brightness = 0;
bool stateChosen = false;

void setup()
{
    pinMode(LED_PIN, OUTPUT);
    Serial.begin(9600);
}

void loop()
{
    // To switch the state once chosen, press the RESET button on the microcontroller
    if (Serial.available())
    {
        String input = Serial.readStringUntil('\n');
        if (!stateChosen) 
        {
            state = input.toInt();
            stateChosen = true;
        }
        else
        {
            brightness = input.toInt();
            brightness = max(0, min(255, brightness));
        }
    }

    switch (state)
    {
        case 0:
            ledBlink();
            break;
        case 1:
            ledFade();
            break;
        case 2:
            ledSerial();
            break;
        default:
            stateChosen = false;
            Serial.println("Invalid state chosen. Choose again from 0 to 2.");
    }
}

void ledBlink()
{
    digitalWrite(LED_PIN, HIGH);
    delay(1000);
    digitalWrite(LED_PIN, LOW);
    delay(1000);
}

void ledFade()
{
    for (int i = 0; i <= 255; i++)
    {
        analogWrite(LED_PIN, i);
        delay(50);
    }

    for (int i = 255; i >= 0; i--)
    {
        analogWrite(LED_PIN, i);
        delay(50);
    }
}

void ledSerial()
{
    analogWrite(LED_PIN, brightness);
}
