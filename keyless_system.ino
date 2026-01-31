#include <WiFi.h>
#include <WebServer.h>
#include "BluetoothSerial.h"

// Cek apakah Bluetooth tersedia
#ifdef CONFIG_BT_ENABLED
BluetoothSerial SerialBT;
#else
// Placeholder untuk kompatibilitas
class DummyBluetooth {
  public:
    bool hasClient() { return false; }
    void begin(const char* name) {}
    bool available() { return false; }
    int read() { return 0; }
};
DummyBluetooth SerialBT;
#endif

// Definisi Pin sesuai wiring guide kamu
#define LED_ESP_READY 4    // LED 1 - ESP Ready
#define LED_BT_CONN 5      // LED 2 - BT Connected
#define LED_WIFI_CONN 18   // LED 3 - WiFi Connected to Phone
#define LED_KONTAK 19      // LED 4 - Kontak ON
#define LED_MESIN 21       // LED 5 - Mesin ON
#define LED_STARTER 22     // LED 6 - Starter
#define LED_DISCONNECT 23  // LED 7 - Disconnect Alert
#define LED_ALARM 25       // LED 8 - Alarm/Error
#define BTN_MAIN 13        // Button utama
#define BUZZER_PIN 15      // Buzzer

// Variabel status
bool isBTConnected = false;
bool isWiFiConnected = false;
bool isKontakOn = false;
bool isMesinOn = false;
bool isStarterActive = false;
bool isDisconnectedAlert = false;
bool isConnected = false;

// WiFi credentials
const char* targetSSID = "STARLINK";
const char* targetPassword = "12345678910";

// Variabel untuk deteksi double click
unsigned long lastPressTime = 0;
int pressCount = 0;
const unsigned long debounceDelay = 300;

// Web Server
WebServer server(80);

// Fungsi untuk logging yang lebih bersih
void logMessage(String message) {
  Serial.println("[LOG] " + message);
}

void updateLEDs() {
  // Update status LED sesuai dengan wiring guide:
  // Karena menggunakan LED bar common anode (VCC ke 3.3V):
  // - Saat GPIO LOW, LED menyala
  // - Saat GPIO HIGH, LED mati
  
  digitalWrite(LED_ESP_READY, LOW);           // Selalu menyala setelah setup
  digitalWrite(LED_BT_CONN, isBTConnected ? LOW : HIGH);    // BT Connected
  digitalWrite(LED_WIFI_CONN, isWiFiConnected ? LOW : HIGH); // WiFi Connected
  digitalWrite(LED_KONTAK, isKontakOn ? LOW : HIGH);       // Kontak ON
  digitalWrite(LED_MESIN, isMesinOn ? LOW : HIGH);         // Mesin ON
  digitalWrite(LED_STARTER, isStarterActive ? LOW : HIGH); // Starter
  digitalWrite(LED_DISCONNECT, isDisconnectedAlert ? LOW : HIGH); // Disconnect Alert
  digitalWrite(LED_ALARM, LOW); // Alarm (aktif saat HIGH)
}

// Fungsi untuk buzzer
void beep(int duration) {
  digitalWrite(BUZZER_PIN, HIGH);
  delay(duration);
  digitalWrite(BUZZER_PIN, LOW);
}

