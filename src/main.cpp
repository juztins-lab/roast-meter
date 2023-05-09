// VERSION 1.0.0
#include <ArduinoBLE.h>
#include <EEPROM.h>
#include <SFE_MicroOLED.h>
#include <Wire.h>

#include "MAX30105.h"

// -- Constant Values --

#define PIN_RESET 9
#define DC_JUMPER 1

// -- End Constant Values --

// -- EEPROM constants --

#define EEPROM_MAX_LENGTH 256                  // 1024 bytes
#define EEPROM_VALID_IDX 0                     // 1 byte
#define EEPROM_VALID_CODE (0xA0)               // uint8
#define EEPROM_LED_BRIGHTNESS_IDX 1            // 1 byte
#define EEPROM_LED_BRIGHTNESS_DEFAULT 34       // uint8
#define EEPROM_INTERSECTION_POINT_IDX 2        // 1 byte
#define EEPROM_INTERSECTION_POINT_DEFAULT 117  // uint8
#define EEPROM_DEVIATION_IDX 3                 // 1 byte
#define EEPROM_DEVIATION_DEFAULT 0.165f        // float 32 bit 4 bytes
#define EEPROM_BLE_NAME_IDX 128                // 64 byte - 1 byte length + 63 ASCII

// -- End EEPROM constants

// -- Global Variables --

long unblockedValue = 30000;  // Average IR at power up

MAX30105 particleSensor;
MicroOLED oled(PIN_RESET, DC_JUMPER);

// -- End Global Variables --

// -- Global Setting --

// The variable below calibrates the LED output on your hardware.
byte ledBrightness;      // !EEPROM setup
byte sampleAverage = 4;  // Options: 1, 2, 4, 8, 16, --32--
byte ledMode = 2;        // Options: 1 = Red only, --2 = Red + IR--, 3 = Red + IR + Green
int sampleRate = 50;     // Options: 50, 100, 200, 400, 800, 1000, 1600, --3200--
int pulseWidth = 411;    // Options: 69, 118, 215, --411--
int adcRange = 4096;     // Options: 2048, --4096--, 8192, 16384

// The variable below use to calculate Agtron from IR
int intersectionPoint = 117;  // !EEPROM setup
float deviation = 0.165;      // !EEPROM setup

// BLE
String bleName;  // !EEPROM setup

// -- End Global Setting --

// -- Setup Headers --

void setupEEPROM();
void setupBLE();

// -- Setup Headers --

// -- Sub Routine Headers --

void measureSampleJob();
void displayPleaseLoadSample();
void displayMeasurement(int rLevel);

// -- End Sub Routine Headers --

// -- BLE Function Headers --

BLEService roastMeterService("875A0EE0-03DD-4225-AE06-35E8AE92B84C");
BLEByteCharacteristic particleSensorCharacteristic("C32AFDBA-E9F2-453E-9612-85FBF4108AB2", BLERead | BLENotify);
BLEByteCharacteristic agtronCharacteristic("CE216811-0AD9-4AFF-AE29-8B171093A95F", BLERead | BLENotify);

BLEService settingService("59021473-DFC6-425A-9729-09310EBE535E");
BLEByteCharacteristic ledBrightnessLevelCharacteristic("8313695F-3EA1-458B-BD2A-DF4AEE218514", BLERead | BLEWrite);
BLEByteCharacteristic intersectionPointCharacteristic("69548C4B-87D0-4E3E-AC6C-B143C7B2AB30", BLERead | BLEWrite);
BLEFloatCharacteristic deviationCharacteristic("D17234FA-0F48-429A-9E9B-F5DB774EF682", BLERead | BLEWrite);
BLEStringCharacteristic bleNameCharacteristic("CDE44FD7-4C1E-42A0-8368-531DC87F6B56", BLERead | BLEWrite, 64);

// -- End BLE Function Headers --

// -- BLE Handler Headers --

void blePeripheralConnectHandler(BLEDevice central);
void blePeripheralDisconnectHandler(BLEDevice central);

// -- End BLE Handler Headers --

// -- Utility Function Headers --

String multiplyChar(char c, int n);
String stringLastN(String input, int n);
int mapIRToAgtron(int x);
void writeStringToEEPROM(int addrOffset, const String &strToWrite);
String readStringFromEEPROM(int addrOffset);

// -- End Utillity Function Headers --

// -- Main Process --
void setup() {
  Serial.begin(9600);

  BLE.begin();

  Wire.begin();

  oled.begin();      // Initialize the OLED
  oled.clear(ALL);   // Clear the display's internal memory
  oled.clear(PAGE);  // Clear the buffer.

  setupEEPROM();
  setupBLE();

  // Initialize sensor
  if (particleSensor.begin(Wire, I2C_SPEED_FAST) == false)  // Use default I2C port, 400kHz speed
  {
    Serial.println("MAX30105 was not found. Please check wiring/power. ");
    while (1)
      ;
  }

  particleSensor.setup(ledBrightness, sampleAverage, ledMode, sampleRate, pulseWidth, adcRange);  // Configure sensor with these settings
}

void loop() {
  measureSampleJob();
}

// -- End Main Process --

// -- Setups --

