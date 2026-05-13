# 💧 Smart Water Purifier System

An IoT-based smart water purification and monitoring system built using Arduino, ESP8266, and FastAPI. The system continuously monitors water quality in real time using TDS and turbidity sensors, automatically decides whether filtration is required, and publishes live sensor data to a web dashboard over WiFi.

---

## ✨ Features

- Real-time water quality monitoring
- Automatic filtration control
- TDS and turbidity analysis
- OLED live sensor display
- WiFi-based data transmission
- FastAPI-powered dashboard
- CSV data logging
- Remote monitoring on local network

---

## 🏗️ System Architecture

The project is divided into three main components:

### Arduino Controller (`ArduinoCode`)
Handles:
- Sensor readings
- Motor control
- OLED display output
- Serial communication with ESP8266

### ESP8266 WiFi Module (`ESP8266Code`)
Responsible for:
- WiFi connectivity
- Receiving UART data
- Sending HTTP requests to the server

### Web Server & Dashboard (`WebServer`)
Built using FastAPI to:
- Receive sensor data
- Log readings into CSV
- Serve the monitoring dashboard
- Provide downloadable logs

---

## ⚙️ Working Principle

1. The ultrasonic sensor checks the water level.
2. If the measured distance is less than `19 cm`, water is detected.
3. TDS and turbidity values are read and displayed on the OLED screen.
4. The Arduino sends sensor data to the ESP8266 through serial communication.
5. Based on the readings:
   - If `TDS > 500 ppm`, a **Danger** alert is shown.
   - If `Turbidity > 75 NTU`, water is sent through filtration using **Motor A**.
   - Otherwise, water bypasses filtration using **Motor B**.
6. The ESP8266 sends the data to the FastAPI server over WiFi.
7. The server logs the readings and updates the dashboard in real time.

---

## 🔌 Hardware Requirements

- Arduino Uno
- ESP8266 NodeMCU
- TDS Sensor
- Turbidity Sensor
- HC-SR04 Ultrasonic Sensor
- OLED Display (128x32, I2C)
- 2 × DC Motors
- L298N Motor Driver
- Breadboard and jumper wires

---

## 📍 Pin Connections

| Component | Pin |
|---|---|
| TDS Sensor | A1 |
| Turbidity Sensor | A0 |
| Ultrasonic Trigger | 12 |
| Ultrasonic Echo | 13 |
| OLED SDA | A4 |
| OLED SCL | A5 |
| Motor A IN1 | 8 |
| Motor A IN2 | 9 |
| Motor B IN1 | 10 |
| Motor B IN2 | 11 |
| ESP8266 RX | 2 |
| ESP8266 TX | 3 |

---

## 📁 Project Structure

```text
SmartWaterPurifier/
│
├── ArduinoCode/
├── ESP8266Code/
├── WebServer/
└── README.md
```

---

## 🛠️ Software & Libraries

### Arduino
- Adafruit_GFX
- Adafruit_SSD1306
- SoftwareSerial
- Wire

### ESP8266
- ESP8266WiFi
- ESP8266HTTPClient
- SoftwareSerial

### Python
- fastapi
- uvicorn

---

# 🚀 Setup and Installation

## Arduino Setup

1. Install the required Arduino libraries.
2. Open:

```text
ArduinoCode/ArduinoPart/ArduinoPart.ino
```

3. Select the correct board and COM port.
4. Upload the code to the Arduino.

---

## ESP8266 Setup

1. Open:

```text
ESP8266Code/ESP8266Side/ESP8266Side.ino
```

2. Update WiFi credentials and server IP:

```cpp
const char* ssid = "YOUR_WIFI_SSID";
const char* password = "YOUR_WIFI_PASSWORD";
const char* serverport = "http://SERVER_IP_ADDRESS:8000/data";
```

3. Select the correct board and upload the code.

---

## 🌐 Web Server Setup

### Clone Repository

```bash
git clone https://github.com/O-tech477/SmartWaterPurifier.git
cd SmartWaterPurifier/WebServer
```

### Create Virtual Environment

```bash
python -m venv .venv
```

### Activate Environment

#### Linux/macOS

```bash
source .venv/bin/activate
```

#### Windows

```powershell
.venv\Scripts\activate
```

### Install Dependencies

```bash
pip install uv
uv sync
```

Or:

```bash
pip install fastapi uvicorn
```

### Run Server

```bash
uvicorn main:app --host 0.0.0.0 --port 8000
```

---

## 🌐 Dashboard Access

Open:

```text
http://<your-server-ip>:8000
```

The dashboard displays:
- Live TDS values
- Turbidity readings
- Water level status
- Recent logs

---

## 🔗 API Endpoints

| Endpoint | Method | Description |
|---|---|---|
| `/` | GET | Dashboard |
| `/data` | POST | Receive sensor data |
| `/logs` | GET | Fetch logs |
| `/download` | GET | Download CSV logs |

---

## 📦 Example JSON Payload

```json
{
  "tds": 420,
  "turbidity": 61,
  "water_level": 12
}
```

---

## 🔮 Future Improvements

- MQTT support
- Mobile app integration
- Cloud database storage
- Email/SMS alerts
- AI-based water quality analysis
- Battery backup support

---

## 📸 Dashboard Preview

Add screenshots of:
- Hardware setup
- OLED display
- Dashboard UI
- Circuit diagram

Example:

```md
![Dashboard](images/dashboard.png)
```

---

## 📜 License

This project is open-source and available under the MIT License.

---

## 👨‍💻 Author

Developed as an IoT-based Smart Water Purification and Monitoring System using Arduino, ESP8266, and FastAPI.
