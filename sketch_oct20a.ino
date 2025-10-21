#include <LiquidCrystal.h>
#include <EEPROM.h>

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

const int ledPin = 10;
const int buttonPin = 7;

enum State { WAIT_START, WAIT_RANDOM, LED_ON, SHOW_RESULT };
State state = WAIT_START;

volatile bool timerFlag = false;     // nustatomas per Timer1 pertraukimą
unsigned long now = 0;               // laikas atnaujinamas kas 10 ms
unsigned long previousMillis = 0;
unsigned long randomDelayTime = 0;
unsigned long ledOnTime = 0;
unsigned long reactionTime = 0;
unsigned long lastButtonChange = 0;
bool buttonPressed = false;

unsigned long bestTime = 9999;
unsigned long avgTime = 0;
unsigned long trialCount = 0;

const byte RT_VALID = 0xA7;
int addrBest = 0;
int addrAvg = 4;
int addrCount = 8;
int addrValid = 12;

// debounce
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
  EEPROM.get(addrValid, check);
  if (check == RT_VALID) {
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
  EEPROM.put(addrValid, RT_VALID);
}

ISR(TIMER1_COMPA_vect) {
  timerFlag = true;  // pazymineme kad praejo 10ms
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

  noInterrupts();
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1  = 0;
  OCR1A = 15999;   
  TCCR1B |= (1 << WGM12);       
  TCCR1B |= (1 << CS12) | (1 << CS10); 
  TIMSK1 |= (1 << OCIE1A);      
  interrupts();
}

void loop() {
  if (timerFlag) {        //  kas 10 ms pakeliamas
    timerFlag = false;    // isvalom, kad laukti kito
    now = millis();       // atnaujinam laika milis

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
}
