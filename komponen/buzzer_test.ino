/*
 * Buzzer test script for ESP32 Keyless System
 * Tests the buzzer according to your wiring guide
 */

// Global variable definitions
const int BUZZER_PIN = 15;  // Buzzer + connected to GPIO 15

void setup() {
  // Initialize buzzer pin as output according to wiring guide
  pinMode(BUZZER_PIN, OUTPUT);
  
  Serial.begin(115200);
  Serial.println("ESP32 Buzzer Test Starting...");
}

void loop() {
  Serial.println("Testing buzzer - Short beep...");
  
  // Create a short beep
  digitalWrite(BUZZER_PIN, HIGH);  // Turn ON buzzer
  delay(200);                      // Beep for 200ms
  digitalWrite(BUZZER_PIN, LOW);   // Turn OFF buzzer
  delay(500);                      // Wait 500ms
  
  Serial.println("Testing buzzer - Medium beep...");
  
  // Create a medium beep
  digitalWrite(BUZZER_PIN, HIGH);  // Turn ON buzzer
  delay(500);                      // Beep for 500ms
  digitalWrite(BUZZER_PIN, LOW);   // Turn OFF buzzer
  delay(500);                      // Wait 500ms
  
  Serial.println("Testing buzzer - Long beep...");
  
  // Create a long beep
  digitalWrite(BUZZER_PIN, HIGH);  // Turn ON buzzer
  delay(1000);                     // Beep for 1 second
  digitalWrite(BUZZER_PIN, LOW);   // Turn OFF buzzer
  delay(1000);                     // Wait 1 second
  
  Serial.println("Testing buzzer - Beep sequence...");
  
  // Create a sequence of beeps
  for(int i = 0; i < 3; i++) {
    digitalWrite(BUZZER_PIN, HIGH);  // Turn ON buzzer
    delay(150);                      // Beep for 150ms
    digitalWrite(BUZZER_PIN, LOW);   // Turn OFF buzzer
    delay(150);                      // Wait 150ms
  }
  
  delay(2000);  // Wait 2 seconds before repeating the test sequence
}