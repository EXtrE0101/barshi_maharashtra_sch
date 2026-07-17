#define MQ4 A0

#define RED 10
#define GREEN 9
#define BLUE 8

#define BUZZER 11

const int SAFE_LIMIT = 270;
const int WARNING_LIMIT = 280;

unsigned long previousLED = 0;
unsigned long previousTone = 0;

bool ledState = false;
bool toneState = false;

int brightness = 0;
int fadeAmount = 5;

void setup()
{
  Serial.begin(9600);

  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(BLUE, OUTPUT);

  pinMode(BUZZER, OUTPUT);

  Serial.println("MQ4 GAS MONITOR");
}

void loop()
{
  int gas = analogRead(MQ4);

  Serial.print("Gas Value : ");
  Serial.println(gas);

  if (gas < SAFE_LIMIT)
  {
    safeMode();
  }
  else if (gas < WARNING_LIMIT)
  {
    warningMode();
  }
  else
  {
    dangerMode();
  }
}

//================ SAFE ==================
void safeMode()
{
  noTone(BUZZER);

  digitalWrite(RED, LOW);
  digitalWrite(BLUE, LOW);

  brightness += fadeAmount;

  if (brightness <= 0 || brightness >= 255)
    fadeAmount = -fadeAmount;

  analogWrite(GREEN, brightness);

  delay(15);
}

//=============== WARNING =================
void warningMode()
{
  analogWrite(GREEN, 0);
  digitalWrite(RED, LOW);

  if (millis() - previousLED > 400)
  {
    previousLED = millis();

    ledState = !ledState;

    digitalWrite(BLUE, ledState);
  }

  if (millis() - previousTone > 600)
  {
    previousTone = millis();

    toneState = !toneState;

    if (toneState)
      tone(BUZZER, 1500);
    else
      noTone(BUZZER);
  }
}

//=============== DANGER ==================
void dangerMode()
{
  analogWrite(GREEN, 0);
  digitalWrite(BLUE, LOW);

  if (millis() - previousLED > 120)
  {
    previousLED = millis();

    ledState = !ledState;

    digitalWrite(RED, ledState);
  }

  // Loud alternating siren
  if (millis() - previousTone > 80)
  {
    previousTone = millis();

    toneState = !toneState;

    if (toneState)
      tone(BUZZER, 3000);   // High tone
    else
      tone(BUZZER, 1200);   // Low tone
  }

  Serial.println("***** GAS LEAK DETECTED! *****");
}
