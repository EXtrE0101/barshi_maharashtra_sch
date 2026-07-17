/*
==========================================================
SMART HOME SECURITY SYSTEM
Arduino UNO

Features
--------------------------------
✔ 2 Ultrasonic Sensors
✔ RGB Status LED
✔ Variable Speed Alarm
✔ Green Breathing
✔ Serial Monitor
==========================================================
*/

#define TRIG1 2
#define ECHO1 3

#define TRIG2 4
#define ECHO2 5

#define BUZZER 7

#define RED 8
#define GREEN 9
#define BLUE 10

// Detection Range (cm)
const int DETECT_DISTANCE = 20;

// Variables
long distance1 = 0;
long distance2 = 0;
long nearest = 999;

int brightness = 0;
int fadeAmount = 5;

bool ledState = false;

unsigned long previousLED = 0;

//-----------------------------------
void setup()
{

  Serial.begin(9600);

  pinMode(TRIG1, OUTPUT);
  pinMode(ECHO1, INPUT);

  pinMode(TRIG2, OUTPUT);
  pinMode(ECHO2, INPUT);

  pinMode(BUZZER, OUTPUT);

  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(BLUE, OUTPUT);

  digitalWrite(RED, LOW);
  digitalWrite(GREEN, LOW);
  digitalWrite(BLUE, LOW);

  Serial.println("================================");
  Serial.println("SMART HOME SECURITY SYSTEM");
  Serial.println("Detection Range : 20 cm");
  Serial.println("================================");
}

//-----------------------------------
void loop()
{

  distance1 = getDistance(TRIG1, ECHO1);
  distance2 = getDistance(TRIG2, ECHO2);

  nearest = min(distance1, distance2);

  Serial.print("Front : ");
  Serial.print(distance1);
  Serial.print(" cm   ");

  Serial.print("Back : ");
  Serial.print(distance2);
  Serial.print(" cm   ");

  Serial.print("Nearest : ");
  Serial.print(nearest);
  Serial.println(" cm");

  //--------------------------------
  // SAFE MODE
  //--------------------------------

  if (nearest > DETECT_DISTANCE)
  {

    noTone(BUZZER);

    digitalWrite(RED, LOW);
    digitalWrite(BLUE, LOW);

    analogWrite(GREEN, brightness);

    brightness += fadeAmount;

    if (brightness >= 255)
    {
      brightness = 255;
      fadeAmount = -5;
    }

    if (brightness <= 0)
    {
      brightness = 0;
      fadeAmount = 5;
    }

    delay(15);
  }

  //--------------------------------
  // ALERT MODE
  //--------------------------------
  else
  {

    analogWrite(GREEN, 0);

    if (millis() - previousLED > 100)
    {

      previousLED = millis();

      ledState = !ledState;

      digitalWrite(RED, ledState);
      digitalWrite(BLUE, !ledState);

    }




    //--------------------------------
    // 16–20 cm
    // Slow Beep
    //--------------------------------
    if (nearest > 15)
    {
      tone(BUZZER, 1800);
      delay(30);
      noTone(BUZZER);
      delay(250);
    }

    //--------------------------------
    // 11–15 cm
    // Medium Beep
    //--------------------------------
    else if (nearest > 10)
    {
      tone(BUZZER, 2200);
      delay(35);
      noTone(BUZZER);
      delay(150);
    }

    //--------------------------------
    // 6–10 cm
    // Fast Beep
    //--------------------------------
    else if (nearest > 5)
    {
      tone(BUZZER, 2800);
      delay(40);
      noTone(BUZZER);
      delay(60);
    }

    //--------------------------------
    // 0–5 cm
    // Continuous Alarm
    //--------------------------------
    else
    {
      tone(BUZZER, 3200);
      delay(10);
    }

  }

  delay(20);

}

//=====================================
// Read Ultrasonic Distance
//=====================================
long getDistance(int trigPin, int echoPin)
{
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);

  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH, 30000);

  if (duration == 0)
  {
    return 999;
  }

  long distance = duration * 0.0343 / 2;

  return distance;
}
