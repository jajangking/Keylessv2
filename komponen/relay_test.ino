// Relay test script for ESP32 Keyless System
// Tests both relays according to your wiring guide

// Define relay pins according to your wiring guide
const int kontakRelayPin = 26;  // Relay CH1 (Kontak - ON/OFF Switch)
const int starterRelayPin = 27; // Relay CH2 (Starter - Momentary Switch)

void setup() {
  // Initialize relay pins as outputs according to wiring guide
  pinMode(kontakRelayPin, OUTPUT);
  pinMode(starterRelayPin, OUTPUT);
  
  // Initialize relays to OFF state (HIGH for active-low relays)
  // Assuming your relays are active LOW (common in many relay modules)
  digitalWrite(kontakRelayPin, HIGH);   // Turn OFF Kontak relay
  digitalWrite(starterRelayPin, HIGH);  // Turn OFF Starter relay
  
  Serial.begin(115200);
  Serial.println("ESP32 Relay Test Starting...");
}

void loop() {
  Serial.println("Testing Kontak Relay (CH1)...");
  
  // Activate Kontak relay
  digitalWrite(kontakRelayPin, LOW);    // Turn ON Kontak relay
  Serial.println("Kontak relay ON");
  delay(2000);                          // Keep ON for 2 seconds
  
  // Deactivate Kontak relay
  digitalWrite(kontakRelayPin, HIGH);   // Turn OFF Kontak relay
  Serial.println("Kontak relay OFF");
  delay(1000);                          // Wait 1 second
  
  Serial.println("Testing Starter Relay (CH2)...");
  
  // Activate Starter relay
  digitalWrite(starterRelayPin, LOW);   // Turn ON Starter relay
  Serial.println("Starter relay ON");
  delay(2000);                          // Keep ON for 2 seconds
  
  // Deactivate Starter relay
  digitalWrite(starterRelayPin, HIGH);  // Turn OFF Starter relay
  Serial.println("Starter relay OFF");
  delay(1000);                          // Wait 1 second
  
  Serial.println("Testing Both Relays Together...");
  
  // Activate both relays
  digitalWrite(kontakRelayPin, LOW);    // Turn ON Kontak relay
  digitalWrite(starterRelayPin, LOW);   // Turn ON Starter relay
  Serial.println("Both relays ON");
  delay(2000);                          // Keep ON for 2 seconds
  
  // Deactivate both relays
  digitalWrite(kontakRelayPin, HIGH);   // Turn OFF Kontak relay
  digitalWrite(starterRelayPin, HIGH);  // Turn OFF Starter relay
  Serial.println("Both relays OFF");
  delay(2000);                          // Wait 2 seconds before repeating
}