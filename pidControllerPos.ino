/*
    Controlling DC Motor using MDD10A motor driver and encoder using PID control system.
*/

#define PWM_PIN 13
#define DIR_PIN 12
#define ENCA 4
#define ENCB 5

volatile int pos_i = 0;
unsigned long prevTime = 0;

int target = 0;

const float kp = 1;
const float ki = 0.05;
const float kd = 0.01;

float prevError = 0;
float integral = 0;

void setup()
{
    Serial.begin(9600);

    // Setup motor driver pins
    pinMode(PWM_PIN, OUTPUT);
    pinMode(DIR_PIN, OUTPUT);

    // Setup encoder pins
    pinMode(ENCA, INPUT);
    pinMode(ENCB, INPUT);

    attachInterrupt(digitalPinToInterrupt(ENCA), readEncoder, RISING);

    // Assign prevTime at setup so as to prevent initial motor jerks
    prevTime = micros();
}

void loop()
{
    // Read 'target' from Serial input
    if (Serial.available())
    {
        String input = Serial.readStringUntil('\n');
        target = input.toInt();
        prevError = 0;
        integral = 0;
    }

    // Prevents race condition
    noInterrupts();
    int pos = pos_i;
    interrupts();
    
    // Time and deltatime calculation
    unsigned long curTime = micros();
    float deltaTime = ((float) (curTime - prevTime)) * 1e-6;
    prevTime = curTime;

    // PID controller
    float curError = (float) (target - pos);

    float tolerance = max(5.0, abs(target) * 0.05); // 5% tolerance
    if (abs(curError) < tolerance)
    {
        setMotor(0, 0);
        integral = 0;
        prevError = curError;
        return; // Just skips the current execution of function, arduino/esp32 calls the function again
    }

    integral += curError * deltaTime;
    float derivative = (curError - prevError) / deltaTime;
    float pid = kp * curError + ki * integral + kd * derivative;

    // Set the motor using the PWM value got from the PID controller
    int pwmVal = min((int) abs(pid), 255);
    int dir = pid > 0 ? -1 : 1; // CONVENTION
    setMotor(pwmVal, dir);

    prevError = curError;
}

void setMotor(int pwmVal, int dir)
{
    // MDD10A Motor driver
    if (dir == 1)
    {
        digitalWrite(DIR_PIN, LOW);
        analogWrite(PWM_PIN, pwmVal);
    }
    else if (dir == -1)
    {
        digitalWrite(DIR_PIN, HIGH);
        analogWrite(PWM_PIN, pwmVal);
    }
    else
    {
        analogWrite(PWM_PIN, 0);
    }
}

void readEncoder()
{
    if (digitalRead(ENCB))
    {
        pos_i++;
    }
    else
    {
        pos_i--;
    }
}
