// VERSION 1.0.0
#include <ArduinoBLE.h>
#include <EEPROM.h>
#include <SFE_MicroOLED.h>
#include <Wire.h>

#include "MAX30105.h"

// -- Constant Values --
#ifndef FIRMWARE_REVISION_STRING
#define FIRMWARE_REVISION_STRING "v0.1"
#endif  //FIRMWARE_REVISION_STRING

#define PIN_RESET 9
#define DC_JUMPER 1

#define BLE_UUID_ROAST_METER_SERVICE "875A0EE0-03DD-4225-AE06-35E8AE92B84C"
#define BLE_UUID_PARTICLE_SENSOR "C32AFDBA-E9F2-453E-9612-85FBF4108AB2"
#define BLE_UUID_AGTRON "CE216811-0AD9-4AFF-AE29-8B171093A95F"
#define BLE_UUID_METER_STATE "8ACE2828-996F-48E4-8E9C-8284678B4B57"

#define BLE_UUID_DEVICE_INFOMATION_SERVICE "180A"
#define BLE_UUID_FIRMWARE_REVISION "2A26"

#define BLE_UUID_SETTING_SERVICE "59021473-DFC6-425A-9729-09310EBE535E"
#define BLE_UUID_LED_BRIGHTNESS "8313695F-3EA1-458B-BD2A-DF4AEE218514"
#define BLE_UUID_INTERSECTION_POINT "69548C4B-87D0-4E3E-AC6C-B143C7B2AB30"
#define BLE_UUID_DEVIATION "D17234FA-0F48-429A-9E9B-F5DB774EF682"
#define BLE_UUID_BLE_NAME "CDE44FD7-4C1E-42A0-8368-531DC87F6B56"
#define BLE_UUID_UNBLOCK_LEVEL "B8BEFA0C-FFDD-4096-9ACD-208657B4B73C"

#define STATE_SETUP 0
#define STATE_WARMUP 1
#define STATE_READY 2
#define STATE_MEASURED 3

// -- End Constant Values --

// -- EEPROM constants --

#define EEPROM_MAX_LENGTH 256                  // 1024 bytes
#define EEPROM_VALID_IDX 0                     // 1 byte
#define EEPROM_VALID_CODE (0xAA)               // uint8
#define EEPROM_LED_BRIGHTNESS_IDX 1            // 1 byte
#define EEPROM_LED_BRIGHTNESS_DEFAULT 135      // uint8
#define EEPROM_INTERSECTION_POINT_IDX 2        // 1 byte
#define EEPROM_INTERSECTION_POINT_DEFAULT 117  // uint8
#define EEPROM_DEVIATION_IDX 3                 // 1 byte
#define EEPROM_DEVIATION_DEFAULT 0.165f        // float 32 bit 4 bytes
#define EEPROM_BLE_NAME_IDX 128                // 64 byte - 1 byte length + 63 ASCII

// -- End EEPROM constants

// -- Global Variables --

uint32_t unblockedValue = 30000;  // Average IR at power up

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
int adcRange = 16384;    // Options: 2048, 4096, 8192, --16384--

// The variable below use to calculate Agtron from IR
int intersectionPoint = 117;  // !EEPROM setup
float deviation = 0.165;      // !EEPROM setup

// BLE
String bleName;  // !EEPROM setup

// -- End Global Setting --

// -- Setup Headers --

void setupEEPROM();
void setupBLE();
void setupParticleSensor();

// -- Setup Headers --

// -- Sub Routine Headers --

void displayStartUp();
void warmUpLED();
void measureSampleJob();
void displayPleaseLoadSample();
void displayMeasurement(int rLevel);

// -- End Sub Routine Headers --

// -- BLE Function Headers --
BLEService roastMeterService(BLE_UUID_ROAST_METER_SERVICE);

BLEDescriptor particleSensorDescriptor("2901", "particle_sensor");
BLEUnsignedIntCharacteristic particleSensorCharacteristic(BLE_UUID_PARTICLE_SENSOR, BLERead | BLENotify);

BLEDescriptor agtronDescriptor("2901", "agtron");
BLEByteCharacteristic agtronCharacteristic(BLE_UUID_AGTRON, BLERead | BLENotify);

BLEDescriptor meterStateDescriptor("2901", "meter_state");
BLEByteCharacteristic meterStateCharacteristic(BLE_UUID_METER_STATE, BLERead | BLENotify);

BLEService settingService(BLE_UUID_SETTING_SERVICE);

BLEDescriptor ledBrightnessLevelDescriptor("2901", "led_brightness_level");
BLEByteCharacteristic ledBrightnessLevelCharacteristic(BLE_UUID_LED_BRIGHTNESS, BLERead | BLEWrite);

