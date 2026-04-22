#include <WebServer.h>
#include <WiFi.h>
#include "esp_camera.h"

// ─── WiFi Configuration ─────────────────────────────────
const char* ssid     = "YOUR_SSID";
const char* password = "YOUR_PASSWORD";

// ─── Camera Pin Map (AI-Thinker ESP32-CAM) ──────────────
#define PWDN_GPIO_NUM     32
#define RESET_GPIO_NUM    -1
#define XCLK_GPIO_NUM      0
#define SIOD_GPIO_NUM     26
#define SIOC_GPIO_NUM     27
#define Y9_GPIO_NUM       35
#define Y8_GPIO_NUM       34
#define Y7_GPIO_NUM       39
#define Y6_GPIO_NUM       36
#define Y5_GPIO_NUM       21
#define Y4_GPIO_NUM       19
#define Y3_GPIO_NUM       18
#define Y2_GPIO_NUM        5
#define VSYNC_GPIO_NUM    25
#define HREF_GPIO_NUM     23
#define PCLK_GPIO_NUM     22

// ─── Motor Pins (L298N Driver) ──────────────────────────
#define IN1 12
#define IN2 13
#define IN3 15
#define IN4 14

// ─── Web Servers ────────────────────────────────────────
WebServer controlServer(80);
WebServer streamServer(81);

// ════════════════════════════════════════════════════════
//  CAMERA INITIALIZATION
// ════════════════════════════════════════════════════════
void startCamera() {
  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer   = LEDC_TIMER_0;
  config.pin_d0       = Y2_GPIO_NUM;
  config.pin_d1       = Y3_GPIO_NUM;
  config.pin_d2       = Y4_GPIO_NUM;
  config.pin_d3       = Y5_GPIO_NUM;
  config.pin_d4       = Y6_GPIO_NUM;
  config.pin_d5       = Y7_GPIO_NUM;
  config.pin_d6       = Y8_GPIO_NUM;
  config.pin_d7       = Y9_GPIO_NUM;
  config.pin_xclk     = XCLK_GPIO_NUM;
  config.pin_pclk     = PCLK_GPIO_NUM;
  config.pin_vsync    = VSYNC_GPIO_NUM;
  config.pin_href     = HREF_GPIO_NUM;
  config.pin_sscb_sda = SIOD_GPIO_NUM;
  config.pin_sscb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn     = PWDN_GPIO_NUM;
  config.pin_reset    = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG;

  // Adjust image quality based on PSRAM availability
  if (psramFound()) {
    config.frame_size   = FRAMESIZE_VGA;   // 640x480
    config.jpeg_quality = 10;              // 0=best, 63=worst
    config.fb_count     = 2;
  } else {
    config.frame_size   = FRAMESIZE_QVGA;  // 320x240
    config.jpeg_quality = 20;
    config.fb_count     = 1;
  }

  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init FAILED: 0x%x\n", err);
    return;
  }
  Serial.println("Camera ready ✓");
}

// ════════════════════════════════════════════════════════
//  MOTOR CONTROL FUNCTIONS
// ════════════════════════════════════════════════════════
void stopMotors()    { digitalWrite(IN1,0); digitalWrite(IN2,0); digitalWrite(IN3,0); digitalWrite(IN4,0); }
void moveForward()   { digitalWrite(IN1,1); digitalWrite(IN2,0); digitalWrite(IN3,1); digitalWrite(IN4,0); }
void moveBackward()  { digitalWrite(IN1,0); digitalWrite(IN2,1); digitalWrite(IN3,0); digitalWrite(IN4,1); }
void turnLeft()      { digitalWrite(IN1,0); digitalWrite(IN2,1); digitalWrite(IN3,1); digitalWrite(IN4,0); }
void turnRight()     { digitalWrite(IN1,1); digitalWrite(IN2,0); digitalWrite(IN3,0); digitalWrite(IN4,1); }

