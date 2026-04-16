#define TRIGGER_PIN 10
#define ECHO_PIN 11

float duration, distance;

void setup()
{
  pinMode(TRIGGER_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  Serial.begin(9600);
}

void loop()
{
  digitalWrite(TRIGGER_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIGGER_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGGER_PIN, LOW);
  
  duration = pulseIn(ECHO_PIN, HIGH); // Measures in microseconds
  
  distance = duration * 0.0343 / 2.0;
  
  Serial.print("Distance = ");
  if (distance >= 333.98 || distance <= 2.6) {
  	Serial.println("Out of range");
  } else {
  	Serial.print(distance);
    Serial.println(" cm");
  }
  delay(500);
}