#include <Wire.h>
#include "MAX30105.h"
#include <SFE_MicroOLED.h>

#define PIN_RESET 9
#define DC_JUMPER 1

MAX30105 particleSensor;
MicroOLED oled(PIN_RESET, DC_JUMPER);

long samplesTaken = 0;  //Counter for calculating the Hz or read rate
long unblockedValue;    //Average IR at power up
long startTime;         //Used to calculate measurement rate

String multiplyChar(char c, int n) {
  String result = "";
  for (int i = 0; i < n; i++) {
    result += c;
  }
  return result;
}

void displayMeasurement(int rLevel) {
  oled.setCursor(0, 0);

  String strLevel = String(rLevel);
  char pad_char = ' ';

  String pad_str = multiplyChar(pad_char, 3);
  oled.print(pad_str);
  oled.print(strLevel);
  oled.print(pad_str);

  oled.setCursor(0, 16);

  if (rLevel >= 110) {
    oled.print("exlight");
    oled.print(multiplyChar(pad_char, 1));
  }

  if (rLevel >= 96 && rLevel < 110) {
    oled.print("vlight");
    oled.print(multiplyChar(pad_char, 2));
  }

  if (rLevel >= 90 && rLevel < 95) {
    oled.print("light");
    oled.print(multiplyChar(pad_char, 3));
  }

  if (rLevel >= 81 && rLevel < 90) {
    oled.print("mlight");
    oled.print(multiplyChar(pad_char, 2));
  }

  if (rLevel >= 71 && rLevel < 80) {
    oled.print("medium");
    oled.print(multiplyChar(pad_char, 2));
  }

  if (rLevel >= 61 && rLevel < 70) {
    oled.print("dark");
    oled.print(multiplyChar(pad_char, 4));
  }

  if (rLevel < 61) {
    oled.print("vdark");
    oled.print(multiplyChar(pad_char, 3));
  }

  oled.display();
}

void setup() {
  Serial.begin(9600);
  
  Wire.begin();
  oled.begin();    // Initialize the OLED
  oled.clear(ALL); // Clear the display's internal memory
  oled.display();  // Display what's in the buffer (splashscreen)

  delay(1000); // Delay 1000 ms

  oled.clear(PAGE); // Clear the buffer.

  // Initialize sensor
  if (particleSensor.begin(Wire, I2C_SPEED_FAST) == false)  //Use default I2C port, 400kHz speed
  {
    Serial.println("MAX30105 was not found. Please check wiring/power. ");
    while (1)
      ;
  }

  //Setup to sense up to 18 inches, max LED brightness
  byte ledBrightness = 0x1F;  //Options: 0=Off to 255=50mA
  byte sampleAverage = 16;     //Options: 1, 2, 4, 8, 16, 32
  byte ledMode = 2;           //Options: 1 = Red only, 2 = Red + IR, 3 = Red + IR + Green
  int sampleRate = 400;       //Options: 50, 100, 200, 400, 800, 1000, 1600, 3200
  int pulseWidth = 411;       //Options: 69, 118, 215, 411
  int adcRange = 4096;        //Options: 2048, 4096, 8192, 16384

  particleSensor.setup(ledBrightness, sampleAverage, ledMode, sampleRate, pulseWidth, adcRange);  //Configure sensor with these settings

  //particleSensor.setPulseAmplitudeRed(0);    //Turn off Red LED
  //particleSensor.setPulseAmplitudeGreen(0);  //Turn off Green LED

  //Take an average of IR readings at power up
  unblockedValue = 0;
  for (byte x = 0; x < 32; x++) {
    unblockedValue += particleSensor.getIR();  //Read the IR value
  }
  unblockedValue /= 32;

  startTime = millis();
}

void loop() {
  samplesTaken++;
  long currentDelta = particleSensor.getIR() - unblockedValue;

  if (currentDelta > (long)100) {
    Serial.print("IR - ");
    int rLevel = int(particleSensor.getIR() / 1000);
    Serial.print(rLevel);
    displayMeasurement(rLevel);
    Serial.println("");
  }
  delay(100);
}
