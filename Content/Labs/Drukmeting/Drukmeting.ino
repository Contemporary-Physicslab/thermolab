// Script to read the pressure and temperature sensor.
// Functionality:
// single press button B: spit out data
// three second press button B: clear dataset

#include <SPI.h>
#include <SD.h>
#include <OneWire.h>
#include <DallasTemperature.h>

const int druksensor = A1;
const int CS_PIN = SDCARD_SS_PIN;

const int BUTTON_PIN =    2;
const int TEMP_PIN =      3;
const int POWER_EN_PIN =  4;
const int BUZZER_PIN =    5;

const char* FNAME = "DATA.CSV";

// Variables
int value;
float temperature;
uint32_t now = 0;
uint32_t buttonStart = 0;
bool state_button;
static uint32_t last = 0;
const uint32_t SAMPLE_MS = 100;
const uint32_t LONG_PRESS_MS = 3000;   // 3 seconden
int sensorresolution = 10; // 9, 10, 11, or 12 bits

OneWire oneWire(TEMP_PIN);
DallasTemperature sensors(&oneWire);

const uint32_t ON_TIME_MS = 4UL * 60UL * 60UL * 1000UL;
//const uint32_t ON_TIME_MS = 10UL * 1000UL; // 10 seconden voor testen
uint32_t startTime;

void setup() {
  Serial.begin(115200);

  // PIN SETUP
  pinMode(POWER_EN_PIN, OUTPUT);
  digitalWrite(POWER_EN_PIN, HIGH);

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);

  pinMode(druksensor, INPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  startTime = millis();

  sensors.begin();
  sensors.setResolution(sensorresolution);

  delay(800);

  // CHECK SD
  int attempts = 0;

  while (!SD.begin(CS_PIN)) {
    if (++attempts >= 5) fatalSd();
    blink(1, 50, 500);
  }

  // Read file
  File f = SD.open(FNAME, FILE_WRITE);

  if (!f) {
    fatalFile();
  }

  if (f.size() == 0) {
    f.println("time_ms,value,temperature_C");
  }

  f.close();

  blink(5, 60, 300);
  beep(2000, 100, 3, 200);
  delay(200);
  beep(2000, 500, 1, 200);
}

void loop() {
  CHECK_BUTTON();
  READ_SENSORS();
  READ_CSV();
  PYTHON_READOUT();
}

void PYTHON_READOUT(){  
  if (Serial.available()) {
    char command = Serial.read();

    if (command == 'D') {
     printFileSerial();
    }
  }
}

void CHECK_BUTTON() {
  state_button = digitalRead(BUTTON_PIN);
  //button not pressed
  if (state_button == HIGH) return;

  //button pressed
  unsigned long start = millis();
  unsigned long timer = 0;
  while (digitalRead(BUTTON_PIN) == LOW && timer < LONG_PRESS_MS){
    timer = millis() - start;
    delay(10);  
  }
  
  // Long press, clear card
  if (timer >= LONG_PRESS_MS){
    beep(1000, 1000, 1, 200);
    clearSdCard();

    // Turn off power
    digitalWrite(POWER_EN_PIN, LOW);
  }
  // short press, print read out
  else {
    beep(2000, 100, 1, 200);
    printFileSerial();
  }  
}

void READ_SENSORS(){

  now = millis();

  if (now - last > SAMPLE_MS) {    
    last = now;
    value = analogRead(druksensor);
    sensors.requestTemperatures();
    temperature = sensors.getTempCByIndex(0);

    File f = SD.open(FNAME, FILE_WRITE);

    if (!f) {
      fatalFile();
    }

    //Direct uitspugen
    // Serial.println(String(now) + "," + String(value) + "," + String(temperature, 2));

    //WRITE TO FILE
    f.println(String(now) + "," + String(value) + "," + String(temperature, 2));
    f.close();

    // TESTING ONLY:
    // digitalWrite(LED_BUILTIN, HIGH);
    // delay(10);
    // digitalWrite(LED_BUILTIN, LOW);
  }
}


void beep(int frequency, int duration, int aantal, int pauze) {
  for (int i = 0; i < aantal; i++) {
    tone(BUZZER_PIN, frequency);
    delay(duration);
    noTone(BUZZER_PIN);

    if (i < aantal - 1) {
      delay(pauze);
    }
  }
}

void blink(int times, int onMs, int pauseMs) {
  for (int i = 0; i < 2*times; i++) {
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
    delay(onMs);
  }
  delay(pauseMs);
}

void fatalSd() {
  while (1) {
    Serial.println("fatalSd");
    blink(3, 100, 600);
  }
}

void fatalFile() {
  while (1) {
    Serial.println("fatalFile");
    blink(2, 250, 800);
  }
}

void printFileSerial() {
  File f = SD.open(FNAME, FILE_READ);

  if (!f) {
    Serial.println("ERROR: DATA.CSV cannot be opened");
    return;
  }

  Serial.println("BEGIN_DATA");

  while (f.available()) {
    Serial.write(f.read());
  }

  f.close();

  Serial.println("END_DATA");
}

void clearSdCard() {
  Serial.println("whipe DATA.CSV...");
  if (SD.exists(FNAME)) {
    if (!SD.remove(FNAME)) {
      Serial.println("failed to whipe DATA.CSV");
      beep(6000, 100, 3, 200);
      return;
    }
  }

  File f = SD.open(FNAME, FILE_WRITE);

  if (!f) {
    fatalFile();
  }

  f.println("time_ms,value,temperature_C");
  f.close();

  Serial.println("Made a new file");

  // bevestiging
  beep(3000, 100, 3, 150);
}

void READ_CSV() {
  if (millis() - startTime >= ON_TIME_MS) {
    printFileSerial();
    digitalWrite(POWER_EN_PIN, LOW); //Uitschakelen
    while (1) {
      delay(1000);
    }
  }
}