// ════════════════════════════════════════════════════════
//  CONTROL WEB PAGE (HTML)
// ════════════════════════════════════════════════════════
void handleRoot() {
  // Get local IP as string to embed in HTML
  String ip = WiFi.localIP().toString();

  String html = R"rawhtml(
<!DOCTYPE html>
<html>
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <title>Robot Car</title>
  <style>
    * { box-sizing: border-box; margin: 0; padding: 0; }
    body {
      background: #0f0f0f; color: #fff;
      font-family: 'Segoe UI', sans-serif;
      display: flex; flex-direction: column;
      align-items: center; padding: 16px; gap: 16px;
    }
    h2 { color: #00d4ff; letter-spacing: 2px; }
    #stream {
      width: 100%; max-width: 640px;
      border: 2px solid #00d4ff;
      border-radius: 8px;
    }
    .controls {
      display: grid;
      grid-template-columns: repeat(3, 80px);
      grid-template-rows: repeat(3, 80px);
      gap: 8px;
    }
    .btn {
      background: #1a1a2e; color: #00d4ff;
      border: 2px solid #00d4ff; border-radius: 8px;
      font-size: 28px; cursor: pointer;
      transition: background .15s;
      user-select: none; -webkit-user-select: none;
    }
    .btn:active, .btn.pressed { background: #00d4ff; color: #000; }
    .center { grid-column: 2; grid-row: 2; }
    #status { color: #888; font-size: 13px; }
  </style>
</head>
<body>
  <h2>🤖 ROBOT CAR</h2>
  <img id="stream" src="http://)rawhtml" + ip + R"rawhtml(:81/stream" alt="Camera Stream">
  <div class="controls">
    <div></div>
    <button class="btn" id="F" ontouchstart="cmd('F')" ontouchend="cmd('S')"
            onmousedown="cmd('F')" onmouseup="cmd('S')">▲</button>
    <div></div>

    <button class="btn" id="L" ontouchstart="cmd('L')" ontouchend="cmd('S')"
            onmousedown="cmd('L')" onmouseup="cmd('S')">◀</button>
    <button class="btn center" id="S" onclick="cmd('S')">⏹</button>
    <button class="btn" id="R" ontouchstart="cmd('R')" ontouchend="cmd('S')"
            onmousedown="cmd('R')" onmouseup="cmd('S')">▶</button>

    <div></div>
    <button class="btn" id="B" ontouchstart="cmd('B')" ontouchend="cmd('S')"
            onmousedown="cmd('B')" onmouseup="cmd('S')">▼</button>
    <div></div>
  </div>
  <p id="status">Ready</p>

  <script>
    // Keyboard control mapping
    const keyMap = { ArrowUp:'F', ArrowDown:'B', ArrowLeft:'L', ArrowRight:'R', ' ':'S' };
    document.addEventListener('keydown', e => { if(keyMap[e.key]) { e.preventDefault(); cmd(keyMap[e.key]); } });
    document.addEventListener('keyup',  e => { if(['ArrowUp','ArrowDown','ArrowLeft','ArrowRight'].includes(e.key)) cmd('S'); });

    function cmd(action) {
      fetch('/control?action=' + action)
        .then(r => r.text())
        .then(t => document.getElementById('status').innerText = t)
        .catch(() => document.getElementById('status').innerText = 'Connection lost!');
    }
  </script>
</body>
</html>
)rawhtml";

  controlServer.send(200, "text/html", html);
}

// Handle incoming control commands
void handleControl() {
  if (!controlServer.hasArg("action")) {
    controlServer.send(400, "text/plain", "Missing action");
    return;
  }
  String action = controlServer.arg("action");

  if      (action == "F") { moveForward();  controlServer.send(200, "text/plain", "Moving Forward"); }
  else if (action == "B") { moveBackward(); controlServer.send(200, "text/plain", "Moving Backward"); }
  else if (action == "L") { turnLeft();     controlServer.send(200, "text/plain", "Turning Left"); }
  else if (action == "R") { turnRight();    controlServer.send(200, "text/plain", "Turning Right"); }
  else if (action == "S") { stopMotors();   controlServer.send(200, "text/plain", "Stopped"); }
  else                    {                 controlServer.send(400, "text/plain", "Unknown action"); }
}

// ════════════════════════════════════════════════════════
//  MJPEG STREAM (Runs on Core 0 to avoid motor interference)
// ════════════════════════════════════════════════════════
void handleStream() {
  WiFiClient client = streamServer.client();

  // Send MJPEG headers
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: multipart/x-mixed-replace; boundary=frame");
  client.println("Access-Control-Allow-Origin: *");
  client.println();

  while (client.connected()) {
    camera_fb_t* fb = esp_camera_fb_get();
    if (!fb) { Serial.println("Frame capture failed"); break; }

    client.printf("--frame\r\nContent-Type: image/jpeg\r\nContent-Length: %u\r\n\r\n", fb->len);
    client.write(fb->buf, fb->len);
    client.print("\r\n");

    esp_camera_fb_return(fb);
    delay(30); // ~33 FPS max, adjust based on network
  }
}

// Streaming task pinned to Core 0
void streamTask(void* param) {
  streamServer.on("/stream", handleStream);
  streamServer.begin();
  for (;;) {
    streamServer.handleClient();
    vTaskDelay(1); // Yield to RTOS
  }
}

// ════════════════════════════════════════════════════════
//  SETUP & MAIN LOOP
// ════════════════════════════════════════════════════════
void setup() {
  Serial.begin(115200);

  // Initialize motor pins
  pinMode(IN1, OUTPUT); pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT); pinMode(IN4, OUTPUT);
  stopMotors();

  // Connect to WiFi
  WiFi.begin(ssid, password);
  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED) { delay(500); Serial.print("."); }
  Serial.println("\nWiFi connected!");
  Serial.print("Control : http://"); Serial.println(WiFi.localIP());
  Serial.print("Stream  : http://"); Serial.print(WiFi.localIP()); Serial.println(":81/stream");

  startCamera();

  // Control server on Core 1 (main loop)
  controlServer.on("/", handleRoot);
  controlServer.on("/control", handleControl);
  controlServer.begin();

  // Start stream task on Core 0
  xTaskCreatePinnedToCore(streamTask, "streamTask", 8192, NULL, 1, NULL, 0);
}

void loop() {
  controlServer.handleClient();
}
