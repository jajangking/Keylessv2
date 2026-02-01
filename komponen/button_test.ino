/*
 * Button test script for ESP32 Keyless System
 * Tests the additional 4-pin button with LED according to your wiring guide
 */

// Define pins according to your updated wiring guide
const int ADDITIONAL_BUTTON_PIN = 14;  // Additional button pin 1 connected to GPIO 14
const int ADDITIONAL_LED_PIN = 12;     // Additional button LED anode connected to GPIO 12

void setup() {
  // Initialize button pin as input with internal pull-up resistor
  pinMode(ADDITIONAL_BUTTON_PIN, INPUT_PULLUP);
  
  // Initialize LED pin as output
  pinMode(ADDITIONAL_LED_PIN, OUTPUT);
  
  // Initialize LED to OFF state
  digitalWrite(ADDITIONAL_LED_PIN, LOW);

  Serial.begin(115200);
  Serial.println("ESP32 Additional Button with LED Test Starting...");
  Serial.println("Press the 4-pin button to test functionality.");
}

void loop() {
  // Read the state of the additional button
  int buttonState = digitalRead(ADDITIONAL_BUTTON_PIN);
  
  // Check if button is pressed (LOW because of INPUT_PULLUP)
  if(buttonState == LOW) {
    // Turn ON the LED when button is pressed
    digitalWrite(ADDITIONAL_LED_PIN, HIGH);
    
    Serial.println("Additional button PRESSED - LED ON");
    
    // Debounce delay to prevent multiple readings
    delay(50);
    
    // Wait until button is released
    while(digitalRead(ADDITIONAL_BUTTON_PIN) == LOW) {
      // Wait for button release
    }
    
    Serial.println("Additional button RELEASED");
  } else {
    // Turn OFF the LED when button is not pressed
    digitalWrite(ADDITIONAL_LED_PIN, LOW);
  }
  
  // Small delay to reduce CPU usage
  delay(10);
}