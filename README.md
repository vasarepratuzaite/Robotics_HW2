# VU MIF ROBOTICS — HOMEWORK NR. 2

## MAIN GOAL
This project implements a **Reaction Time Tester** using an **Arduino Uno** microcontroller. The system measures how quickly a user reacts to a **visual stimulus (LED)** that appears after a random delay. When the LED turns on, the user presses a button as fast as possible — the Arduino calculates the reaction time and displays it on an **LCD screen**. The **best and average results** are saved in **EEPROM memory**, ensuring that the data is preserved even after power-off.

---

## FEATURES
- Measures human reaction time in milliseconds.  
- Random delay (1–5 seconds) before LED activation.  
- Stores **best** and **average** results in EEPROM with validation (magic byte).  
- Uses **external interrupt** for fast button detection and **timer interrupt** for periodic scheduling.  
- Fully **asynchronous, event-driven design** — no blocking `delay()` calls.  
- User feedback via **LCD display** and **LED indicator**.  
- Safe and low-voltage circuit using 220 Ω resistors.

---

## SYSTEM OVERVIEW
When powered on, the LCD shows “Reaction Tester” or “Press to start,” indicating a successful boot.  
After a random delay, the LED lights up, prompting the user to press the button quickly.  
The Arduino measures and displays the reaction time on the LCD.  
The LED turns off instantly upon button press, demonstrating **real-time interrupt response**.  
After rebooting, the LCD immediately shows the same “Best: xxxx ms” value — proving **EEPROM persistence** across power cycles.

---

## DEMO VIDEO
[Watch demonstration video](https://youtu.be/Eed4sBLrolo)

---

## PHOTO




## ARDUINO CODE HIGHLIGHTS
The Arduino code follows an **asynchronous, event-driven state machine** (`WAIT_START → WAIT_RANDOM → LED_ON → SHOW_RESULT`).  
It controls LED activation, button input, and LCD output concurrently using **non-blocking timing** with `millis()`.  
A **hardware timer (Timer1)** provides periodic flags for time-based scheduling, while **EEPROM** stores the best and average reaction times along with a validation byte.  
This design demonstrates **interrupt-driven behavior**, **real-time responsiveness**, and **persistent data storage**.

---

## HARDWARE COMPONENTS
| **Component** | **Quantity** | **Function** |
|----------------|--------------|---------------|
| **Arduino Uno R3 (U2)** | 1 | Main microcontroller |
| **220 Ω Resistor (R2, R3)** | 2 | Current limiting (LED + LCD) |
| **LCD 16×2 (U3)** | 1 | Displays reaction times and messages |
| **Pushbutton (S2)** | 1 | User input for reaction test |
| **Red LED (D2)** | 1 | Visual “Go!” signal indicator |

---

## TECHNICAL DETAILS
- **External interrupt:** Triggered by button press (`attachInterrupt()` or event flag on pin D7).  
- **Timer interrupt:** Generates periodic tasks for scheduling (`Timer1` or non-blocking `millis()` loop).  
- **EEPROM layout:**  
  - Address 0–3 → Best result  
  - Address 4–7 → Average result  
  - Address 8 → Validation byte (0xAB)  
- **Supply voltage:** 5 V (safe logic level).  
- **Resistors:** 220 Ω limit LED current.  

---

## DEMONSTRATION SUMMARY
1. Power on the Arduino — LCD displays *“Reaction Tester”*.  
2. Perform several reaction tests — LED flashes, user presses the button, and LCD shows *“Reaction Time: ... ms”*.  
3. The LED turns off immediately after pressing — demonstrating interrupt responsiveness.  
4. Power cycle the board — LCD shows the same *“Best: xxxx ms”* result, confirming EEPROM data persistence.  

---

## PHOTO
<img width="966" height="633" alt="Screenshot 2025-10-20 at 20 35 39" src="https://github.com/user-attachments/assets/13214795-9955-4c97-8b71-8c23538786f4" />
