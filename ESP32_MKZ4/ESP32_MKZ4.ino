/*
 * Copyright (c) 2015, Majenko Technologies All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * * Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer.
 *
 * * Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * * Neither the name of Majenko Technologies nor the names of its
 *   contributors may be used to endorse or promote products derived from
 *   this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

// Fully customized by ts5h, 2022
#include <WebServer.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <Wire.h>
#include <ESP32Servo.h>

/* Set these to your desired credentials */
const char *SSID = "MKZ4";
const char *PASSWORD = "";

WebServer server(80);
WebServer server_8080(8080);

/* Set I2C library*/
const int SDA_PIN = 4;
const int SCL_PIN = 14;

const int DRV8830_ADDR = 0x64;
const int CONTROL = 0x00;
const int FAULT = 0x01;

const int COASTING = 0x00;
const int FORWARD = 0x01;
const int REVERSE = 0x02;
const int BREAK = 0x03;

const int COMMAND_START = 0;
const int COMMAND_STOP = 1;
const int COMMAND_BACK = 2;

/* Set SG90 Servo motor */
// Degrees / You have to adjust your servo for each individual
const int SERVO_PIN = 16;
const int SERVO_CENTER = 90;
const int SERVO_DEGREE = 16;

Servo servo;
char state = COMMAND_STOP;

const int LED_PIN = 12;
#define LED_H (digitalWrite(LED_PIN, HIGH))
#define LED_L (digitalWrite(LED_PIN, LOW))


void setup() {
  pinMode(LED_PIN, OUTPUT);
  delay(1000);

  // Initialize motors
  Wire.begin(SDA_PIN, SCL_PIN);
  stop();

  servo.attach(SERVO_PIN);
  servo.write(SERVO_CENTER);

  /* 
  Set & start WiFi
  You can remove the password parameter if you want the AP to be open.
  */
  Serial.begin(115200);
  Serial.println("Configuring access point...");
  delay(20);

  /* Open http://192.168.4.1 in your web browser */
  WiFi.softAP(SSID, PASSWORD);

  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);

  server.on("/", handle_root);
  server_8080.on("/stop", handle_stop);
  server_8080.on("/move", handle_move);

  server.begin();
  server_8080.begin();
  Serial.println("HTTP server started");

  delay(100);
  LED_H;
}

void loop() {
  server.handleClient();
  server_8080.handleClient();
  delayMicroseconds(100);
}


// Handlers
void handle_root() {
  // See form.ino
  extern String form;
  server.send(200, "text/html", form);
}

void handle_stop() {
  LED_L;
  stop();
  servo.write(SERVO_CENTER);
  server_8080.send(200, "text/html", "");
  LED_H;
}

float oldY = 0;

void handle_move() {
  if (!server_8080.hasArg("x") || !server_8080.hasArg("y")) return;

  float x = server_8080.arg("x").toFloat();
  float y = server_8080.arg("y").toFloat();

  LED_L;

  // Left / Right
  if (x == 0) {
    servo.write(SERVO_CENTER);
  } else {
    int degree = SERVO_CENTER + round(SERVO_DEGREE * x);
    servo.write(degree);
  }

  // Forward / Back
  if (y == 0) {
    stop();
  } else if (y != oldY) {
    int speed = floor(100 * y);
    y > 0 ? drive(speed) : back(speed * -1);
  }

  oldY = y;

  server_8080.send(200, "text/html", "");
  LED_H;
}

// Motor functions
void stop() {
  stop_motor();
  state = COMMAND_STOP;
}

void drive(int speed) {
  if (state == COMMAND_BACK) stop_motor();
  start_motor(speed);
  state = COMMAND_START;
}

void back(int speed) {
  if (state == COMMAND_START) stop_motor();
  reverse_motor(speed);
  state = COMMAND_BACK;
}

void stop_motor() {
  motor_func(0x00 << 2 | COASTING);
  delay(10);
  motor_func(0x00 << 2 | BREAK);
  delay(10);
}

void start_motor(int speed) {
  char duty = speed_to_duty(speed);
  motor_func(duty << 2 | FORWARD);
  delay(10);
}

void reverse_motor(int speed) {
  char duty = speed_to_duty(speed);
  motor_func(duty << 2 | REVERSE);
  delay(10);
}

// Convert to duty: 0x06-0x20 (6-32) / 0.48V-2.52V
char speed_to_duty(int speed) {
  return (char) map(speed, 0, 100, 0x06, 0x20);
}

void motor_func(char duty) {
  Wire.beginTransmission(DRV8830_ADDR);
  Wire.write(CONTROL);
  Wire.write(duty);
  Wire.endTransmission();
}
