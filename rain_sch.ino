#include <Servo.h>

Servo roof;

// -------- Pins --------
#define RAIN_SENSOR 2
#define SERVO_PIN   3

#define RED_LED     8
#define GREEN_LED   9
#define BLUE_LED    10

#define BUZZER      11

bool rainDetected = false;

void setup() {

  Serial.begin(9600);

  pinMode(RAIN_SENSOR, INPUT);

  pinMode(RED_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(BLUE_LED, OUTPUT);

  pinMode(BUZZER, OUTPUT);

  roof.attach(SERVO_PIN);
  roof.write(0);

  Serial.println("SMART RAIN PROTECTION SYSTEM");
}

void loop() {

  // LOW = Rain Detected (most rain sensors)
  rainDetected = (digitalRead(RAIN_SENSOR) == LOW);

  if (rainDetected) {

    roof.write(90);

    Serial.println("RAIN DETECTED - CLOSING ROOF");

    rainMode();

  } else {

    roof.write(0);

    noRainMode();

  }
}

//==============================
// NO RAIN MODE
//==============================
void noRainMode() {

  digitalWrite(RED_LED, LOW);
  digitalWrite(BLUE_LED, LOW);

  noTone(BUZZER);

  // Breathing Green LED
  for (int b = 0; b <= 255; b += 5) {

    if (digitalRead(RAIN_SENSOR) == LOW) return;

    analogWrite(GREEN_LED, b);
    delay(15);
  }

  for (int b = 255; b >= 0; b -= 5) {

    if (digitalRead(RAIN_SENSOR) == LOW) return;

    analogWrite(GREEN_LED, b);
    delay(15);
  }
}

//==============================
// RAIN MODE
//==============================
void rainMode() {

  analogWrite(GREEN_LED, 0);

  while (digitalRead(RAIN_SENSOR) == LOW) {

    // RED
    digitalWrite(RED_LED, HIGH);
    digitalWrite(BLUE_LED, LOW);

    tone(BUZZER, 1200);
    delay(250);

    // BLUE
    digitalWrite(RED_LED, LOW);
    digitalWrite(BLUE_LED, HIGH);

    tone(BUZZER, 700);
    delay(250);
  }

  noTone(BUZZER);

  digitalWrite(RED_LED, LOW);
  digitalWrite(BLUE_LED, LOW);
}
