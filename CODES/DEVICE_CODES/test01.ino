#include "BluetoothSerial.h"
#include <Wire.h>
#include <Adafruit_BMP280.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// --------------------------------------------- Pin Config
#define sensorPin 36
#define Humidifyer 23
#define AIR_SDA_PIN 26
#define AIR_SCL_PIN 25
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define DIS_SDA_PIN 21
#define DIS_SCL_PIN 22

// --------------------------------------------- System Config
BluetoothSerial SerialBT;
Adafruit_BMP280 bmp;

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);


// ------------------------------------------------------------------------------------------------------ SETUP
void setup() 
{
  Serial.begin(115200);

  // ------------------------------------------------------------- Bluetooth Name
  SerialBT.begin("ESP32_CPAP"); 

  // ------------------------------------------------------------- Humidifyer  
  pinMode(Humidifyer, OUTPUT);  
  digitalWrite(Humidifyer, LOW);

  // ------------------------------------------------------------- Air Preasure Sensor
  Wire.begin(AIR_SDA_PIN, AIR_SCL_PIN); 
  if (!bmp.begin(0x76))  // use 0x77 if needed
  {  
    Serial.println("BMP280 not found!");
    while (1);
  }

  //---------------------------------------------------------------OLED Display
  Wire.begin(DIS_SDA_PIN, DIS_SCL_PIN);

  // OLED init
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) 
  {
    Serial.println("OLED not found");
    while (1);
  }  
  display.clearDisplay();

  // ------------------------------------------------------------- Initialized
  Serial.println("BMP280 initialized");
  Serial.println("Bluetooth is Configuration done! Waiting for connection...");

  delay(500);
}

//------------------------------------------------------------------------------------------------------ LOOP
void loop() 
{
  display.clearDisplay();

  // -------------------------------------------------------- Sensor Value Read
  int sensorValue = analogRead(sensorPin);
  float temperature = bmp.readTemperature();
  float pressure = bmp.readPressure() / 100.0; // hPa
  float altitude = bmp.readAltitude(1013.25);

  // ------------------------------------------------------ Sensor check
  if (isnan(sensorPin)) 
  {
    Serial.println("!!ERROR!!, Sensor is not connected");
    SerialBT.println("!!ERROR!!, Sensor is not connected");
    return;
  }
  // ------------------------------------------------------- Humidifyer Controll
  if(sensorValue < 1000)
  {
    digitalWrite(Humidifyer, HIGH);
    Serial.println("Humidifyer ON");
    SerialBT.println("Humidifyer ON");
  }
  else 
  {
    digitalWrite(Humidifyer, LOW);
    Serial.println("Humidifyer OFF");
    SerialBT.println("Humidifyer OFF");
  }
  // --------------------------------------------------------- Serial Print
  Serial.print("PaperSensor: ");
  Serial.print(sensorValue);
  Serial.print(", ");
  
  Serial.print("Temp: ");
  Serial.print(temperature);
  Serial.print(" °C");
  Serial.print(", ");

  Serial.print("Pressure: ");
  Serial.print(pressure);
  Serial.print(" hPa");
  Serial.print(", ");

  Serial.print("Altitude: ");
  Serial.print(altitude);
  Serial.print(" m");
  Serial.println(", ");
 
  //----------------------------------------------------------- Bluetooth Print
  SerialBT.print("PaperSensor: ");
  SerialBT.print(sensorValue);
  SerialBT.print(", ");
  
  SerialBT.print("Temp: ");
  SerialBT.print(temperature);
  SerialBT.print(" °C");
  SerialBT.print(", ");

  SerialBT.print("Pressure: ");
  SerialBT.print(pressure);
  SerialBT.print(" hPa");
  SerialBT.print(", ");

  SerialBT.print("Altitude: ");
  SerialBT.print(altitude);
  SerialBT.print(" m");
  SerialBT.println(", ");

// --------------------------------------------------------- OLED Display
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println("CPAP Monitor");

  display.setCursor(0, 20);
  display.print("Temp: ");
  display.print(temperature);
  display.println(" C");

  display.setCursor(0, 35);
  display.print("Pressure:");
  display.print(pressure);
  display.println(" hPa");

  display.display();
  delay(100); 
}
