//Importing libraries for code
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <SoftwareSerial.h>
#include <Wire.h>

//IMP: Do NOT use A4 and A5 pins I2C of display module 
//A4 and A5 pins for the display OLED. A4 -> SerialData
//A5 -> Serial clock

//Pins to control the motors 
//A -> Filtered Pathway
//B -> Unfiltered pathway
#define MotorAControl1 8
#define MotorAControl2 9
#define MotorBControl1 10
#define MotorBControl2 11

//Pins to take sensor readings
#define TurbidityIn A0 //Turbidity Sensor one analog input pin
#define TDSIn A1    //TDS Sensor one analog input pin
#define UltrasoundTrigger 12//Ultrasound Trigger pin
#define UltrasoundEcho 13 //Ultrasound echo pin

//Making a serial object to communicate with ESP8266
SoftwareSerial espSerial(2, 3);

//Threshhold to turn on the filtering pathway and water level
const int TDS_THRESHOLD = 500;
const int TURBIDITY_THRESHOLD = 75;
const int WATER_LEVEL_THRESHHOLD = 19;

//Turbidity sensor constants
//TODO:Need to tweak them later
const float VREF = 5;
const int ADCMAX = 1023;
const int SAMPLES_TURBIDITY = 10;

//Variables for the Ultrasound sensor
float distance, duration, totalDistance;
const int SAMPLES_ULTRASOUND = 10;

//Constants for the OLED display
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32
#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3C

//Making a display object
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

//Struct to hold the three sensor values 
struct __attribute__((packed)) DataPacket {
  float val1;
  float val2;
  float val3;
};

DataPacket sensorData;

//This function sends data to esp with UART 
void sendDataToESP(){
  espSerial.write(0x42); // Start Marker
  espSerial.write((byte*)&sensorData, sizeof(sensorData));
  espSerial.write(0x21); // End Marker (!)
}

//This reads the water level with ultrasound sensor
float readWaterLevel(){
  //Resetting the total distance 
  totalDistance = 0;
  //Taking 10 readings
  for (int i = 0; i < SAMPLES_ULTRASOUND; i++) {
    // Clear the trigger
    digitalWrite(UltrasoundTrigger, LOW);
    delayMicroseconds(2);
    
    // Send a 10 microsecond pulse
    digitalWrite(UltrasoundTrigger, HIGH);
    delayMicroseconds(10);
    digitalWrite(UltrasoundTrigger, LOW);

    // Read the echo
    duration = pulseIn(UltrasoundEcho, HIGH);
    
    // Calculate distance and add to total
    distance = (duration * 0.0343) / 2;
    totalDistance += distance;

    delay(10); 
  }
  float averageDistance = totalDistance / SAMPLES_ULTRASOUND;  
  return averageDistance;
}


/**
 * Converts analog voltage to TDS (ppm)
 * Based on the standard compensation formula.
 */
float tdsFromVoltage(float v) {
  // The 0.5 at the end is a standard temperature compensation factor (at 25°C)
  float tds = (133.42 * v * v * v - 255.86 * v * v + 857.39 * v) * 0.5;
  return tds;
}

/**
 * Reads the TDS sensor and returns the ppm value
 */
float readTDS() {
  int raw = analogRead(TDSIn);
  float voltage = (raw * VREF) / (float)ADCMAX;
  return tdsFromVoltage(voltage);
}

//Turbidity sensor helper function
float ntuFromVoltage(float volt){
  if (volt >= 4.42) return 0;
  if (volt <= 0.08) return 3000;
  return (4.42 - volt) * (3000.0 / (4.42 - 0.08));
}

