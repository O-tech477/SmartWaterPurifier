# Smart Water Purifier System

A smart IoT-based water purification and monitoring system built using an Arduino, ESP8266, and FastAPI. The system monitors water quality in real time using TDS and turbidity sensors, automatically decides whether filtration is required, and publishes live sensor data to a web dashboard over WiFi.

---

## Features

- Real-time water quality monitoring
- Automatic water filtration decision-making
- TDS and turbidity analysis
- Water level detection using ultrasonic sensing
- OLED display for live sensor readings
- WiFi-based data transmission using ESP8266
- FastAPI-powered web dashboard
- CSV-based data logging
- Live monitoring from any device on the local network

---

## System Architecture

The project is divided into three main components:

### 1. Arduino Controller (`ArduinoCode`)
The main control unit responsible for:
- Reading all sensor values
- Driving motors
- Displaying data on the OLED screen
- Sending sensor data to the ESP8266 module

### 2. ESP8266 WiFi Module (`ESP8266Code`)
Acts as a communication bridge between the Arduino and the web server by:
- Connecting to WiFi
- Receiving UART data from the Arduino
- Sending HTTP POST requests to the FastAPI server

### 3. Web Server & Dashboard (`WebServer`)
A Python backend built using FastAPI that:
- Receives incoming sensor data
- Logs readings into a CSV file
- Hosts a live monitoring dashboard
- Provides downloadable sensor logs

---

## Working Principle

1. The Arduino continuously monitors the water level using the ultrasonic sensor.
2. If the measured distance is less than `19 cm`, water is considered present.
3. The system reads:
   - TDS (Total Dissolved Solids)
   - Turbidity
4. Sensor values are displayed on the OLED screen.
5. The Arduino sends the readings to the ESP8266 through serial communication.
6. Based on the readings:
   - If `TDS > 500 ppm`, a **Danger** alert is displayed.
   - If `Turbidity > 75 NTU`, water is redirected through the filtration path using **Motor A**.
   - If water quality is acceptable, water bypasses filtration using **Motor B**.
7. The ESP8266 transmits the data to the FastAPI server over WiFi.
8. The server logs the readings into `logs.csv`.
9. The web dashboard displays real-time sensor data and recent readings.

---

## Hardware Requirements

- Arduino Uno (or compatible board)
- ESP8266 NodeMCU
- TDS Sensor
- Turbidity Sensor
- Ultrasonic Sensor (HC-SR04)
- OLED Display (128x32, I2C)
- 2 × DC Motors
- L298N Motor Driver Module
- Breadboard and jumper wires

---

## Pin Connections

### Arduino Connections

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

## Project Structure

```text
SmartWaterPurifier/
│
├── ArduinoCode/
│   └── ArduinoPart.ino
│
├── ESP8266Code/
│   └── ESP8266Side.ino
│
├── WebServer/
│   ├── main.py
│   ├── logs.csv
│   └── templates/
│
└── README.md
```

---

## Software & Libraries

### Arduino Libraries

- Adafruit_GFX
- Adafruit_SSD1306
- SoftwareSerial
- Wire

### ESP8266 Libraries

- ESP8266WiFi
- ESP8266HTTPClient
- SoftwareSerial

### Python Dependencies

- fastapi
- uvicorn

---

# Setup and Installation

## 1. Arduino Controller Setup

1. Install the required Arduino libraries using the Library Manager:
   - Adafruit GFX
   - Adafruit SSD1306

2. Connect the Arduino board to your computer.

3. Open:

```text
ArduinoCode/ArduinoPart/ArduinoPart.ino
```

4. Select:
   - Correct board
   - Correct COM port

5. Upload the code to the Arduino.

---

## 2. ESP8266 Setup

1. Configure the Arduino IDE for ESP8266 development.

2. Open:

```text
ESP8266Code/ESP8266Side/ESP8266Side.ino
```

3. Update the WiFi credentials and server address:

```cpp
const char* ssid = "YOUR_WIFI_SSID";
const char* password = "YOUR_WIFI_PASSWORD";
const char* serverport = "http://SERVER_IP_ADDRESS:8000/data";
```

4. Select:
   - NodeMCU 1.0 (ESP-12E Module)
   - Correct COM port

5. Upload the code to the ESP8266.

---

## 3. Web Server Setup

### Clone the Repository

```bash
git clone https://github.com/O-tech477/SmartWaterPurifier.git
cd SmartWaterPurifier/WebServer
```

### Create Virtual Environment

```bash
python -m venv .venv
```

### Activate Virtual Environment

#### Linux/macOS

```bash
source .venv/bin/activate
```

#### Windows

```powershell
.venv\Scripts\activate
```

### Install Dependencies

Using `uv`:

```bash
pip install uv
uv sync
```

Or using pip:

```bash
pip install fastapi uvicorn
```

### Run the Server

```bash
uvicorn main:app --host 0.0.0.0 --port 8000
```

---

## Accessing the Dashboard

Open a browser and navigate to:

```text
http://<your-server-ip>:8000
```

The dashboard will display:
- Live TDS readings
- Turbidity values
- Water level data
- Recent sensor logs

---

## API Endpoints

| Endpoint | Method | Description |
|---|---|---|
| `/` | GET | Web dashboard |
| `/data` | POST | Receive sensor data |
| `/logs` | GET | Retrieve logged readings |
| `/download` | GET | Download CSV log file |

---

## Example JSON Payload

```json
{
  "tds": 420,
  "turbidity": 61,
  "water_level": 12
}
```

---

## Future Improvements

- MQTT-based communication
- Mobile application integration
- Cloud database storage
- Email/SMS alerts
- AI-based water quality prediction
- Automatic motor speed control
- Battery backup support

---

## Dashboard Preview

> Add screenshots of:
- Hardware setup
- OLED display
- Web dashboard
- Circuit diagram

Example:

```md
![Dashboard](images/dashboard.png)
```

---

## License

This project is open-source and available under the MIT License.

---

## Author

Developed as an IoT-based Smart Water Purification and Monitoring System using Arduino, ESP8266, and FastAPI.
