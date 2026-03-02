#include <Servo.h>

Servo myservo;

const int trigPin = 11;
const int echoPin = 12;

const float detectDistance = 8;  // object closer than 8 cm
const unsigned long holdTime = 1000; // hold lid open for 1 second

unsigned long openTime = 0;
bool lidOpen = false;

void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  Serial.begin(9600);
  myservo.attach(9);
  myservo.write(0); 
}

void loop() {
  // Send ultrasonic pulse
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Measure echo duration and calculate distance
  float duration = pulseIn(echoPin, HIGH);
  float distance = (duration * 0.0343) / 2;  // in cm

  Serial.print("Distance: ");
  Serial.println(distance);

  if (distance > 0 && distance <= detectDistance) {
    // Object detected → open lid
    myservo.write(90);
    lidOpen = true;
    openTime = millis(); // record the time we opened the lid
  } 
  else if (lidOpen) {
    // Object gone → check if hold time has passed
    if (millis() - openTime >= holdTime) {
      myservo.write(0); // close lid
      lidOpen = false;  // reset lid state
    }
  }

  delay(50); // small delay for stable readings
}