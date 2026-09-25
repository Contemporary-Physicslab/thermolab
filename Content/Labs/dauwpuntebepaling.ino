// Arduino code made by Bente for her BEp
// used to calculate dauwpunt
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Adafruit_BME280.h>
#include <Adafruit_Sensor.h>
#include <PID_v1.h>
#include <math.h>

// -------------------- PINS --------------------

#define SENSOR1 A0
#define SENSOR2 A1

#define RELAY1 5
#define RELAY2 6

// -------------------- LCD + BME --------------------

LiquidCrystal_I2C lcd(0x27, 16, 2);
Adafruit_BME280 bme;

// -------------------- SETPOINTS --------------------

double Setpoint1 = 10.0;
double Setpoint2 = 15.0;

// -------------------- PID --------------------

double Input1;
double Output1;

double Input2;
double Output2;

// Startwaarden
double Kp = 700;
double Ki = 15;
double Kd = 0;

// REVERSE = koelen
PID pid1(&Input1, &Output1, &Setpoint1,
         Kp, Ki, Kd, REVERSE);

PID pid2(&Input2, &Output2, &Setpoint2,
         Kp, Ki, Kd, REVERSE);

// -------------------- WINDOW --------------------

const unsigned long WindowSize = 10000;

unsigned long windowStartTime1;
unsigned long windowStartTime2;

// -------------------- ADC GEMIDDELDE --------------------

int readADCaverage(int pin)
{
  long sum = 0;

  for(int i = 0; i < 20; i++)
  {
    sum += analogRead(pin);
    delay(2);
  }

  return sum / 20;
}

// -------------------- THERMISTOR --------------------

float readTemp(int pin)
{
  int adc = readADCaverage(pin);

  if(adc <= 0)
    adc = 1;

  float R = 1023.0 / adc - 1.0;
  R = 10000.0 / R;

  float temp =
      1.0 /
      (
        log(R / 10000.0) / 3950.0 +
        1.0 / (25.0 + 273.15)
      )
      - 273.15;

  return temp;
}

// -------------------- SETUP --------------------

void setup()
{
  Serial.begin(9600);

  pinMode(RELAY1, OUTPUT);
  pinMode(RELAY2, OUTPUT);

  // Active LOW relais
  digitalWrite(RELAY1, HIGH);
  digitalWrite(RELAY2, HIGH);

  lcd.init();
  lcd.backlight();

  if(!bme.begin(0x76))
  {
    lcd.clear();
    lcd.print("BME280 ERROR");

    while(1);
  }

  pid1.SetOutputLimits(0, WindowSize);
  pid2.SetOutputLimits(0, WindowSize);

  pid1.SetSampleTime(5000);
  pid2.SetSampleTime(5000);

  pid1.SetMode(AUTOMATIC);
  pid2.SetMode(AUTOMATIC);

  windowStartTime1 = millis();
  windowStartTime2 = millis();

  // CSV-header
  Serial.println("time,temp1,temp2,sp1,sp2,out1,out2,rh");
}

// -------------------- LOOP --------------------

void loop()
{
  float temp1 = readTemp(SENSOR1);
  float temp2 = readTemp(SENSOR2);

  float humidity = bme.readHumidity();

  Input1 = temp1;
  Input2 = temp2;

  pid1.Compute();
  pid2.Compute();

  bool relay1State = false;
  bool relay2State = false;

  // ---------- KANAAL 1 ----------

  if(millis() - windowStartTime1 >= WindowSize)
  {
    windowStartTime1 += WindowSize;
  }

  if((millis() - windowStartTime1) < Output1)
  {
    digitalWrite(RELAY1, LOW);
    relay1State = true;
  }
  else
  {
    digitalWrite(RELAY1, HIGH);
    relay1State = false;
  }

  // ---------- KANAAL 2 ----------

  if(millis() - windowStartTime2 >= WindowSize)
  {
    windowStartTime2 += WindowSize;
  }

  if((millis() - windowStartTime2) < Output2)
  {
    digitalWrite(RELAY2, LOW);
    relay2State = true;
  }
  else
  {
    digitalWrite(RELAY2, HIGH);
    relay2State = false;
  }

  // ---------- LCD ----------

  lcd.clear();

  lcd.setCursor(0,0);
  lcd.print("S1:");
  lcd.print(temp1,1);
  lcd.print((char)223);
  lcd.print("C");

  lcd.setCursor(10,0);
  lcd.print("RH:");
  lcd.print(humidity,0);

  lcd.setCursor(0,1);
  lcd.print("S2:");
  lcd.print(temp2,1);
  lcd.print((char)223);
  lcd.print("C");

  // ---------- CSV SERIAL OUTPUT ----------

  Serial.print(millis() / 1000.0);
  Serial.print(",");

  Serial.print(temp1,1);
  Serial.print(",");

  Serial.print(temp2,1);
  Serial.print(",");

  Serial.print(Setpoint1,1);
  Serial.print(",");

  Serial.print(Setpoint2,1);
  Serial.print(",");

  Serial.print(Output1,0);
  Serial.print(",");

  Serial.print(Output2,0);
  Serial.print(",");

  Serial.println(humidity,1);

  delay(1000);
}