// Handler untuk web interface
void handleRoot() {
  String html = "<!DOCTYPE html><html>";
  html += "<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">";
  html += "<title>Keyless System Control</title>";
  html += "<style>";
  html += "body { font-family: Arial; margin: 0; padding: 20px; background-color: #f0f0f0; }";
  html += ".container { max-width: 600px; margin: 0 auto; }";
  html += ".card { background: white; border-radius: 10px; padding: 20px; margin: 10px 0; box-shadow: 0 2px 5px rgba(0,0,0,0.1); }";
  html += ".btn { display: inline-block; padding: 12px 24px; margin: 5px; background-color: #4CAF50; color: white; text-decoration: none; border-radius: 5px; border: none; cursor: pointer; font-size: 16px; }";
  html += ".btn-red { background-color: #f44336; }";
  html += ".btn-blue { background-color: #2196F3; }";
  html += ".status { padding: 10px; margin: 5px 0; border-radius: 5px; background-color: #e0e0e0; }";
  html += "</style></head><body>";
  html += "<div class='container'>";
  html += "<h1>Keyless System Control</h1>";

  // Status Cards
  html += "<div class='card'>";
  html += "<h3>Status Sistem</h3>";
  html += "<div class='status'>WiFi: " + String(isWiFiConnected ? "TERHUBUNG" : "TIDAK TERHUBUNG") + "</div>";
  html += "<div class='status'>Bluetooth: " + String(isBTConnected ? "TERHUBUNG" : "TIDAK TERHUBUNG") + "</div>";
  html += "<div class='status'>Kontak: " + String(isKontakOn ? "AKTIF" : "NON-AKTIF") + "</div>";
  html += "<div class='status'>Mesin: " + String(isMesinOn ? "NYALA" : "MATI") + "</div>";
  html += "<div class='status'>Koneksi: " + String(isConnected ? "TERSEDIA" : "TIDAK TERSEDIA") + "</div>";
  html += "</div>";

  // Control Buttons
  html += "<div class='card'>";
  html += "<h3>Kontrol</h3>";
  if (isConnected) {
    html += "<a class='btn btn-blue' href='/kontak_toggle'>Toggle Kontak</a><br>";
    if (isKontakOn) {
      html += "<a class='btn btn-blue' href='/mesin_toggle'>Toggle Mesin</a><br>";
      html += "<a class='btn' href='/starter'>Starter</a><br>";
    }
    html += "<a class='btn btn-red' href='/lock'>Lock Sistem</a><br>";
  } else {
    html += "<a class='btn' href='/unlock'>Unlock Sistem</a><br>";
  }
  html += "<a class='btn' href='/beep'>Beep Buzzer</a><br>";
  html += "</div>";

  // Info
  html += "<div class='card'>";
  html += "<h3>Info</h3>";
  html += "<p>IP Address: " + WiFi.localIP().toString() + "</p>";
  html += "<p>Uptime: " + String(millis()/1000) + " detik</p>";
  html += "</div>";

  html += "</div></body></html>";
  server.send(200, "text/html", html);
}

void handleUnlock() {
  if (!isConnected) {
    isConnected = true;
    logMessage("Sistem dibuka via web");
    beep(100);
  }
  server.sendHeader("Location", "/");
  server.send(303);
}

void handleLock() {
  if (isConnected) {
    isConnected = false;
    isKontakOn = false;
    isMesinOn = false;
    logMessage("Sistem dikunci via web");
    beep(300);
  }
  server.sendHeader("Location", "/");
  server.send(303);
}

void handleKontakToggle() {
  if (isConnected) {
    isKontakOn = !isKontakOn;
    if (!isKontakOn) {
      isMesinOn = false; // Matikan mesin jika kontak dimatikan
    }
    logMessage("Kontak " + String(isKontakOn ? "DI" : "DIMATIKAN"));
    beep(100);
  }
  server.sendHeader("Location", "/");
  server.send(303);
}

void handleMesinToggle() {
  if (isConnected && isKontakOn) {
    isMesinOn = !isMesinOn;
    logMessage("Mesin " + String(isMesinOn ? "DI" : "DIMATIKAN"));
    beep(150);
  }
  server.sendHeader("Location", "/");
  server.send(303);
}

void handleStarter() {
  if (isConnected && isKontakOn && !isStarterActive) {
    isStarterActive = true;
    logMessage("Starter diaktifkan");
    beep(200);
    delay(500);
    isStarterActive = false;
    logMessage("Starter dinonaktifkan");
    beep(100);
  }
  server.sendHeader("Location", "/");
  server.send(303);
}

void handleBeep() {
  beep(200);
  logMessage("Buzzer diaktifkan via web");
  server.sendHeader("Location", "/");
  server.send(303);
}

