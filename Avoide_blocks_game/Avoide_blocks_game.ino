/*
  Project: ESP32 Avoid Block Game
  Created by: ProtoSam IoT
  GitHub: https://github.com/ProtoSamIoT
  YouTube: https://www.youtube.com/@protosamiot

  Fast-paced avoid block game using ESP32, Joystick module, and OLED display.
  Dodge incoming blocks and survive as long as possible.
  
  Star ⭐ the repo and subscribe for more ESP32 & IoT projects.
*/
#include <Arduino.h>
#include <U8g2lib.h>

/* ---------- OLED ---------- */
U8G2_SH1106_128X64_NONAME_F_HW_I2C oled(U8G2_R0, U8X8_PIN_NONE);

/* ---------- JOYSTICK ---------- */
#define JOY_X 34
#define JOY_Y 35
#define JOY_SW 32

/* ---------- BUZZER ---------- */
#define BUZZER 25

/* ---------- GAME STATES ---------- */
#define GAME_START 0
#define GAME_PLAY  1
#define GAME_OVER  2

int gameState = GAME_START;

/* ---------- PLAYER ---------- */
int playerX = 60;           // initial horizontal position
int playerWidth = 10;
int playerHeight = 6;
int moveSpeed = 3;          // joystick movement speed

/* ---------- OBSTACLES ---------- */
struct Obstacle {
  int x;
  int y;
};
#define MAX_OBS 3
Obstacle obstacles[MAX_OBS];

/* ---------- GAME VARS ---------- */
int score = 0;
int lastBeep = 0;
unsigned long lastFrame;

/* ---------- SETUP ---------- */
void setup() {
  pinMode(JOY_SW, INPUT_PULLUP);
  pinMode(BUZZER, OUTPUT);

  oled.begin();
  oled.setFont(u8g2_font_6x10_tf);

  randomSeed(analogRead(0));

  resetGame();
}

/* ---------- LOOP ---------- */
void loop() {
  unsigned long now = millis();
  if (now - lastFrame < 50) return; // frame rate control
  lastFrame = now;

  int joyX = analogRead(JOY_X);
  bool btn = digitalRead(JOY_SW) == LOW;

  if (gameState == GAME_START) {
    drawStart();
    if (btn) gameState = GAME_PLAY;
  }
  else if (gameState == GAME_PLAY) {
    movePlayer(joyX);
    moveObstacles();
    checkCollision();
    drawGame();
  }
  else if (gameState == GAME_OVER) {
    drawGameOver();
    if (btn) resetGame();
  }
}

/* ---------- GAME FUNCTIONS ---------- */
void movePlayer(int joyX) {
  if (joyX < 1200) playerX -= moveSpeed; // move left
  else if (joyX > 3000) playerX += moveSpeed; // move right

  // constrain within screen
  if (playerX < 0) playerX = 0;
  if (playerX > 128 - playerWidth) playerX = 128 - playerWidth;
}

void moveObstacles() {
  for (int i = 0; i < MAX_OBS; i++) {
    obstacles[i].y += 3; // falling speed
    if (obstacles[i].y > 64) {
      obstacles[i].y = 0;
      obstacles[i].x = random(0, 128 - 10);
      score++; // increase score for each avoided obstacle
      tone(BUZZER, 700, 50);
    }
  }
}

void checkCollision() {
  for (int i = 0; i < MAX_OBS; i++) {
    if (obstacles[i].y + 6 >= 58 && obstacles[i].y <= 58 + playerHeight &&
        obstacles[i].x + 10 >= playerX && obstacles[i].x <= playerX + playerWidth) {
      gameState = GAME_OVER;
      tone(BUZZER, 125, 200);
      delay(150);
      tone(BUZZER, 125, 200);
    }
  }
}

/* ---------- DRAW ---------- */
void drawGame() {
  oled.clearBuffer();

  // Player block
  oled.drawBox(playerX, 58, playerWidth, playerHeight);

  // Obstacles
  for (int i = 0; i < MAX_OBS; i++) {
    oled.drawBox(obstacles[i].x, obstacles[i].y, 10, 6);
  }

  // Score
  oled.setCursor(90, 10);
  oled.print("S:");
  oled.print(score);

  oled.sendBuffer();
}

void drawStart() {
  oled.clearBuffer();
  oled.drawStr(20, 25, "Avoid the Blocks");
  oled.drawStr(10, 45, "Move joystick to start");
  oled.sendBuffer();
}

void drawGameOver() {
  oled.clearBuffer();
  oled.drawStr(30, 25, "GAME OVER");
  oled.setCursor(35, 45);
  oled.print("Score:");
  oled.print(score);
  oled.sendBuffer();
}

void resetGame() {
  gameState = GAME_START;
  score = 0;
  playerX = 60;
  for (int i = 0; i < MAX_OBS; i++) {
    obstacles[i].x = random(0, 118);
    obstacles[i].y = random(-50, 0);
  }
}