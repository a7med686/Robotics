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

### ESP32-CAM And FTDI Wire Connections Diagram: 

<img width="900" height="600" alt="image" src="https://github.com/user-attachments/assets/f0b48141-a5ef-4948-9309-027f20018b4c" />

| ESP32 | FTDI |
|------|------|
| 5V | VCC |
| GND | GND |
| U0R | TX |
| U0T | RX |
| IO0 ↔ GND |

---

### ESP32-CAM ↔ Motor Driver

<img width="900" height="528" alt="image" src="https://github.com/user-attachments/assets/d4574ff0-b0b8-4bce-97a2-7c1c1c43f2ec" />


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
1. Install ESP32 Board Support in Arduino IDE
    - Go to File > Preferences → Paste these URLs in “Additional Board URLs”:
  
       1. https://dl.espressif.com/dl/package_esp32_index.json
       2. https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
   
    - Go to Tools > Board > Board Manager → Search “ESP32” → Install
  2. Select:
     - Board: AI Thinker ESP32-CAM
     - Partition Scheme: Default 4MB with SPIFFS
     - PSRAM: Enabled (if available)

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
- The ESP32-CAM hosts a video stream at:http://< 10.167.98.199 >:81/stream
- It also hosts a control UI at: http://< 10.167.98.199 > with motor buttons
- Clicking the buttons sends HTTP requests (/control?move=F, etc.)
- ESP32 processes these to control motors via GPIO 12–15

---

## 📱 Usage
1. Upload code using FTDI
2. Power the car (ESP32 and motor driver)
3. Open Serial Monitor at 115200 → Get IP
4. Open browser on phone or PC:
     - Stream page: http://< 10.167.98.199>:81/stream
     - Control page: http://< 10.167.98.199>
- Add screenshots of:
  - Serial monitor showing IP
  - Web UI on phone
  - Camera stream output

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
- Add obstacle avoidance using ultrasonic sensors
- Add voice command control via browser
- Record stream to SD card or capture photos
- Turn this into a security patrol robot

---

## 🏁 Conclusion
Conclusion
This surveillance car is a fun and powerful IoT project that combines video
streaming, robotics, and web technology — all running on a single ESP32-
CAM!t
