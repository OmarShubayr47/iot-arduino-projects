#include <Servo.h>

// ── Pin Definitions ──────────────────────────────────────
const int POT_PIN   = A0;
const int SW_PIN    = 2;
const int SERVO_PIN = 3;

// ── Constants ────────────────────────────────────────────
const int CENTER_ANGLE    = 90;
const int ANGLE_THRESHOLD = 2;   // Ignore changes smaller than 2 degrees
const int NUM_SAMPLES     = 5;   // Readings to average for smoothing

// ── Variables ────────────────────────────────────────────
Servo myServo;
int servoAngle = CENTER_ANGLE;
int lastAngle  = CENTER_ANGLE;

// ── Smoothing Function ───────────────────────────────────
int smoothRead(int pin) {
  long total = 0;
  for (int i = 0; i < NUM_SAMPLES; i++) {
    total += analogRead(pin);
    delay(2);
  }
  return total / NUM_SAMPLES;
}

void setup() {
  Serial.begin(9600);
  pinMode(SW_PIN, INPUT_PULLUP);
  myServo.attach(SERVO_PIN);
  myServo.write(CENTER_ANGLE);
  Serial.println("Potentiometer Servo Control Ready");
  Serial.println("---------------------------------");
}

void loop() {

  // ── Read Smoothed Potentiometer Value ───────────────────
  int potValue = smoothRead(POT_PIN);
  int buttonState = digitalRead(SW_PIN);

  // ── Button: Reset to Center ─────────────────────────────
  if (buttonState == LOW) {
    servoAngle = CENTER_ANGLE;
    myServo.write(servoAngle);
    lastAngle = servoAngle;
    Serial.println(">> Button Pressed: Servo reset to 90 degrees");
    delay(300);
    return;
  }

  // ── Map Potentiometer to Servo Angle ────────────────────
  int targetAngle = map(potValue, 0, 1023, 0, 180);
  targetAngle = constrain(targetAngle, 0, 180);

  // ── Threshold Filter ────────────────────────────────────
  // Only move servo if change is meaningful
  if (abs(targetAngle - lastAngle) >= ANGLE_THRESHOLD) {
    servoAngle = targetAngle;
  }

  // ── Update Servo Only When Angle Changed ────────────────
  if (servoAngle != lastAngle) {
    myServo.write(servoAngle);
    lastAngle = servoAngle;
  }

  // ── Serial Output ───────────────────────────────────────
  Serial.print("Pot Value: ");  Serial.print(potValue);
  Serial.print("  Angle: ");    Serial.print(servoAngle);
  Serial.println(" deg");

  delay(15);
}