// Motor test
// Reference: https://spiceman.jp/arduino-drv8830-program/
#include <Wire.h>

/* Set I2C library*/
#define DRV8830_ADDR 0x64
#define CONTROL 0x00
#define FAULT 0x01

#define FORWARD 0x01
#define REVERSE 0x02
#define BREAK 0x03

void setup() {
  Serial.begin(115200);

  // I2C
  Wire.begin(4, 14);

  // CONTROL clear
  Wire.beginTransmission(DRV8830_ADDR);
  Wire.write(CONTROL);
  Wire.write(0x00 << 2 | 0x00);
  Wire.endTransmission();

  // FAULT clear
  Wire.beginTransmission(DRV8830_ADDR);
  Wire.write(FAULT);
  Wire.write(0x80);
  Wire.endTransmission();

  delay(1000);
}

void loop() {
  // forward 0.48V
  Wire.beginTransmission(DRV8830_ADDR);
  Wire.write(CONTROL);
  Wire.write(0x06 << 2 | FORWARD);
  Wire.endTransmission();

  delay(10);

  // forward 0.96V
  Wire.beginTransmission(DRV8830_ADDR);
  Wire.write(CONTROL);
  Wire.write(0x0C << 2 | FORWARD);
  Wire.endTransmission();

  delay(10);

  // forward 1.45V
  Wire.beginTransmission(DRV8830_ADDR);
  Wire.write(CONTROL);
  Wire.write(0x12 << 2 | FORWARD);
  Wire.endTransmission();
 
  delay(1000);

  // coasting
  Wire.beginTransmission(DRV8830_ADDR);
  Wire.write(CONTROL);
  Wire.write(0x00 << 2 | 0x00);
  Wire.endTransmission();

  delay(10);
  
  // break
  Wire.beginTransmission(DRV8830_ADDR);
  Wire.write(CONTROL);
  Wire.write(0x00 << 2 | BREAK);
  Wire.endTransmission();

  delay(10);
    
  // coasting
  Wire.beginTransmission(DRV8830_ADDR);
  Wire.write(CONTROL);
  Wire.write(0x00 << 2 | 0x00);
  Wire.endTransmission();

  delay(10);

  // reverse 0.48V
  Wire.beginTransmission(DRV8830_ADDR);
  Wire.write(CONTROL);
  Wire.write(0x06 << 2 | REVERSE);
  Wire.endTransmission();

  delay(10);
  
  // reverse 0.96V
  Wire.beginTransmission(DRV8830_ADDR);
  Wire.write(CONTROL);
  Wire.write(0x0C << 2 | REVERSE);
  Wire.endTransmission();

  delay(10);

  // reverse 1.45V
  Wire.beginTransmission(DRV8830_ADDR);
  Wire.write(CONTROL);
  Wire.write(0x12 << 2 | REVERSE);
  Wire.endTransmission();
 
  delay(1000);

  // coasting
  Wire.beginTransmission(DRV8830_ADDR);
  Wire.write(CONTROL);
  Wire.write(0x00 << 2 | 0x00);
  Wire.endTransmission();

  delay(10);
  
  // break
  Wire.beginTransmission(DRV8830_ADDR);
  Wire.write(CONTROL);
  Wire.write(0x00 << 2 | BREAK);
  Wire.endTransmission();
  
  delay(10);
}