// VERSION 1.0.0
#include <ArduinoBLE.h>
#include <SFE_MicroOLED.h>
#include <Wire.h>

#include "MAX30105.h"

// -- Constant Values --

#define PIN_RESET 9
#define DC_JUMPER 1

// -- End Constant Values --

// -- EEPROM constants --

// -- End EEPROM constants

// -- Global Variables --

long unblockedValue;  // Average IR at power up
MAX30105 particleSensor;
MicroOLED oled(PIN_RESET, DC_JUMPER);

// -- End Global Variables --

// -- Global Configs --

// The variable below calibrates the LED output on your hardware.
byte ledBrightness = 34;
byte sampleAverage = 4;  // Options: 1, 2, 4, 8, 16, --32--
byte ledMode = 2;        // Options: 1 = Red only, --2 = Red + IR--, 3 = Red + IR + Green
int sampleRate = 50;     // Options: 50, 100, 200, 400, 800, 1000, 1600, --3200--
int pulseWidth = 411;    // Options: 69, 118, 215, --411--
int adcRange = 4096;     // Options: 2048, --4096--, 8192, 16384

// The variable below use to calculate Agtron from IR
int intersectionPoint = 117;
float deviation = 0.165;

// -- End Global Configs --

// -- Sub Routine Headers --

void measureSampleJob();
void displayPleaseLoadSample();
void displayMeasurement(int rLevel);

// -- End Sub Routine Headers --

// -- BLE Function Headers --

// -- End BLE Function Headers --

// -- Utility Function Headers --

String multiplyChar(char c, int n);
int mapIRToAgtron(int x);

// -- End Utillity Function Headers --

// -- Main Process --
void setup() {
  Serial.begin(9600);

  Wire.begin();
  oled.begin();      // Initialize the OLED
  oled.clear(ALL);   // Clear the display's internal memory
  oled.clear(PAGE);  // Clear the buffer.

  delay(100);  // Delay 100 ms
  oled.setFontType(3);

  // Initialize sensor
  if (particleSensor.begin(Wire, I2C_SPEED_FAST) == false)  // Use default I2C port, 400kHz speed
  {
    Serial.println("MAX30105 was not found. Please check wiring/power. ");
    while (1)
      ;
  }

  particleSensor.setup(ledBrightness, sampleAverage, ledMode, sampleRate, pulseWidth, adcRange);  // Configure sensor with these settings

  // Update to ignore readings under 30.000
  unblockedValue = 30000;
}

void loop() {
  measureSampleJob();
}

// -- End Main Process --

// Sub Routines

int measureSampleJobTimer = millis();
void measureSampleJob() {
  if (measureSampleJobTimer - millis() > 100) {
    int rLevel = particleSensor.getIR();
    long currentDelta = rLevel - unblockedValue;

    if (currentDelta > (long)100) {
      displayMeasurement(rLevel / 1000);
    } else {
      displayPleaseLoadSample();
    }
    measureSampleJobTimer = millis();
  }
}

void displayPleaseLoadSample() {
  oled.clear(PAGE);
  oled.setCursor(0, 0);
  oled.setFontType(1);
  oled.print("Please load   sample!");
  oled.display();
}

void displayMeasurement(int rLevel) {
  oled.clear(PAGE);
  oled.setCursor(0, 0);

  int calibratedReading = mapIRToAgtron(rLevel);
  int centerPadding = 4 - String(calibratedReading).length();
  String paddingText = multiplyChar(' ', centerPadding);

  oled.setFontType(3);
  oled.print(paddingText);
  oled.print(calibratedReading);

  Serial.println("real:" + String(rLevel));
  Serial.println("agtron:" + String(calibratedReading));
  Serial.println("===========================");

  oled.display();
}

// Sub Routines

// Utillity Functions

String multiplyChar(char c, int n) {
  String result = "";
  for (int i = 0; i < n; i++) {
    result += c;
  }
  return result;
}

int mapIRToAgtron(int x) {
  return round(x - (intersectionPoint - x) * deviation);
}

// End Utillity Functions