void setupEEPROM() {
  EEPROM.init();
  // You may choose to enable more or less EEPROM -
  // Default length is 1024 bytes (if setLength is not called)
  EEPROM.setLength(EEPROM_MAX_LENGTH);
  // Note: larger sizes will increase RAM usage and execution time

  // use EEPROM.get(int index, T type) to retrieve
  // an arbitrary type from flash memory
  uint8_t eeprom_valid;
  EEPROM.get(EEPROM_VALID_IDX, eeprom_valid);

  if (eeprom_valid != EEPROM_VALID_CODE) {
    Serial.println("EEPROM was invalid");

    // use EEPROM.put(int index, T type) to store
    // a variable in psuedo-eeprom flash memory
    // use a variable with a type so that EEPROM
    // knows how much memory to use
    uint8_t code_to_store = EEPROM_VALID_CODE;
    EEPROM.put(EEPROM_VALID_IDX, code_to_store);

    // store default LED brightness value in EEPROM
    uint8_t led_brightness_to_store = EEPROM_LED_BRIGHTNESS_DEFAULT;
    EEPROM.put(EEPROM_LED_BRIGHTNESS_IDX, led_brightness_to_store);

    // store default intersection point value in EEPROM
    uint8_t intersection_point_to_store = EEPROM_INTERSECTION_POINT_DEFAULT;
    EEPROM.put(EEPROM_INTERSECTION_POINT_IDX, intersection_point_to_store);

    // store default deviation value in EEPROM
    float deviation_to_store = EEPROM_DEVIATION_DEFAULT;
    EEPROM.put(EEPROM_DEVIATION_IDX, deviation_to_store);

    // store default BLE name in EEPROM
    BLE.begin();
    String ble_name_to_store = "Roast Meter " + stringLastN(BLE.address(), 5);
    writeStringToEEPROM(EEPROM_BLE_NAME_IDX, ble_name_to_store);

    Serial.println("EEPROM initialized");
  }

  EEPROM.get(EEPROM_VALID_IDX, eeprom_valid);
  if (eeprom_valid != EEPROM_VALID_CODE) {
    Serial.println("EEPROM CAN NOT initialized");
    while (1) {
    };
  }

  Serial.println("EEPROM is valid");

  // Load setting from EEPROM

  uint8_t eeprom_led_brightness;
  EEPROM.get(EEPROM_LED_BRIGHTNESS_IDX, eeprom_led_brightness);
  ledBrightness = eeprom_led_brightness;
  Serial.println("Set ledBrightness to " + String(ledBrightness));

  uint8_t eeprom_intersection_point;
  EEPROM.get(EEPROM_INTERSECTION_POINT_IDX, eeprom_intersection_point);
  intersectionPoint = eeprom_intersection_point;
  Serial.println("Set intersection point to " + String(intersectionPoint));

  float eeprom_deviation;
  EEPROM.get(EEPROM_DEVIATION_IDX, eeprom_deviation);
  deviation = eeprom_deviation;
  Serial.print("Set deviation to ");
  Serial.print(deviation);
  Serial.println();

  bleName = readStringFromEEPROM(EEPROM_BLE_NAME_IDX);
  Serial.println("Set BLE name to " + String(bleName));
}

void setupBLE() {
  if (!BLE.begin()) {
    Serial.println("starting Bluetooth® Low Energy module failed!");
  }

  // set the local name peripheral advertises
  BLE.setLocalName(bleName.c_str());
  // set the UUID for the service this peripheral advertises
  BLE.setAdvertisedService(roastMeterService);

  // add the characteristic to the service
  roastMeterService.addCharacteristic(particleSensorCharacteristic);
  roastMeterService.addCharacteristic(agtronCharacteristic);

  settingService.addCharacteristic(ledBrightnessLevelCharacteristic);
  settingService.addCharacteristic(intersectionPointCharacteristic);
  settingService.addCharacteristic(deviationCharacteristic);
  settingService.addCharacteristic(bleNameCharacteristic);

  // add service
  BLE.addService(roastMeterService);
  BLE.addService(settingService);

  // assign event handlers for connected, disconnected to peripheral
  BLE.setEventHandler(BLEConnected, blePeripheralConnectHandler);
  BLE.setEventHandler(BLEDisconnected, blePeripheralDisconnectHandler);

  particleSensorCharacteristic.setValue(0);
  agtronCharacteristic.setValue(0);

  ledBrightnessLevelCharacteristic.setValue(ledBrightness);
  intersectionPointCharacteristic.setValue(intersectionPoint);
  deviationCharacteristic.setValue(deviation);
  bleNameCharacteristic.setValue(bleName);

  // start advertising
  BLE.advertise();

  Serial.println(("Bluetooth® device active, waiting for connections..."));
}

// -- End Setups --

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

// -- End Sub Routines --

// -- BLE Handler --

void blePeripheralConnectHandler(BLEDevice central) {
  // central connected event handler
  Serial.print("BLE Connected event, central: ");
  Serial.println(central.address());
}

void blePeripheralDisconnectHandler(BLEDevice central) {
  // central disconnected event handler
  Serial.print("BLE Disconnected event, central: ");
  Serial.println(central.address());
}

// -- End BLE Handler --

// -- Utillity Functions --

String multiplyChar(char c, int n) {
  String result = "";
  for (int i = 0; i < n; i++) {
    result += c;
  }
  return result;
}

String stringLastN(String input, int n) {
  int inputSize = input.length();

  return (n > 0 && inputSize > n) ? input.substring(inputSize - n) : "";
}

int mapIRToAgtron(int x) {
  return round(x - (intersectionPoint - x) * deviation);
}

// https://roboticsbackend.com/arduino-write-string-in-eeprom/
void writeStringToEEPROM(int addrOffset, const String &strToWrite) {
  byte len = strToWrite.length();
  EEPROM.write(addrOffset, len);
  for (int i = 0; i < len; i++) {
    EEPROM.write(addrOffset + 1 + i, strToWrite[i]);
  }
}

String readStringFromEEPROM(int addrOffset) {
  int newStrLen = EEPROM.read(addrOffset);
  char data[newStrLen + 1];

  for (int i = 0; i < newStrLen; i++) {
    data[i] = EEPROM.read(addrOffset + 1 + i);
  }
  data[newStrLen] = '\0';

  return String(data);
}

// -- End Utillity Functions --