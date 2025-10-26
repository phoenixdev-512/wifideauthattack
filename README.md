# Wi-Fi Security Monitor

![License](https://img.shields.io/badge/License-MIT-blue.svg)
![Platform](https://img.shields.io/badge/Platform-ESP32-important)
![Purpose](https://img.shields.io/badge/Purpose-Education%20%26%20Security-yellowgreen)

> **Educational tool for detecting Wi-Fi deauthentication attacks**

<p align="center">
  <img src="https://img.icons8.com/color/96/000000/wifi-lock.png" alt="Wi-Fi Security Icon"/>
</p>

## 📖 Overview

A passive monitoring tool for network administrators to detect deauthentication attacks in Wi-Fi networks. Designed for **educational purposes** and **authorized security testing**.

## ✨ Features

| Feature | Description |
|---------|-------------|
| 🔍 Attack Detection | Identifies deauthentication packets in real-time |
| 🚨 Alert System | Immediate notifications for suspicious activity |
| 📊 Logging | Timestamped records of security events |
| ⚖️ Ethical Design | Strictly for authorized educational use |

## ⚠️ Legal Disclaimer

> **Warning**
> 
> This tool is **only** for:
> - Educational use in controlled environments
> - Security testing **with explicit network ownership/permission**
>
> ❗ Unauthorized use may violate laws like the **Computer Fraud and Abuse Act**

## 🛠️ Hardware/Software Requirements

### Hardware
- ESP32 (or compatible Wi-Fi microcontroller)

### Software
- Arduino IDE ([Download](https://www.arduino.cc/en/software))
- ESP32 Board Package ([Install Guide](https://docs.espressif.com/projects/arduino-esp32/en/latest/installing.html))
- Libraries:
  ```arduino
  #include <WiFi.h>
  #include <esp_wifi.h>
  ```

## 🚀 Installation

1. **Clone the repository**
   ```bash
   git clone https://github.com/phoenixdev-512/wifideauthattack.git
   cd wifideauthattack
   ```

2. **Setup Arduino IDE**
   - Install ESP32 board support
   - Add required libraries (`WiFi.h` and `esp_wifi.h` are included with ESP32 package)

3. **Upload to ESP32**
   - Open `wifi_scanner212.ino` in Arduino IDE
   - Select your ESP32 board from Tools > Board
   - Select the correct COM port
   - Click Upload

4. **Monitor Output**
   - Open Serial Monitor at 115200 baud rate
   - Follow on-screen instructions to scan and select networks

## 📝 Usage Example

```
Scanning networks...

Nearby Wi-Fi Networks:
[0] SSID: MyNetwork | MAC: AA:BB:CC:DD:EE:FF | Signal: -45 dBm
[1] SSID: TestNetwork | MAC: 11:22:33:44:55:66 | Signal: -72 dBm

Enter the number of the Wi-Fi network to attack:
```

## 🌟 Future Roadmap

- [ ] Push notifications via Telegram/Email
- [ ] Automated MAC address whitelisting
- [ ] Statistical anomaly detection
- [ ] Web-based configuration interface
- [ ] Support for additional ESP32 variants

## 🤝 Contributing

Contributions are welcome! Please follow these steps:

1. Fork the project
2. Create your feature branch (`git checkout -b feature/AmazingFeature`)
3. Commit changes (`git commit -m 'Add amazing feature'`)
4. Push to the branch (`git push origin feature/AmazingFeature`)
5. Open a Pull Request

Please ensure your code follows the existing style and includes appropriate documentation.

## 📜 License

Distributed under the MIT License. See [LICENSE](LICENSE) file for details.

## 🔒 Security Notice

<p align="center">
  <em>⚠️ Use responsibly and only on networks you own or have explicit permission to test</em>
</p>

---

<p align="center">
  Made with ❤️ for cybersecurity education
</p>
