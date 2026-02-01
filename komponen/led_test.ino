// Comprehensive LED test script for ESP32 Keyless System
// Tests all 8 indicator LEDs according to your wiring guide

// Define all LED pins according to your wiring guide
const int ledPins[] = {4, 5, 18, 19, 21, 22, 23, 25}; // LED1-LED8
const int numLeds = 8;

// LED labels for reference
String ledLabels[] = {"Ready", "BT Connected", "WiFi Connected", "Kontak ON",
                      "Mesin ON", "Starter LED", "Disconnect Alert", "Alarm"};

void setup() {
  // Initialize all LED pins as outputs according to wiring guide
  for(int i = 0; i < numLeds; i++) {
    pinMode(ledPins[i], OUTPUT);
    digitalWrite(ledPins[i], LOW); // Ensure all LEDs start OFF
  }

  Serial.begin(115200);
  Serial.println("ESP32 LED Test Starting...");
}

void loop() {
  // Test each LED individually
  for(int i = 0; i < numLeds; i++) {
    Serial.print("Testing LED");
    Serial.print(i+1);
    Serial.print(" (");
    Serial.print(ledLabels[i]);
    Serial.print(") on GPIO ");
    Serial.println(ledPins[i]);

    // Turn ON current LED
    digitalWrite(ledPins[i], HIGH);

    // Wait for 500ms to see the LED lit
    delay(500);

    // Turn OFF current LED
    digitalWrite(ledPins[i], LOW);

    // Wait before next LED
    delay(100);
  }

  // Small pause before repeating the sequence
  delay(1000);
}