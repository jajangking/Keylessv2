/*
 * Bluetooth and WiFi Connection Indicator for ESP32 Keyless System
 *
 * Features:
 * - Connects to WiFi hotspot (SSID: STARLINK, Password: 12345678910)
 * - LED Indicators:
 *   - LED1 (GPIO 4): Ready indicator
 *   - LED2 (GPIO 5): Bluetooth connection status
 *   - LED3 (GPIO 18): WiFi connection status
 */

#include <WiFi.h>
#include <BluetoothSerial.h>

// Define LED pins according to your wiring guide
const int LED_READY = 4;        // Ready indicator
const int LED_BT_STATUS = 5;    // Bluetooth connection status
const int LED_WIFI_STATUS = 18; // WiFi connection status

// WiFi credentials
const char* ssid = "STARLINK";
const char* password = "12345678910";

// Bluetooth serial object
BluetoothSerial SerialBT;

// Variables to store connection status
bool wifiConnected = false;
bool btConnected = false;

// Timer variables for blinking ready LED
unsigned long previousMillis = 0;
const long interval = 1000; // 1 second interval
bool ledState = false;

void setup() {
  // Initialize LED pins as outputs
  pinMode(LED_READY, OUTPUT);
  pinMode(LED_BT_STATUS, OUTPUT);
  pinMode(LED_WIFI_STATUS, OUTPUT);

  // Initialize LEDs to OFF state
  digitalWrite(LED_READY, LOW);
  digitalWrite(LED_BT_STATUS, LOW);
  digitalWrite(LED_WIFI_STATUS, LOW);

  Serial.begin(115200);
  Serial.println("ESP32 Bluetooth and WiFi Connection Indicator Starting...");

  // Initialize Bluetooth
  if (!SerialBT.begin("ESP32_Bluetooth")) {
    Serial.println("An error occurred initializing Bluetooth");
  } else {
    Serial.println("Bluetooth initialized successfully");
  }

  // Connect to WiFi
  connectToWiFi();

  Serial.println("System ready. Waiting for connections...");
}

void loop() {
  // Handle WiFi connection
  handleWiFiConnection();

  // Handle Bluetooth connection
  handleBluetoothConnection();

  // Blink the ready LED to indicate system is running
  blinkReadyLED();

  // Small delay to reduce CPU usage
  delay(10);
}

void connectToWiFi() {
  Serial.print("Connecting to WiFi: ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  // Wait for connection with timeout
  int attempts = 0;
  const int maxAttempts = 20; // 20 * 500ms = 10 seconds timeout

  while (WiFi.status() != WL_CONNECTED && attempts < maxAttempts) {
    delay(500);
    Serial.print(".");
    attempts++;
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("");
    Serial.println("WiFi connected successfully!");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
    wifiConnected = true;
  } else {
    Serial.println("");
    Serial.println("Failed to connect to WiFi");
    wifiConnected = false;
  }
}

void handleWiFiConnection() {
  // Check current WiFi status
  bool currentStatus = (WiFi.status() == WL_CONNECTED);

  if (currentStatus != wifiConnected) {
    wifiConnected = currentStatus;

    if (wifiConnected) {
      Serial.println("WiFi connected!");
      Serial.print("IP address: ");
      Serial.println(WiFi.localIP());
    } else {
      Serial.println("WiFi disconnected!");
      // Attempt to reconnect
      connectToWiFi();
    }
  }

  // Update WiFi status LED - LED ON when connected, OFF when not connected
  if (wifiConnected) {
    digitalWrite(LED_WIFI_STATUS, HIGH);  // Turn ON LED when WiFi is connected
  } else {
    digitalWrite(LED_WIFI_STATUS, LOW);   // Turn OFF LED when WiFi is not connected
  }
}

void handleBluetoothConnection() {
  // Check if there's a Bluetooth client connected
  bool currentStatus = SerialBT.hasClient();

  if (currentStatus != btConnected) {
    btConnected = currentStatus;

    if (btConnected) {
      Serial.println("Bluetooth client connected!");
    } else {
      Serial.println("Bluetooth client disconnected!");
    }
  }

  // Update Bluetooth status LED - LED ON when connected, OFF when not connected
  if (btConnected) {
    digitalWrite(LED_BT_STATUS, HIGH);    // Turn ON LED when Bluetooth is connected
  } else {
    digitalWrite(LED_BT_STATUS, LOW);     // Turn OFF LED when Bluetooth is not connected
  }
}

void blinkReadyLED() {
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    // Toggle LED state
    ledState = !ledState;
    digitalWrite(LED_READY, ledState);
  }
}

// Optional: Function to handle incoming Bluetooth data
void handleBluetoothData() {
  if (SerialBT.available()) {
    String receivedData = SerialBT.readString();
    receivedData.trim(); // Remove any trailing whitespace

    Serial.print("Received via Bluetooth: ");
    Serial.println(receivedData);

    // Echo back to Bluetooth client
    SerialBT.println("Echo: " + receivedData);
  }
}