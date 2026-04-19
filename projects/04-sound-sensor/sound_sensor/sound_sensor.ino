const int analogPin = A0;  // Analog output
const int digitalPin = 7;  // Digital output

void setup() {
  Serial.begin(9600);
  pinMode(digitalPin, INPUT);
  Serial.println("Sound Sensor Ready");
}

void loop() {
  int soundLevel = analogRead(analogPin);   // 0 to 1023
  int soundDetected = digitalRead(digitalPin); // HIGH or LOW

  Serial.print("Sound Level: ");
  Serial.print(soundLevel);

  if (soundDetected == LOW) {
    Serial.println("  >> Sound Detected!");
  } else {
    Serial.println("  >> Quiet");
  }

  delay(100);
}