//This averages the tubidity sensor value  
float readTurbidity(){
  float sumVolt = 0;

  //Taking an average of SAMPLES_TURBIDITY values for stability
  for (int i = 0; i < SAMPLES_TURBIDITY; i++)
  {
    float volt = (analogRead(TurbidityIn) * VREF) / ADCMAX;
    sumVolt += volt;
    delay(10); 
  }

  sumVolt = sumVolt/SAMPLES_TURBIDITY;

  float ntu = ntuFromVoltage(sumVolt);

  /*Serial.println("Turbidity:");
  Serial.print(ntu);
  Serial.println();*/


  return ntu;
}

//This turns on the motor A
void turnOnMotorA(){
  digitalWrite(MotorAControl1, HIGH);
  digitalWrite(MotorAControl2, LOW);
}

void turnOffMotors(){
  digitalWrite(MotorAControl1, LOW);
  digitalWrite(MotorAControl2, LOW);
  digitalWrite(MotorBControl1, LOW);
  digitalWrite(MotorBControl2, LOW);
}

//This turns on the motor B 
void turnOnMotorB(){
  digitalWrite(MotorBControl1, HIGH);
  digitalWrite(MotorBControl2, LOW);
}

//This function updates the sensor values in the sensorData struct created
void updateSensorData(){

  sensorData.val1 = readTDS();
  sensorData.val2 = readTurbidity();
  sensorData.val3 = readWaterLevel();;

}

//Function to display the sensor values on the screen
void displayOLEDSensors(){
  //Clearing screen to update values
  display.clearDisplay();

  //Rotating screen by 180 degree 
  display.setRotation(2);

  //Setting up text style
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0,0);

  //Rendering the text
  display.print("TDS: ");
  display.println(sensorData.val1);
  display.print(F("Tr: "));
  display.println(sensorData.val2);

  //Displaying the final screen
  display.display();
}

void displayOLEDNoWater(){
  //Clearing screen to update values
  display.clearDisplay();

  //Rotating screen by 180 degree 
  display.setRotation(2);

  //Setting up text style
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0,0);

  //Rendering the text
  display.println("WATER NOT DETECTED !!");

  //Displaying the final screen
  display.display();
}

void displayOLEDDanger(){
  //Clearing screen to update values
  display.clearDisplay();

  //Rotating screen by 180 degree 
  display.setRotation(2);

  //Setting up text style
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0,0);

  //Rendering the text
  display.println("Danger. High TDS !!");

  //Displaying the final screen
  display.display();
}

//Setup func which runs once
void setup(){

  //Setting Motor control pins
  pinMode(MotorAControl1, OUTPUT);
  pinMode(MotorAControl2, OUTPUT);
  pinMode(MotorBControl1, OUTPUT);
  pinMode(MotorBControl2, OUTPUT);

  //Setting pins to take in sensor values
  pinMode(TurbidityIn, INPUT);
  pinMode(TDSIn, INPUT);
  pinMode(UltrasoundTrigger, OUTPUT);
  pinMode(UltrasoundEcho, INPUT);

  //Initilizing the serial monitor
  Serial.begin(9600);
  espSerial.begin(9600);

  //Initilaizing the display
  Wire.begin();
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("OLED failed"));
    while (true); // stop if display fails
  }

  //Clearing any old text from the display
  display.clearDisplay();
  display.display();


  Serial.println("Initialized...");

}

//main loop for the programmes

void loop() {

  updateSensorData();
  if(sensorData.val3 < WATER_LEVEL_THRESHHOLD)
  {
  
    updateSensorData();
    sendDataToESP();
    displayOLEDSensors();

    if(sensorData.val1 > TDS_THRESHOLD)
    {
      displayOLEDDanger();
    }
    else if (sensorData.val2 > TURBIDITY_THRESHOLD)
    {
      displayOLEDSensors();
      turnOnMotorA();
      delay(5000);
      turnOffMotors();
    }
    else
    {
      displayOLEDSensors();
      turnOnMotorB();
      delay(5000);
      turnOffMotors();
    }
  }
  else
  {
    turnOffMotors();
    displayOLEDNoWater();
  }
  
}
