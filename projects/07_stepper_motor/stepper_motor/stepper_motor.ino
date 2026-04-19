#include <Stepper.h>

// ── Motor Configuration ───────────────────────────────────
const int STEPS_PER_REV = 2048;  // 28BYJ-48 full revolution
Stepper myStepper(STEPS_PER_REV, 8, 10, 9, 11); // IN1,IN3,IN2,IN4 order

// ── Pin Definitions ───────────────────────────────────────
const int POT_PIN       = A0;
const int JOY_PIN       = A1;
const int MODE_BTN      = 2;
const int FWD_BTN       = 3;
const int BWD_BTN       = 4;

// ── Constants ─────────────────────────────────────────────
const int STEPS_PER_PRESS = 512;   // Quarter turn per button press
const int DEAD_ZONE       = 50;    // Joystick dead zone
const int JOY_CENTER      = 512;
const int MIN_SPEED       = 5;     // RPM minimum
const int MAX_SPEED       = 15;    // RPM maximum (28BYJ-48 limit)

// ── Mode Tracking ─────────────────────────────────────────
int currentMode   = 1;   // 1=Potentiometer, 2=Joystick, 3=Button steps
unsigned long lastModePress  = 0;
unsigned long lastFwdPress   = 0;
unsigned long lastBwdPress   = 0;
const int DEBOUNCE = 300;

// ── Print mode name ───────────────────────────────────────
void printMode() {
  Serial.print(">> Mode ");
  Serial.print(currentMode);
  Serial.print(": ");
  if (currentMode == 1) Serial.println("Potentiometer Speed Control");
  if (currentMode == 2) Serial.println("Joystick Direction Control");
  if (currentMode == 3) Serial.println("Button Step Control");
  Serial.println("------------------------------");
}

void setup() {
  Serial.begin(9600);
  pinMode(MODE_BTN, INPUT_PULLUP);
  pinMode(FWD_BTN,  INPUT_PULLUP);
  pinMode(BWD_BTN,  INPUT_PULLUP);
  myStepper.setSpeed(10); // Default 10 RPM
  Serial.println("Stepper Motor Control — 3 Modes");
  Serial.println("================================");
  printMode();
}

void loop() {
  unsigned long now = millis();

  // ── Mode Button: Cycle through 1 → 2 → 3 → 1 ──────────
  if (digitalRead(MODE_BTN) == LOW && now - lastModePress > DEBOUNCE) {
    lastModePress = now;
    currentMode++;
    if (currentMode > 3) currentMode = 1;
    printMode();
    delay(300);
    return;
  }

  // ════════════════════════════════════════════════════════
  // MODE 1 — Potentiometer controls speed and direction
  // Left half (0-511)  = rotate backward, speed increases left
  // Right half (512-1023) = rotate forward, speed increases right
  // ════════════════════════════════════════════════════════
  if (currentMode == 1) {
    int potValue = analogRead(POT_PIN);

    if (potValue < 462) {
      // Left of center — rotate backward
      int speed = map(potValue, 461, 0, MIN_SPEED, MAX_SPEED);
      myStepper.setSpeed(speed);
      myStepper.step(-4);
      Serial.print("POT: "); Serial.print(potValue);
      Serial.print("  Speed: "); Serial.print(speed);
      Serial.println(" RPM  BWD");

    } else if (potValue > 562) {
      // Right of center — rotate forward
      int speed = map(potValue, 563, 1023, MIN_SPEED, MAX_SPEED);
      myStepper.setSpeed(speed);
      myStepper.step(4);
      Serial.print("POT: "); Serial.print(potValue);
      Serial.print("  Speed: "); Serial.print(speed);
      Serial.println(" RPM  FWD");

    } else {
      // Center dead zone — stop
      Serial.println("POT: Center — Stopped");
      delay(100);
    }
  }

  // ════════════════════════════════════════════════════════
  // MODE 2 — Joystick X axis controls direction and speed
  // Push right = forward, push left = backward
  // Further from center = faster
  // ════════════════════════════════════════════════════════
  else if (currentMode == 2) {
    int joyValue = analogRead(JOY_PIN);

    if (abs(joyValue - JOY_CENTER) < DEAD_ZONE) {
      Serial.println("JOY: Dead zone — Stopped");
      delay(50);

    } else if (joyValue > JOY_CENTER + DEAD_ZONE) {
      // Push right — forward
      int speed = map(joyValue, JOY_CENTER + DEAD_ZONE, 1023, MIN_SPEED, MAX_SPEED);
      myStepper.setSpeed(speed);
      myStepper.step(4);
      Serial.print("JOY: "); Serial.print(joyValue);
      Serial.print("  Speed: "); Serial.print(speed);
      Serial.println(" RPM  FWD");

    } else {
      // Push left — backward
      int speed = map(joyValue, JOY_CENTER - DEAD_ZONE, 0, MIN_SPEED, MAX_SPEED);
      myStepper.setSpeed(speed);
      myStepper.step(-4);
      Serial.print("JOY: "); Serial.print(joyValue);
      Serial.print("  Speed: "); Serial.print(speed);
      Serial.println(" RPM  BWD");
    }
  }

  // ════════════════════════════════════════════════════════
  // MODE 3 — Button steps
  // FWD button = 512 steps forward (quarter turn)
  // BWD button = 512 steps backward (quarter turn)
  // ════════════════════════════════════════════════════════
  else if (currentMode == 3) {

    if (digitalRead(FWD_BTN) == LOW && now - lastFwdPress > DEBOUNCE) {
      lastFwdPress = now;
      myStepper.setSpeed(10);
      Serial.println(">> Forward 512 steps (quarter turn)");
      myStepper.step(STEPS_PER_PRESS);
      Serial.println(">> Done");
    }

    if (digitalRead(BWD_BTN) == LOW && now - lastBwdPress > DEBOUNCE) {
      lastBwdPress = now;
      myStepper.setSpeed(10);
      Serial.println(">> Backward 512 steps (quarter turn)");
      myStepper.step(-STEPS_PER_PRESS);
      Serial.println(">> Done");
    }
  }
}