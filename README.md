
# ESP32 Avoid Block Game

An interactive Avoid Block game developed using ESP32, joystick module, buzzer, and an OLED/TFT display. The player moves left and right to dodge falling blocks. The game ends when a block collides with the player.

---

## 🎮 Game Description

In this game, blocks continuously fall from the top of the display.  
The player controls a block at the bottom of the screen using a joystick.

### Controls:
- Move joystick **left** → Player moves left  
- Move joystick **right** → Player moves right  
- Avoid falling blocks  
- If a block hits the player, **Game Over**

A buzzer provides sound feedback during collision or game over events.

---

## ✨ Features
- Real-time joystick-based control
- Smooth falling block animation
- Collision detection
- Game-over indication
- Audio feedback using buzzer
- Simple and lightweight embedded game

---

## 🧩 Hardware Required
- ESP32 Development Board
- Joystick Module
- OLED / TFT Display (I2C)
- Buzzer
- Jumper Wires
- Breadboard

---

## 🔌 Pin Configuration

### 🎯 Joystick
```cpp
#define JOY_X   34
#define JOY_Y   35
#define JOY_SW  32

🔔 Buzzer

#define BUZZER 25

🖥️ Display (I2C)

Display Pin	ESP32 Pin
SDA	GPIO 21
SCL	GPIO 22

