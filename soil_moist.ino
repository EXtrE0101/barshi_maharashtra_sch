#include <TM1637Display.h>
#include <DHT.h>

// ---------------- DHT ----------------
#define DHTPIN 2
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// ---------------- Soil ----------------
#define SOIL_PIN A0

// ---------------- Display 1 (Temperature) ----------------
#define CLK1 4
#define DIO1 5
TM1637Display tempDisplay(CLK1, DIO1);

// ---------------- Display 2 (Humidity) ----------------
#define CLK2 6
#define DIO2 7
TM1637Display humDisplay(CLK2, DIO2);

// ---------------- RGB ----------------
#define RED_LED 8
#define GREEN_LED 9
#define BLUE_LED 10

// ---------------- Buzzer ----------------
#define BUZZER 11

// ---------------- Thresholds ----------------
// Change these after calibration
const int DRY_LEVEL = 700;
const int WET_LEVEL = 350;

// ---------------- Variables ----------------
float temperature = 0;
float humidity = 0;

int soilValue = 0;

unsigned long previousSensor = 0;
unsigned long previousLED = 0;
unsigned long previousSiren = 0;

const unsigned long sensorInterval = 1000;

bool ledState = false;
bool sirenState = false;

int breathValue = 0;
int breathDirection = 5;

enum SoilState
{
  GOOD,
  FLOODED,
  DRY
};

SoilState soilStatus = GOOD;

// ---------------- Setup ----------------
void setup()
{
  Serial.begin(9600);

  dht.begin();

  pinMode(RED_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(BLUE_LED, OUTPUT);

  pinMode(BUZZER, OUTPUT);

  digitalWrite(RED_LED, LOW);
  digitalWrite(GREEN_LED, LOW);
  digitalWrite(BLUE_LED, LOW);

  noTone(BUZZER);

  tempDisplay.setBrightness(7);
  humDisplay.setBrightness(7);

  tempDisplay.showNumberDec(0, true);
  humDisplay.showNumberDec(0, true);

  Serial.println("=================================");
  Serial.println("SMART PLANT MONITOR");
  Serial.println("=================================");
  Serial.println("Initializing...");
}

//====================================================
// LOOP
//====================================================

void loop()
{
    unsigned long currentMillis = millis();

    // Read sensors every second
    if (currentMillis - previousSensor >= sensorInterval)
    {
        previousSensor = currentMillis;

        temperature = dht.readTemperature();
        humidity = dht.readHumidity();

        soilValue = analogRead(SOIL_PIN);

        // Display Temperature
        if (!isnan(temperature))
        {
            tempDisplay.showNumberDec((int)temperature, true);
        }

        // Display Humidity
        if (!isnan(humidity))
        {
            humDisplay.showNumberDec((int)humidity, true);
        }

        // Determine Soil Status
        if (soilValue > DRY_LEVEL)
        {
            soilStatus = DRY;
        }
        else if (soilValue < WET_LEVEL)
        {
            soilStatus = FLOODED;
        }
        else
        {
            soilStatus = GOOD;
        }

        // Serial Monitor
        Serial.println("--------------------------------");

        Serial.print("Temperature : ");
        Serial.print(temperature);
        Serial.println(" C");

        Serial.print("Humidity    : ");
        Serial.print(humidity);
        Serial.println(" %");

        Serial.print("Soil Value  : ");
        Serial.println(soilValue);

        switch (soilStatus)
        {
            case GOOD:
                Serial.println("Status : GOOD SOIL");
                break;

            case FLOODED:
                Serial.println("Status : FLOODED");
                break;

            case DRY:
                Serial.println("Status : DRY");
                break;
        }
    }

    // Run LED and buzzer effects
    updateEffects();
}
//====================================================
// LED & BUZZER EFFECTS
//====================================================

void updateEffects()
{
    unsigned long currentMillis = millis();

    switch (soilStatus)
    {

    //==========================
    // GOOD SOIL
    //==========================
    case GOOD:

        noTone(BUZZER);

        digitalWrite(RED_LED, LOW);
        digitalWrite(BLUE_LED, LOW);

        // Breathing Green
        if (currentMillis - previousLED >= 20)
        {
            previousLED = currentMillis;

            analogWrite(GREEN_LED, breathValue);

            breathValue += breathDirection;

            if (breathValue >= 255)
            {
                breathValue = 255;
                breathDirection = -5;
            }

            if (breathValue <= 0)
            {
                breathValue = 0;
                breathDirection = 5;
            }
        }

        break;

    //==========================
    // FLOODED
    //==========================
    case FLOODED:

        noTone(BUZZER);

        analogWrite(GREEN_LED, 0);
        digitalWrite(RED_LED, LOW);

        if (currentMillis - previousLED >= 500)
        {
            previousLED = currentMillis;

            ledState = !ledState;

            digitalWrite(BLUE_LED, ledState);
        }

        break;

    //==========================
    // DRY
    //==========================
    case DRY:

        analogWrite(GREEN_LED, 0);
        digitalWrite(BLUE_LED, LOW);

        // Fast Flash Red
        if (currentMillis - previousLED >= 120)
        {
            previousLED = currentMillis;

            ledState = !ledState;

            digitalWrite(RED_LED, ledState);
        }

        // Siren
        if (currentMillis - previousSiren >= 150)
        {
            previousSiren = currentMillis;

            sirenState = !sirenState;

            if (sirenState)
            {
                tone(BUZZER, 1500);
            }
            else
            {
                tone(BUZZER, 700);
            }
        }

        break;
    }
}

