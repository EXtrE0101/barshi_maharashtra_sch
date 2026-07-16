#include <TM1637Display.h>

// -------- Pins --------
#define ENTRY_IR 2
#define EXIT_IR 3
#define CLK 4
#define DIO 5
#define BUZZER 11

TM1637Display display(CLK, DIO);

// -------- Settings --------
const int MAX_PEOPLE = 4;
int people = 0;

// Previous sensor states
bool lastEntry = HIGH;
bool lastExit = HIGH;

void updateDisplay() {
  display.showNumberDec(people, true, 4, 0);
}

void setup() {
  Serial.begin(9600);

  pinMode(ENTRY_IR, INPUT);
  pinMode(EXIT_IR, INPUT);
  pinMode(BUZZER, OUTPUT);

  digitalWrite(BUZZER, LOW);

  display.setBrightness(7);
  updateDisplay();

  Serial.println("==================================");
  Serial.println(" ROOM OCCUPANCY COUNTER");
  Serial.println(" Maximum Capacity = 4");
  Serial.println("==================================");
  Serial.print("Current People: ");
  Serial.println(people);
}

void loop() {

  bool entryState = digitalRead(ENTRY_IR);
  bool exitState  = digitalRead(EXIT_IR);

  // ---------- ENTRY ----------
  if (lastEntry == HIGH && entryState == LOW) {

    Serial.println("Entry Sensor Triggered");

    if (people < MAX_PEOPLE) {

      people++;
      updateDisplay();

      Serial.print("Person Entered");
      Serial.print(" | Total People = ");
      Serial.println(people);

    } else {

      Serial.println("ROOM FULL!");
      Serial.println("Entry Denied");

      digitalWrite(BUZZER, HIGH);
      delay(2000);
      digitalWrite(BUZZER, LOW);

      updateDisplay();
    }
  }

  // ---------- EXIT ----------
  if (lastExit == HIGH && exitState == LOW) {

    Serial.println("Exit Sensor Triggered");

    if (people > 0) {
      people--;
    }

    updateDisplay();

    Serial.print("Person Exited");
    Serial.print(" | Total People = ");
    Serial.println(people);
  }

  // Save previous states
  lastEntry = entryState;
  lastExit = exitState;

  // Wait until sensors are released (prevents multiple counts)
  while (digitalRead(ENTRY_IR) == LOW || digitalRead(EXIT_IR) == LOW) {
    delay(10);
  }

  delay(50);
}