void setup() {
  // Setup pin mode
  pinMode(LED_ESP_READY, OUTPUT);
  pinMode(LED_BT_CONN, OUTPUT);
  pinMode(LED_WIFI_CONN, OUTPUT);
  pinMode(LED_KONTAK, OUTPUT);
  pinMode(LED_MESIN, OUTPUT);
  pinMode(LED_STARTER, OUTPUT);
  pinMode(LED_DISCONNECT, OUTPUT);
  pinMode(LED_ALARM, OUTPUT);
  pinMode(BTN_MAIN, INPUT_PULLUP);
  pinMode(BUZZER_PIN, OUTPUT);

  // Matikan semua LED dan buzzer di awal
  digitalWrite(LED_ESP_READY, HIGH);  // Matikan dulu
  digitalWrite(LED_BT_CONN, HIGH);
  digitalWrite(LED_WIFI_CONN, HIGH);
  digitalWrite(LED_KONTAK, HIGH);
  digitalWrite(LED_MESIN, HIGH);
  digitalWrite(LED_STARTER, HIGH);
  digitalWrite(LED_DISCONNECT, HIGH);
  digitalWrite(LED_ALARM, HIGH);
  digitalWrite(BUZZER_PIN, LOW);

  Serial.begin(115200);
  logMessage("Sistem dimulai");
  
  // Setup WiFi Client
  WiFi.begin(targetSSID, targetPassword);
  logMessage("Mencoba konek ke WiFi: " + String(targetSSID));
  
  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 30) {
    delay(500);
    Serial.print(".");
    attempts++;
  }
  
  if (WiFi.status() == WL_CONNECTED) {
    isWiFiConnected = true;
    isConnected = true;
    logMessage("Terhubung ke WiFi!");
    logMessage("IP Address: " + WiFi.localIP().toString());
  } else {
    logMessage("Gagal konek ke WiFi");
  }

  // Setup Bluetooth jika tersedia
  #ifdef CONFIG_BT_ENABLED
  SerialBT.begin("KeylessESP32");
  logMessage("Bluetooth siap");
  #else
  logMessage("Bluetooth tidak tersedia");
  #endif

  // Setup Web Server
  server.on("/", handleRoot);
  server.on("/unlock", handleUnlock);
  server.on("/lock", handleLock);
  server.on("/kontak_toggle", handleKontakToggle);
  server.on("/mesin_toggle", handleMesinToggle);
  server.on("/starter", handleStarter);
  server.on("/beep", handleBeep);
  server.begin();
  logMessage("Web server siap di port 80");

  // Update status awal
  updateLEDs();
}

void loop() {
  // Handle Web Server
  server.handleClient();

  // Cek koneksi WiFi (reconnect jika putus)
  if (WiFi.status() == WL_CONNECTED) {
    if (!isWiFiConnected) {
      isWiFiConnected = true;
      logMessage("WiFi terhubung kembali");
    }
  } else {
    if (isWiFiConnected) {
      isWiFiConnected = false;
      if (!isBTConnected) {
        isConnected = false;
      }
      logMessage("WiFi terputus");
    }
    
    // Coba konek ulang
    if (millis() % 10000 < 100) { // Coba setiap 10 detik
      logMessage("Mencoba konek ulang ke WiFi...");
      WiFi.begin(targetSSID, targetPassword);
    }
  }

  // Cek koneksi Bluetooth jika tersedia
  #ifdef CONFIG_BT_ENABLED
  if (SerialBT.hasClient()) {
    if (!isBTConnected) {
      isBTConnected = true;
      isConnected = true;
      logMessage("Bluetooth terhubung");
    }
  } else {
    if (isBTConnected) {
      isBTConnected = false;
      if (!isWiFiConnected) {
        isConnected = false;
      }
      logMessage("Bluetooth terputus");
    }
  }
  #endif

  // Deteksi tombol ditekan
  if (!digitalRead(BTN_MAIN)) {
    delay(50); // Debounce
    if (!digitalRead(BTN_MAIN)) {
      unsigned long currentTime = millis();
      
      if (currentTime - lastPressTime > debounceDelay) {
        pressCount = 1;
        lastPressTime = currentTime;
      } else {
        pressCount++;
        
        if (pressCount >= 2) {
          if (isKontakOn && isConnected) {
            isMesinOn = !isMesinOn;
            logMessage("Mesin " + String(isMesinOn ? "DI" : "DIMATIKAN") + " via tombol");
            beep(150);
          }
          pressCount = 0;
        }
      }
      
      updateLEDs();
    }
  }

  // Single press: aktifkan kontak (jika terkoneksi)
  if (pressCount == 1) {
    unsigned long currentTime = millis();
    if (currentTime - lastPressTime > 300) {
      if (isConnected) {
        isKontakOn = !isKontakOn;
        if (!isKontakOn) {
          isMesinOn = false;
        }
        logMessage("Kontak " + String(isKontakOn ? "DI" : "DIMATIKAN") + " via tombol");
        beep(100);
        updateLEDs();
      }
      pressCount = 0;
    }
  }

  // Cek disconnect alert
  if ((isKontakOn || isMesinOn) && !isBTConnected && !isWiFiConnected && isConnected) {
    if (!isDisconnectedAlert) {
      isDisconnectedAlert = true;
      logMessage("PERINGATAN: Sistem aktif tapi tidak terhubung!");
      // Beep peringatan
      for (int i = 0; i < 3; i++) {
        beep(100);
        delay(100);
      }
    }
  } else {
    if (isDisconnectedAlert) {
      isDisconnectedAlert = false;
    }
  }

  updateLEDs();
  delay(50);
}