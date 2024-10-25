#include <Wire.h>
#include <SH1106.h> // Include the SH1106 library
#include <Adafruit_BME680.h>
#include <WiFi.h> // Include the WiFi library for connecting to WiFi

// Define the relay pins
#define RELAY1_PIN 25
#define RELAY2_PIN 26

// OLED display configuration
#define SCREEN_WIDTH 128  
#define SCREEN_HEIGHT 64
SH1106 display(0x3C, SDA, SCL); // Initialize the SH1106 display

// BME680 sensor configuration
Adafruit_BME680 bme;

// WiFi credentials
const char* ssid = "xxxx-xxxx-xxxx";
const char* password = "xxxx-xxxx-xxxx";

// Thresholds for relay control
const float TEMP_THRESHOLD = 30.0; // Temperature threshold for relay 1 (in degrees Celsius)
const float HUMIDITY_THRESHOLD = 40.0; // Humidity threshold for relay 2 (in %)

void setup() {
  // Start the serial communication
  Serial.begin(115200);

  // Initialize the OLED display
  if (!display.init()) { 
    Serial.println(F("SH1106 allocation failed"));
    for (;;);
  }
  display.flipScreenVertically();
  display.clear();
  
  // Initialize the BME680 sensor
  if (!bme.begin()) {
    Serial.println("Could not find a valid BME680 sensor, check wiring!");
    while (1);
  }

  // Set up the BME680 sensor
  bme.setTemperatureOversampling(BME680_OS_8X);
  bme.setHumidityOversampling(BME680_OS_2X);
  bme.setPressureOversampling(BME680_OS_4X);
  bme.setIIRFilterSize(BME680_FILTER_SIZE_3);
  bme.setGasHeater(320, 150); // 320°C for 150 ms

  // Initialize the relay pins
  pinMode(RELAY1_PIN, OUTPUT);
  pinMode(RELAY2_PIN, OUTPUT);

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
}

void loop() {
  // Perform a BME680 measurement
  if (!bme.performReading()) {
    Serial.println("Failed to perform reading");
    return;
  }

  // Read data from the BME680 sensor
  float temperature = bme.temperature;
  float humidity = bme.humidity;
  float pressure = bme.pressure / 100.0;
  float gas = bme.gas_resistance / 1000.0;

  // Display data on the OLED screen
  display.clear();
  display.setFont(ArialMT_Plain_10);
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.drawString(0, 0, "Temp: " + String(temperature) + " *C");
  display.drawString(0, 12, "Humidity: " + String(humidity) + " %");
  display.drawString(0, 24, "Pressure: " + String(pressure) + " hPa");
  display.drawString(0, 36, "Gas: " + String(gas) + " KOhms");
  display.display();

  // Control the relays based on sensor data
  if (temperature > TEMP_THRESHOLD) {
    digitalWrite(RELAY1_PIN, HIGH); // Turn on relay 1
  } else {
    digitalWrite(RELAY1_PIN, LOW); // Turn off relay 1
  }

  if (humidity < HUMIDITY_THRESHOLD) {
    digitalWrite(RELAY2_PIN, HIGH); // Turn on relay 2
  } else {
    digitalWrite(RELAY2_PIN, LOW); // Turn off relay 2
  }

  // Wait for a second before the next loop
  delay(1000);
}

pakka code to shoe temperature, humidity, pressure and gas
