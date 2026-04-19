#include <Servo.h>

// ── Pin Definitions ──────────────────────────────────────
const int VRx_PIN   = A0;
const int SW_PIN    = 2;
const int SERVO_PIN = 3;

// ── Constants ────────────────────────────────────────────
const int CENTER_ANGLE    = 90;
const int DEAD_ZONE       = 60;
const int JOYSTICK_CENTER = 512;
const int ANGLE_THRESHOLD = 3;
const int NUM_SAMPLES     = 8;
const int UPDATE_INTERVAL = 20; // ms between servo updates

// ── Variables ────────────────────────────────────────────
Servo myServo;
int lastAngle         = CENTER_ANGLE;
unsigned long lastUpdate = 0; // Tracks last time servo was updated

// ── Smoothing: Fast reads, no delay ──────────────────────
int smoothRead(int pin) {
  long total = 0;
  for (int i = 0; i < NUM_SAMPLES; i++) {
    total += analogRead(pin); // No delay — ADC is fast enough
  }
  return total / NUM_SAMPLES;
}

void setup() {
  Serial.begin(9600);
  pinMode(SW_PIN, INPUT_PULLUP);
  myServo.attach(SERVO_PIN);
  myServo.write(CENTER_ANGLE);
  Serial.println("Joystick Servo — Stable Version");
  Serial.println("--------------------------------");
}

void loop() {

  // ── Button check — always responsive ─────────────────
  int buttonState = digitalRead(SW_PIN);
  if (buttonState == LOW) {
    myServo.write(CENTER_ANGLE);
    lastAngle = CENTER_ANGLE;
    Serial.println(">> Reset to 90 degrees");
    delay(300); // Only delay here — debounce is acceptable
    return;
  }

  // ── Non-blocking timing ───────────────────────────────
  unsigned long now = millis();
  if (now - lastUpdate < UPDATE_INTERVAL) {
    return; // Not time to update yet — exit cleanly
  }
  lastUpdate = now;

  // ── Read and process ──────────────────────────────────
  int xValue = smoothRead(VRx_PIN);

  // Dead zone check
  if (abs(xValue - JOYSTICK_CENTER) < DEAD_ZONE) {
    Serial.print("X: "); Serial.print(xValue);
    Serial.println("  >> Dead zone");
    return;
  }

  // Map to angle
  int targetAngle = map(xValue, 0, 1023, 0, 180);
  targetAngle = constrain(targetAngle, 0, 180);

  // Threshold filter
  if (abs(targetAngle - lastAngle) >= ANGLE_THRESHOLD) {
    myServo.write(targetAngle);
    lastAngle = targetAngle;
  }

  // ── Serial Output ─────────────────────────────────────
  Serial.print("X: ");      Serial.print(xValue);
  Serial.print("  Angle: "); Serial.print(lastAngle);
  Serial.println(" deg");
}