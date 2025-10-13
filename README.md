
# VU MIF ROBOTICS HOMEWORK NR. 2

## MAIN GOAL
This project demonstrates a **Reaction Time Tester** built with an **Arduino Uno**. The device measures how quickly a user can respond to a visual cue (LED light).When the LED turns on after a random delay, the user must press the button as fast as possible — the Arduino then calculates and displays the reaction time through the Serial Monitor. The best result is saved in **EEPROM** memory, ensuring the record is preserved even after power-off.

---

## FEATURES
- Measures human reaction time in milliseconds.
- Random delay between signal activation (1–5 seconds).
- Stores best score permanently using EEPROM.
- Uses **hardware interrupt** for fast and precise button detection.
- Serial Monitor provides all interaction and feedback messages.
- Simple LED + button interface for intuitive use.

---

## COMPONENTS

| Component | Quantity | Function |
|------------|-----------|-----------|
| **Arduino Uno R3** | 1 | Main microcontroller unit |
| **LED (any color)** | 1 | Visual “Go!” signal indicator |
| **Resistor (220Ω)** | 1 | Limits current for the LED |
| **Push Button** | 1 | Used to start the test and record the user’s reaction |
| **Breadboard** | 1 | Circuit prototyping base |


---

## PHOTO

<img width="1107" height="795" alt="Screenshot 2025-10-13 at 23 10 19" src="https://github.com/user-attachments/assets/5ee26fbe-4900-4881-972b-021681c343a8" />

---

## SYSTEM OVERVIEW
1. When the device starts, the Serial Monitor displays the title **“Reaction Time Tester”** and loads the previously saved best time from EEPROM. 
2. The user presses the button to start the test.  
3. A random delay between **1 and 5 seconds** is generated.  
4. Once the LED turns ON, the timer starts.  
5. When the user presses the button, the timer stops and displays the **reaction time in milliseconds**.  
6. If the result is better than the previous best, it updates the record stored in EEPROM.  
7. The process can be repeated indefinitely.

---

## ARDUINO CODE HIGHLIGHTS
- Uses **interrupts (attachInterrupt)** for immediate button response.  
- Implements **finite state machine (FSM)** for smooth transitions between game states:  
  `WAIT_START → WAIT_RANDOM_DELAY → LED_ON_MEASURE → RESULT_SHOW`.  
- EEPROM functions (`EEPROM.get()` / `EEPROM.put()`) store and load the best time.  
- Debouncing logic ensures stable button reading with `millis()`.  
- No `delay()` used — program runs fully non-blocking.

---

## CIRCUIT OVERVIEW

| Arduino Pin | Connection | Description |
|--------------|-------------|--------------|
| **13** | LED (via 220Ω resistor) | LED turns ON to signal reaction start |
| **2** | Push Button | Trigger input using internal pull-up resistor |
| **GND** | Common Ground | Shared between all components |

---

### CIRCUIT DIAGRAM (TINKERCAD SCHEMATIC)



## DEMO VIDEO
