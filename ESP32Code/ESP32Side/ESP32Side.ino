#include <ESP8266WiFi.h>       
#include <ESP8266HTTPClient.h> 
#include <WiFiClient.h>     
#include <SoftwareSerial.h>   

//Credentials of the network
const char* ssid = "IITT";
const char* password = "user@123";

// The server address
const char* serverport = "http://10.27.78.6:8000/data";

//Strct with exact same strcuture as the arduino code to accept the data
struct __attribute__((packed)) DataPacket {
  float val1;
  float val2;
  float val3;
};

//Making a stuct instance to store the incoming data
DataPacket sensorData;

//Making serial object to communicate with arduino
SoftwareSerial arduinoSerial(D1, D2); 

bool getSensorData() {
  if (arduinoSerial.available() >= (sizeof(DataPacket) + 2)) { // +2 for Start/End markers
    if (arduinoSerial.read() == 0x42) {
      byte tempBuffer[sizeof(DataPacket)];
      arduinoSerial.readBytes(tempBuffer, sizeof(DataPacket));
      
      if (arduinoSerial.read() == 0x21) { // Check for End Marker
        memcpy(&sensorData, tempBuffer, sizeof(DataPacket));
        return true;
      }
    }
    // If markers are wrong, clear buffer
    while(arduinoSerial.available() > 0) arduinoSerial.read();
  }
  return false;
}

void sendDataToDashboard() {
  if (WiFi.status() == WL_CONNECTED) {
    WiFiClient client;
    HTTPClient http;

    http.begin(client, serverport);
    http.addHeader("Content-Type", "application/json");

    // Constructing the JSON string with the 3 float values
    String httpRequestData = "{";
    httpRequestData += "\"TDS\":" + String(sensorData.val1, 2) + ",";
    httpRequestData += "\"Turbidity\":" + String(sensorData.val2, 2) + ",";
    httpRequestData += "\"WaterLevel\":" + String(sensorData.val3, 2);
    httpRequestData += "}";

    int httpResponseCode = http.POST(httpRequestData);

    if (httpResponseCode > 0) {
      Serial.print("HTTP Response Code: ");
      Serial.println(httpResponseCode);

    } else {
      Serial.print("Error Code: ");
      Serial.println(httpResponseCode);
    }

    http.end();
  }
}


void setup() {

  //Setting up serial monitor and serial communication object
  Serial.begin(9600);
  arduinoSerial.begin(9600);

  //Setting up the wifi connectivity 
  WiFi.begin(ssid, password);

  Serial.println("Connecting to WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected to WiFi!");

}


void loop() {
if (getSensorData()) {
  sendDataToDashboard();
  delay(50);
  }  
}