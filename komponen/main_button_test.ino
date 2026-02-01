/*
 * Main button test script for ESP32 Keyless System
 * Tests the main button according to your wiring guide
 */

// Define pin according to your wiring guide
const int MAIN_BUTTON_PIN = 13;  // Main button pin 1 connected to GPIO 13

void setup() {
  // Initialize button pin as input with internal pull-up resistor
  pinMode(MAIN_BUTTON_PIN, INPUT_PULLUP);
  
  Serial.begin(115200);
  Serial.println("ESP32 Main Button Test Starting...");
  Serial.println("Press the main button to test functionality.");
}

void loop() {
  // Read the state of the main button
  int buttonState = digitalRead(MAIN_BUTTON_PIN);
  
  // Check if button is pressed (LOW because of INPUT_PULLUP)
  if(buttonState == LOW) {
    Serial.println("Main button PRESSED");
    
    // Debounce delay to prevent multiple readings
    delay(50);
    
    // Wait until button is released
    while(digitalRead(MAIN_BUTTON_PIN) == LOW) {
      // Wait for button release
    }
    
    Serial.println("Main button RELEASED");
  }
  
  // Small delay to reduce CPU usage
  delay(10);
}