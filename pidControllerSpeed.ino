/*
    Controlling DC Motor speed using MDD10A motor driver and encoder using PID control system.
*/

#define PWM_PIN 13
#define DIR_PIN 12
#define ENCA 4
#define ENCB 5

const int COUNTS_PER_REV = 600; // FIX: Change the COUNTS_PER_REV depending upon the encoder

volatile int pos_i = 0;
int prevPos = 0;

float velocity = 0;
float filteredV = 0;

unsigned long prevTime = 0;

float target = 0;

const float kp = 1.2;
const float ki = 0.05;
const float kd = 0.01;

float prevError = 0;
float integral = 0;

void IRAM_ATTR readEncoder();

void setup()
{
    Serial.begin(115200);

    pinMode(PWM_PIN, OUTPUT);
    pinMode(DIR_PIN, OUTPUT);

    pinMode(ENCA, INPUT);
    pinMode(ENCB, INPUT);

    attachInterrupt(digitalPinToInterrupt(ENCA), readEncoder, RISING);

    prevTime = micros();
}

void loop()
{
    // Square wave target
    target = 100 * (sin(prevTime * 1e-6) > 0);

    // Prevents race condition
    noInterrupts();
    int pos = pos_i;
    interrupts();

    // Time
    unsigned long curTime = micros();
    float deltaTime = (curTime - prevTime) * 1e-6;
    prevTime = curTime;

    // Velocity (counts/sec)
    float rawVel = (pos - prevPos) / deltaTime;
    prevPos = pos;

    // Convert to RPM
    float rpm = (rawVel / COUNTS_PER_REV) * 60.0;

    // Low-pass filter (~20 to 30 Hz)
    filteredV = 0.7 * filteredV + 0.3 * rpm;
    velocity = filteredV;

    // PID (speed control)
    float error = target - velocity;

    integral += error * deltaTime;

    // Anti-windup
    if (integral > 10000) integral = 10000;
    if (integral < -10000) integral = -10000;

    float derivative = (error - prevError) / deltaTime;

    float pid = kp * error + ki * integral + kd * derivative;

    // Clamp
    if (pid > 255) pid = 255;
    if (pid < -255) pid = -255;

    int pwmVal = abs(pid);

    // Deadzone compensation
    if (pwmVal > 0 && pwmVal < 60)
        pwmVal = 60;

    int dir = (pid > 0) ? 1 : -1;

    setMotor(pwmVal, dir);

    Serial.print(target);
    Serial.print(" ");
    Serial.println(velocity);

    prevError = error;
}

void setMotor(int pwmVal, int dir)
{
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

// ISR (RISING-based direction detection)
void IRAM_ATTR readEncoder()
{
    if (digitalRead(ENCB))
        pos_i++;
    else
        pos_i--;
}