BLEDescriptor intersectionPointDescriptor("2901", "intersection_point");
BLEByteCharacteristic intersectionPointCharacteristic(BLE_UUID_INTERSECTION_POINT, BLERead | BLEWrite);

BLEDescriptor deviationDescriptor("2901", "deviation");
BLEFloatCharacteristic deviationCharacteristic(BLE_UUID_DEVIATION, BLERead | BLEWrite);

BLEDescriptor bleNameDescriptor("2901", "ble_name");
BLEStringCharacteristic bleNameCharacteristic(BLE_UUID_BLE_NAME, BLERead | BLEWrite, 64);

BLEService deviceInfomationService(BLE_UUID_DEVICE_INFOMATION_SERVICE);
BLEStringCharacteristic firmwareRevisionCharacteristic(BLE_UUID_FIRMWARE_REVISION, BLERead | BLEWrite, 64);

// -- End BLE Function Headers --

// -- BLE Handler Headers --

void blePeripheralConnectHandler(BLEDevice central);
void blePeripheralDisconnectHandler(BLEDevice central);

void bleLEDBrightnessLevelWriten(BLEDevice central, BLECharacteristic characteristic);
void bleIntersectionPointWriten(BLEDevice central, BLECharacteristic characteristic);
void bleDeviationWriten(BLEDevice central, BLECharacteristic characteristic);
void bleBLENameWriten(BLEDevice central, BLECharacteristic characteristic);

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

  setupParticleSensor();

  displayStartUp();
  warmUpLED();
}

void loop() {
  measureSampleJob();
  BLE.poll();
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
  BLE.setDeviceName(bleName.c_str());
  // set the UUID for the service this peripheral advertises
  BLE.setAdvertisedService(roastMeterService);

  // add the characteristic to the service
  roastMeterService.addCharacteristic(particleSensorCharacteristic);
  roastMeterService.addCharacteristic(agtronCharacteristic);
  roastMeterService.addCharacteristic(meterStateCharacteristic);

  settingService.addCharacteristic(ledBrightnessLevelCharacteristic);
  settingService.addCharacteristic(intersectionPointCharacteristic);
  settingService.addCharacteristic(deviationCharacteristic);
  settingService.addCharacteristic(bleNameCharacteristic);

  deviceInfomationService.addCharacteristic(firmwareRevisionCharacteristic);

  // add service
  BLE.addService(roastMeterService);
  BLE.addService(settingService);
  BLE.addService(deviceInfomationService);

  // assign event handlers for connected, disconnected to peripheral
  BLE.setEventHandler(BLEConnected, blePeripheralConnectHandler);
  BLE.setEventHandler(BLEDisconnected, blePeripheralDisconnectHandler);

  agtronCharacteristic.addDescriptor(agtronDescriptor);
  particleSensorCharacteristic.addDescriptor(particleSensorDescriptor);
  meterStateCharacteristic.addDescriptor(meterStateDescriptor);

  ledBrightnessLevelCharacteristic.addDescriptor(ledBrightnessLevelDescriptor);
  ledBrightnessLevelCharacteristic.setEventHandler(BLEWritten, bleLEDBrightnessLevelWriten);

  intersectionPointCharacteristic.addDescriptor(intersectionPointDescriptor);
  intersectionPointCharacteristic.setEventHandler(BLEWritten, bleIntersectionPointWriten);

  deviationCharacteristic.addDescriptor(deviationDescriptor);
  deviationCharacteristic.setEventHandler(BLEWritten, bleDeviationWriten);

  bleNameCharacteristic.addDescriptor(bleNameDescriptor);
  bleNameCharacteristic.setEventHandler(BLEWritten, bleBLENameWriten);

  // Assign current value and setting for BLE Characteristic
  particleSensorCharacteristic.setValue(0);
  agtronCharacteristic.setValue(0);
  meterStateCharacteristic.setValue(STATE_SETUP);

  ledBrightnessLevelCharacteristic.setValue(ledBrightness);
  intersectionPointCharacteristic.setValue(intersectionPoint);
  deviationCharacteristic.setValue(deviation);
  bleNameCharacteristic.setValue(bleName);

  firmwareRevisionCharacteristic.writeValue(FIRMWARE_REVISION_STRING);

  // start advertising
  BLE.advertise();

  Serial.println(("Bluetooth® device active, waiting for connections..."));
}

