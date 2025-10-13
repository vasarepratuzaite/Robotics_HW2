#include <EEPROM.h>

const int LED_PIN = 13;
const int BUTTON_PIN = 2;

volatile bool buttonPressed = false;
unsigned long ledOnTime = 0;
unsigned long reactionTime = 0;
unsigned long bestTime = 9999;
unsigned long randomDelay = 0;

enum State { WAIT_START, WAIT_RANDOM_DELAY, LED_ON_MEASURE, RESULT_SHOW };
State state = WAIT_START;

unsigned long stateStart = 0;
unsigned long resultShowUntil = 0;

void setup() {
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), handleButtonPress, FALLING);

  Serial.begin(9600);

  unsigned long storedBest;
  EEPROM.get(0, storedBest);
  if (storedBest == 0xFFFFFFFF || storedBest == 0) {
    bestTime = 9999;
  } else {
    bestTime = storedBest;
  }

  Serial.println("Reaction Time Tester");
  Serial.print("Best result: ");
  Serial.print(bestTime);
  Serial.println(" ms");
  Serial.println("Press button to start.");
}

void loop() {
  unsigned long now = millis();

  switch (state) {
    case WAIT_START:
      digitalWrite(LED_PIN, LOW);
      if (buttonPressed) {
        buttonPressed = false;
        randomDelay = random(1000, 5000);
        stateStart = now;
        Serial.println("Get ready...");
        state = WAIT_RANDOM_DELAY;
      }
      break;

    case WAIT_RANDOM_DELAY:
      if (now - stateStart >= randomDelay) {
        digitalWrite(LED_PIN, HIGH);
        ledOnTime = now;
        Serial.println("Now!");
        state = LED_ON_MEASURE;
      }
      if (buttonPressed) { 
        buttonPressed = false;
        Serial.println("Too early! Try again.");
        state = WAIT_START;
      }
      break;

    case LED_ON_MEASURE:
      if (buttonPressed) {
        buttonPressed = false;
        digitalWrite(LED_PIN, LOW);
        reactionTime = now - ledOnTime;
        Serial.print("Reaction time: ");
        Serial.print(reactionTime);
        Serial.println(" ms");

        if (reactionTime < bestTime) {
          bestTime = reactionTime;
          EEPROM.put(0, bestTime);
          Serial.println("New record!");
        } else {
          Serial.print("Best: ");
          Serial.print(bestTime);
          Serial.println(" ms");
        }

        resultShowUntil = now + 2000;
        state = RESULT_SHOW;
      }
      break;

    case RESULT_SHOW:
      if (now >= resultShowUntil) {
        Serial.println("Press button to start again.");
        state = WAIT_START;
      }
      break;
  }
}

void handleButtonPress() {
  static unsigned long lastPress = 0;
  unsigned long now = millis();
  if (now - lastPress > 200) {
    buttonPressed = true;
    lastPress = now;
  }
}
