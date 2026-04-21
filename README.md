# 🚗 ESP32-CAM Surveillance Car with Live Video Streaming

## 📌 Overview
- In this project, you’ll learn how to build a Wi-Fi-controlled surveillance
car using the ESP32-CAM module.
- It streams live and can be controlled remotely from any smartphone or
computer browser — no apps required!

---

## ✨ Features
- Stream live via Wi-Fi
- Web-based motor control interface (Forward, Backward, Left, Right, Stop)
- Built with ESP32-CAM (low-cost, compact module)
- Uses L298N motor driver
- Control and view from any device on the same network

---

## 🧰 Components Required

| Component | Quantity |
|----------|--------|
| ESP32-CAM (AI Thinker) | 1 |
| FTDI USB-to-Serial Adapter | 1 |
| L298N Motor Driver | 1 |
| DC Motors + Wheels | 4 |
| Chassis + Battery Holder | 1 |
| 18650 Battery / 7.4V LiPo | 1 |
| Jumper Wires | Several |

---

## 🔌 Wiring Connections

### ESP32-CAM ↔ FTDI

| ESP32 | FTDI |
|------|------|
| 5V | VCC |
| GND | GND |
| U0R | TX |
| U0T | RX |
| IO0 ↔ GND |

---

### ESP32-CAM ↔ Motor Driver

| ESP32 | L298N |
|------|------|
| GPIO12 | IN1 |
| GPIO13 | IN2 |
| GPIO15 | IN3 |
| GPIO14 | IN4 |
| 5V | 5V |
| GND | GND |

---

### Power
- ESP32-CAM powered via FTDI (for programming) and 5V regulator or battery
- Motor Driver powered by separate battery (6–12V)

---

## ⚙️ Software Setup

### Install ESP32 Board
Add this URL in Arduino IDE:

https://dl.espressif.com/dl/package_esp32_index.json

Then install ESP32 from Board Manager.

### Board Settings
- Board: AI Thinker ESP32-CAM
- Partition Scheme: Default
- PSRAM: Enabled

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
- ESP32 hosts:
  - Stream → http://IP:81/stream
  - Control → http://IP
- Browser buttons send commands to ESP32
- ESP32 controls motors using GPIO pins

---

## 📱 Usage
1. Upload code using FTDI
2. Power the car
3. Open Serial Monitor (115200)
4. Get IP address
5. Open browser

---

## ⚠️ Troubleshooting

| Problem | Fix |
|--------|-----|
| Motors not stopping | Check wiring |
| Motors not moving | Check ENA/ENB |
| ESP restarts | Fix power |
| Camera error | Change format |

---

## 🚀 Improvements
- Add PWM speed control
- Add ultrasonic sensor
- Voice control
- Save images to SD card

---

## 🏁 Conclusion
This project combines robotics, streaming, and IoT using ESP32-CAM.

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
