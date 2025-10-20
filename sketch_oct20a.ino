#include <LiquidCrystal.h>
#include <EEPROM.h>

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

const int ledPin = 10;
const int buttonPin = 7;

enum State { WAIT_START, WAIT_RANDOM, LED_ON, SHOW_RESULT };
State state = WAIT_START;

unsigned long previousMillis = 0;
unsigned long randomDelayTime = 0;
unsigned long ledOnTime = 0;
unsigned long reactionTime = 0;
unsigned long now = 0;
unsigned long lastButtonChange = 0;
bool buttonPressed = false;
bool ledState = false;
bool readyToPress = false;

unsigned long bestTime = 9999;
unsigned long avgTime = 0;
unsigned long trialCount = 0;
const byte MAGIC = 0xAB;
int addrBest = 0;
int addrAvg = 4;
int addrCount = 8;
int addrMagic = 12;

void checkButton() {
  static bool lastState = HIGH;
  bool reading = digitalRead(buttonPin);
  unsigned long t = millis();

  if (reading != lastState && (t - lastButtonChange > 50)) {
    lastButtonChange = t;
    if (reading == LOW) {
      buttonPressed = true;
    }
  }
  lastState = reading;
}

void loadEEPROM() {
  byte check;
  EEPROM.get(addrMagic, check);
  if (check == MAGIC) {
    EEPROM.get(addrBest, bestTime);
    EEPROM.get(addrAvg, avgTime);
    EEPROM.get(addrCount, trialCount);
  } else {
    bestTime = 9999;
    avgTime = 0;
    trialCount = 0;
  }
}

void saveEEPROM() {
  EEPROM.put(addrBest, bestTime);
  EEPROM.put(addrAvg, avgTime);
  EEPROM.put(addrCount, trialCount);
  EEPROM.put(addrMagic, MAGIC);
}

void setup() {
  pinMode(ledPin, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);

  loadEEPROM();
  randomSeed(analogRead(0));

  lcd.begin(16, 2);
  lcd.clear();
  lcd.print("Reaction Tester");
  delay(1500);
  lcd.clear();
  lcd.print("Best:");
  lcd.print(bestTime);
  lcd.print(" ms");
  lcd.setCursor(0, 1);
  lcd.print("Press to start");
}

void loop() {
  now = millis();
  checkButton();

  switch (state) {

    case WAIT_START:
      digitalWrite(ledPin, LOW);
      if (buttonPressed) {
        buttonPressed = false;
        lcd.clear();
        lcd.print("Get ready...");
        randomDelayTime = random(1000, 5000);
        previousMillis = now;
        state = WAIT_RANDOM;
      }
      break;

    case WAIT_RANDOM:
      if (buttonPressed) {
        buttonPressed = false;
        lcd.clear();
        lcd.print("Too early!");
        previousMillis = now;
        state = SHOW_RESULT;
      } else if (now - previousMillis >= randomDelayTime) {
        digitalWrite(ledPin, HIGH);
        ledOnTime = now;
        lcd.clear();
        lcd.print("LED ON!");
        lcd.setCursor(0, 1);
        lcd.print("Press NOW!");
        state = LED_ON;
      }
      break;

    case LED_ON:
      if (buttonPressed) {
        buttonPressed = false;
        digitalWrite(ledPin, LOW);
        reactionTime = now - ledOnTime;

        trialCount++;
        avgTime = (avgTime * (trialCount - 1) + reactionTime) / trialCount;
        if (reactionTime < bestTime) bestTime = reactionTime;
        saveEEPROM();

        lcd.clear();
        lcd.print("RT:");
        lcd.print(reactionTime);
        lcd.print(" ms");
        lcd.setCursor(0, 1);
        lcd.print("Best:");
        lcd.print(bestTime);
        lcd.print(" ms");

        previousMillis = now;
        state = SHOW_RESULT;
      }
      break;

    case SHOW_RESULT:
      if (now - previousMillis >= 3000) {
        lcd.clear();
        lcd.print("Best:");
        lcd.print(bestTime);
        lcd.print(" ms");
        lcd.setCursor(0, 1);
        lcd.print("Press to start");
        state = WAIT_START;
      }
      break;
  }
}