void setupParticleSensor() {
  particleSensor.setup(ledBrightness, sampleAverage, ledMode, sampleRate, pulseWidth, adcRange);  // Configure sensor with these settings

  particleSensor.setPulseAmplitudeRed(0);
  particleSensor.setPulseAmplitudeGreen(0);

  particleSensor.disableSlots();
  particleSensor.enableSlot(2, 0x02);  // Enable only SLOT_IR_LED = 0x02
}

// -- End Setups --

// Sub Routines

void displayStartUp() {
  oled.clear(PAGE);
  oled.setCursor(0, 0);
  oled.setFontType(1);
  oled.print("Roast  ");
  oled.print("Meter  ");
  oled.setFontType(0);
  oled.print(FIRMWARE_REVISION_STRING);
  oled.display();

  delay(2000);

  oled.clear(PAGE);
  oled.setCursor(0, 0);
  oled.setFontType(1);
  oled.print(bleName);
  oled.display();

  delay(2000);
}

void warmUpLED() {
  meterStateCharacteristic.writeValue(STATE_WARMUP);

  int countDownSeconds = 60;
  unsigned long jobTimerStart = millis();
  unsigned long jobTimer = jobTimerStart;

  while (millis() - jobTimerStart <= 60 * 1000) {
    unsigned long elapsed = millis() - jobTimer;

    if (elapsed > 100) {
      oled.clear(PAGE);
      oled.setCursor(0, 0);
      oled.setFontType(1);

      countDownSeconds = 60 - ((millis() - jobTimerStart) / 1000);

      oled.print("Warm Up " + String(countDownSeconds) + "s");
      oled.display();

      jobTimer = millis();
    }

    BLE.poll();
  }
}

unsigned long measureSampleJobTimer = millis();
void measureSampleJob() {
  if (millis() - measureSampleJobTimer > 100) {
    int rLevel = particleSensor.getIR();
    long currentDelta = (long)rLevel - (long)unblockedValue;

    if (currentDelta > (long)100) {
      int calibratedAgtronLevel = mapIRToAgtron(rLevel / 1000);

      agtronCharacteristic.writeValue(calibratedAgtronLevel);
      particleSensorCharacteristic.writeValue(rLevel / 1000);
      meterStateCharacteristic.writeValue(STATE_MEASURED);

      displayMeasurement(calibratedAgtronLevel);

      Serial.println("real:" + String(rLevel));
      Serial.println("agtron:" + String(calibratedAgtronLevel));
      Serial.println("===========================");
    } else {
      agtronCharacteristic.writeValue(0);
      particleSensorCharacteristic.writeValue(0);
      meterStateCharacteristic.writeValue(STATE_READY);

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

void displayMeasurement(int agtronLevel) {
  oled.clear(PAGE);
  oled.setCursor(0, 0);

  int centerPadding = 4 - String(agtronLevel).length();
  String paddingText = multiplyChar(' ', centerPadding);

  oled.setFontType(3);
  oled.print(paddingText);
  oled.print(agtronLevel);

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

void bleLEDBrightnessLevelWriten(BLEDevice central, BLECharacteristic characteristic) {
  ledBrightness = ledBrightnessLevelCharacteristic.value();

  Serial.print("bleLEDBrightnessLevelWriten event, written: ");
  Serial.println(ledBrightness);

  EEPROM.put(EEPROM_LED_BRIGHTNESS_IDX, ledBrightness);

  setupParticleSensor();
}

void bleIntersectionPointWriten(BLEDevice central, BLECharacteristic characteristic) {
  intersectionPoint = intersectionPointCharacteristic.value();

  Serial.print("bleIntersectionPointWriten event, written: ");
  Serial.println(intersectionPoint);

  EEPROM.put(EEPROM_INTERSECTION_POINT_IDX, intersectionPoint);
}

void bleDeviationWriten(BLEDevice central, BLECharacteristic characteristic) {
  deviation = deviationCharacteristic.value();

  Serial.print("bleDeviationWriten event, written: ");
  Serial.println(deviation);

  EEPROM.put(EEPROM_DEVIATION_IDX, deviation);
}

void bleBLENameWriten(BLEDevice central, BLECharacteristic characteristic) {
  String newBLEName = bleNameCharacteristic.value();

  if (newBLEName.length() > 63) {
    Serial.println("bleBLENameWriten event, written rejected!. String length exceed 63.");
    bleNameCharacteristic.setValue(bleName.c_str());

    return;
  }

  bleName = newBLEName;
  Serial.print("bleBLENameWriten event, written: ");
  Serial.println(bleName);

  BLE.setLocalName(bleName.c_str());
  BLE.setDeviceName(bleName.c_str());

  writeStringToEEPROM(EEPROM_BLE_NAME_IDX, bleName);
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