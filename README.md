# 🚗 ESP32-CAM Surveillance Car with Live Video Streaming

## 📌 Overview
This project shows how to build a Wi-Fi controlled surveillance car using the ESP32-CAM module.
It streams live video and can be controlled remotely from any browser.

---

## ✨ Features
• Stream live via Wi-Fi
•  Web-based motor control interface (Forward, Backward, Left, Right,Stop)
• Built with ESP32-CAM (low-cost, compact module)
•  Uses L298N motor driver
• Control and view from any device on the same network

---

# 🧰 Components Required

| Component | Quantity|
|-----------|---------|
| ESP32-CAM (AI Thinker)         |  1  |
| FTDI USB-to-Serial Adapter     |  1  |
| L298N Motor Driver             |  1  |
| DC Motors + Wheels             |  4  | 
| Chassis + Battery Holder       |  1  |
| 18650 Battery / 7.4V LiPo      |  1  |
| Jumper Wires                   | Several |

---

## 🔌 Wiring Connections

### ESP32-CAM ↔ FTDI

| ESP32 | FTDI |
|------|------|
| 5V | VCC |
| GND | GND |
| U0R | TX |
| U0T | RX |
|100 ↔ GND|

---

### ESP32-CAM ↔ Motor Driver

| ESP32 | L298N |
|------|------|
| GPIO12 | IN1 |
| GPIO13 | IN2 |
| GPIO15 | IN3 |
| GPIO14 | IN4 |
|   5V   | 5 V |
|   GND  | GND |

---

### Power

• ESP32-CAM powered via FTDI (for programming) and 5V regulator or battery
• Motor Driver powered by separate battery (6–12V)

---

## ⚙️ Software Setup

### Install ESP32 Board

1. Go to File > Preferences → Paste these URLs in “Additional Board URLs”:
  • https://dl.espressif.com/dl/package_esp32_index.json
  • https://raw.githubusercontent.com/espressif/arduino- esp32/gh-pages/package_esp32_index.json
2. Go to Tools > Board > Board Manager → Search “ESP32” → Install

### Board Settings

• Board: AI Thinker ESP32-CAM
• Partition Scheme: Default 4MB with SPIFFS
• PSRAM: Enabled (if available)

---

## 💻 Code

```cpp
#define IN1 12
#define IN2 13
#define IN3 15
#define IN4 14
```

---

## 🌐 How It Works
• The ESP32-CAM hosts a video stream at:
➢ http:// 10.167.98.199:81/stream
• It also hosts a control UI at:
➢ http://< 10.167.98.199> with motor buttons
• Clicking the buttons sends HTTP requests (/control?move=F, etc.)
• ESP32 processes these to control motors via GPIO 12–15


---

## 📱 Usage

1. Upload code using FTDI
2. Power the car (ESP32 and motor driver)
3. Open Serial Monitor at 115200 → Get IP
4. Open browser on phone or PC:
   • Stream page: http://< 10.167.98.199>:81/stream
   • Control page: http://< 10.167.98.199>
Add screenshots of:
   • Serial monitor showing IP
   • Web UI on phone
   • Camera stream output

---

