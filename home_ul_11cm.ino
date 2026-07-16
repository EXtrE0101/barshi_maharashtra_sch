// ----------------------------
// Ultrasonic Alarm System
// Arduino UNO + HC-SR04 + Buzzer
// ----------------------------

#define TRIG_PIN 5
#define ECHO_PIN 6
#define BUZZER_PIN 11

// Detection distance (cm)
const int DETECTION_DISTANCE = 11;

long duration;
float distance;

void setup() {

  Serial.begin(9600);

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  pinMode(BUZZER_PIN, OUTPUT);

  digitalWrite(BUZZER_PIN, LOW);

  Serial.println("--------------------------------");
  Serial.println("ULTRASONIC ALARM SYSTEM");
  Serial.print("Detection Distance: ");
  Serial.print(DETECTION_DISTANCE);
  Serial.println(" cm");
  Serial.println("--------------------------------");
}

void loop() {

  // Clear trigger
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);

  // Send 10 µs pulse
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // Read echo
  duration = pulseIn(ECHO_PIN, HIGH, 30000);

  // No echo received
  if (duration == 0) {
    Serial.println("Out of Range");
    digitalWrite(BUZZER_PIN, LOW);
    delay(200);
    return;
  }

  // Calculate distance (cm)
  distance = duration * 0.0343 / 2;

  // Print distance
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  // Alarm
  if (distance <= DETECTION_DISTANCE) {

    digitalWrite(BUZZER_PIN, HIGH);

    Serial.println(">>> OBJECT DETECTED <<<");

  } else {

    digitalWrite(BUZZER_PIN, LOW);

  }

  delay(200);
}
