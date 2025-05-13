#include <SD.h>
#include <SPI.h>
#include <PulseSensorPlayground.h>
#include <DHT.h>
#include <LiquidCrystal_I2C.h>
#include <RTClib.h>

const int chipSelect = 10;
const int buzzerPin = 9;
const int ledPin = 8;
const int heartbeatLedPin = 6;
const int PulseWire = A0;
const int tempThreshold = 40;
const float joulePerStep = 0.25;
const int xPin = A1;
const int yPin = A2;
const int zPin = A3;

PulseSensorPlayground pulseSensor;
DHT dht(7, DHT11);
LiquidCrystal_I2C lcd(0x27, 16, 2);

int stepCount = 0;
unsigned long lastStepTime = 0;
const unsigned long debounceTime = 500;
float prevFilteredX, prevFilteredY, prevFilteredZ;
float threshold = 0.08;
const float alpha = 0.5;

RTC_DS3231 rtc;
byte heart[] = {
 0b00000,
 0b01010,
 0b11111,
 0b11111,
 0b01110,
 0b00100,
 0b00000,
 0b00000};
unsigned long lastLogTime = 0;

void setup() {
 Serial.begin(9600);
 pinMode(buzzerPin, OUTPUT);
 digitalWrite(buzzerPin, LOW);
 pinMode(heartbeatLedPin, OUTPUT);

 delay(1000);
 if (!rtc.begin()) {
  Serial.println("Couldn't find RTC");
 }
 rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));

 pinMode(chipSelect, OUTPUT);
 digitalWrite(chipSelect, HIGH);
 if (!SD.begin(chipSelect)) {
  Serial.println("SD CARD ERROR");
 } else {
  Serial.println("SD Card initialized successfully!");
 }

 File dataFile = SD.open("health.csv", FILE_WRITE);
 if (dataFile && dataFile.size() == 0) {
  dataFile.println("Timestamp, BPM, TemperatureC, Steps, joule");
  dataFile.close();
 }

pulseSensor.analogInput(PulseWire);
 pulseSensor.setThreshold(550);
 pulseSensor.begin();
 lcd.createChar(0, heart);
 dht.begin();

 pinMode(ledPin, OUTPUT);
 digitalWrite(ledPin, LOW);

 lcd.init();
 lcd.backlight();
 lcd.setCursor(0, 0);
 lcd.print("HEALTH MONITOR");
 delay(2000);
 lcd.clear();

 float initX = (analogRead(xPin) / 1023.0) * 3.3;
 float initY = (analogRead(yPin) / 1023.0) * 3.3;
 float initZ = (analogRead(zPin) / 1023.0) * 3.3;
 prevFilteredX = initX - 1.65;
 prevFilteredY = initY - 1.65;
 prevFilteredZ = initZ - 1.65;
}

void loop() {
 static unsigned long lastBlinkTime = 0;
 static bool heartVisible = false;
 static int bpm = 0;

 float temperature = dht.readTemperature();
 bpm = pulseSensor.getBeatsPerMinute();

 if (!isnan(temperature)) {
  digitalWrite(buzzerPin, (temperature > tempThreshold) ? HIGH : LOW);}
 step();
 float joule = stepCount * joulePerStep;

 int blinkTime;
 if (bpm > 0 && bpm < 50) blinkTime = 1000;
 else if (bpm < 100) blinkTime = 600;
 else if (bpm < 150) blinkTime = 400;
 else blinkTime = 200;

 if (bpm > 0) {
  digitalWrite(heartbeatLedPin, HIGH);
  delay(blinkTime / 2);
  digitalWrite(heartbeatLedPin, LOW);
  delay(blinkTime / 2);
 } else {
  digitalWrite(heartbeatLedPin, LOW);}
   if (millis() - lastBlinkTime > 400) {
  heartVisible = !heartVisible;
  lastBlinkTime = millis();}
 lcd.clear();
 lcd.setCursor(0, 0);
 lcd.print("B:");
 lcd.print(bpm);
 lcd.print(" ");

 if (bpm > 0 && heartVisible) {
  lcd.write((uint8_t)0);
 } else {
  lcd.print(" ");
 }

 lcd.setCursor(9, 0);
 lcd.print(" T:");
 lcd.print((int)temperature);
 lcd.print((char)223);
 lcd.print("C");
 lcd.print(" ");
 delay(10);

  if (millis() - lastBlinkTime > 400) {
 lcd.setCursor(0, 1);
 lcd.print("Step:");
 lcd.print(stepCount);
 lcd.print(" ");

 lcd.setCursor(10, 1);
 lcd.print(joule, 1);
 lcd.print("J");

 if (millis() - lastLogTime >= 5000) {
  lastLogTime = millis();
  logData(bpm, temperature, stepCount, joule);
  }
 delay(200);
 }
}