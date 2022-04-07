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

/* Create a WiFi access point and provide a web server on it. */
#include <WebServer.h>
#include <WiFi.h>
#include <WiFiClient.h>

/* Set these to your desired credentials. */
const char *ssid = "MKZ4";
const char *password = "";

WebServer server(80);
WebServer server_8080(8080);

/* Set I2C library*/
#include <Wire.h>
#define ADDR1 0x64

#define COMMAND_START 0
#define COMMAND_STOP 1
#define COMMAND_BACK 2
#define FORWARD 0x01
#define REVERSE 0x02

// For SG90 Servo motor
#include <ESP32Servo.h>

// Degrees / You have to adjust your servo for each individual
#define SERVO_CENTER 90
#define SERVO_LEFT 75
#define SERVO_RIGHT 105

Servo myServo;
const int SERVO_PIN = 16;
char state = COMMAND_STOP;

#define LED_PIN 2
#define LED_H (digitalWrite(2, HIGH))
#define LED_L (digitalWrite(2, LOW))

/* Just a little test message.  Go to http://192.168.4.1 in a web browser
 * connected to this access point to see it.
 */
void setup() {
  delay(1000);
  Serial.begin(115200);
  Serial.println("Configuring access point...");

  Wire.begin(4, 14);
  delay(40);

  /* You can remove the password parameter if you want the AP to be open. */
  WiFi.softAP(ssid, password);

  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);

  server.on("/", handle_root);
  server_8080.on("/stop", handle_stop);
  server_8080.on("/move", handle_move);
  // server_8080.on("/forward", handle_forward);
  // server_8080.on("/back", handle_back);
  // server_8080.on("/left", handle_left);
  // server_8080.on("/right", handle_right);
  // server_8080.on("/leftforward", handle_forward_left);
  // server_8080.on("/rightforward", handle_forward_right);
  // server_8080.on("/leftback", handle_back_left);
  // server_8080.on("/rightback", handle_back_right);

  server.begin();
  server_8080.begin();
  Serial.println("HTTP server started");

  myServo.attach(SERVO_PIN);

  pinMode(LED_PIN, OUTPUT);
  LED_H;
  delay(100);
}

void loop() {
  server.handleClient();
  server_8080.handleClient();
}


// Control directions
void handle_root() {
  extern String form;
  server.send(200, "text/html", form);
}

void handle_stop() {
  Serial.println("Stop");
  LED_L;
  stop_motor();
  state = COMMAND_STOP;
  LED_H;
  server_8080.send(200, "text/html", "");
}

void handle_move() {
  // TODO: Get Args
  String x = server_8080.arg("x");
  String y = server_8080.arg("y");
  Serial.print(x);
  Serial.print(", ");
  Serial.println(y);

  server_8080.send(200, "text/html", "");
}

void handle_forward() {
  Serial.println("Forward");
  drive();
  servo_control(SERVO_CENTER);
  server_8080.send(200, "text/html", "");
}

void handle_back() {
  Serial.println("Back");
  back();
  servo_control(SERVO_CENTER);
  server_8080.send(200, "text/html", "");
}

void handle_left() {
  Serial.println("Left");
  servo_control(SERVO_LEFT);
  server_8080.send(200, "text/html", "");
}

void handle_right() {
  Serial.println("Right");
  servo_control(SERVO_RIGHT);
  server_8080.send(200, "text/html", "");
}

void handle_forward_left() {
  Serial.println("Forward Left");
  drive();
  servo_control(SERVO_LEFT);
  server_8080.send(200, "text/html", "");
}

void handle_forward_right() {
  Serial.println("Forward Right");
  drive();
  servo_control(SERVO_RIGHT);
  server_8080.send(200, "text/html", "");
}

void handle_back_left() {
  Serial.println("Back Left");
  back();
  servo_control(SERVO_LEFT);
  server_8080.send(200, "text/html", "");
}

void handle_back_right() {
  Serial.println("Back Right");
  back();
  servo_control(SERVO_RIGHT);
  server_8080.send(200, "text/html", "");
}

void drive() {
  switch (state) {
    case COMMAND_BACK:
      stop_motor();
      delay(10);
      start_motor();
      break;

    case COMMAND_STOP:
      start_motor();
      break;
  }

  state = COMMAND_START;
}

void back() {
  switch (state) {
    case COMMAND_START:
      stop_motor();
      delay(10);
      reverse_motor();
      break;

    case COMMAND_STOP:
      reverse_motor();
      break;
  }

  state = COMMAND_BACK;
}

void start_motor() {
  char volt = 0x20;

  for (int i = 0; i < 5; i++) {
    volt = volt + ((0x40) * i);
    volt = volt | FORWARD;
    motor_func(ADDR1, volt);
    delay(10);
  }
}

void reverse_motor() {
  char volt = 0x20;

  for (int i = 0; i < 5; i++) {
    volt = volt + ((0x40) * i);
    volt = volt | REVERSE;
    motor_func(ADDR1, volt);
    delay(10);
  }
}

void stop_motor() {
  motor_func(ADDR1, 0x18);
  delay(10);
  motor_func(ADDR1, 0x1B);
  delay(10);
}

void motor_func(char add, char duty) {
  Wire.beginTransmission(add);
  Wire.write(0x00);
  Wire.write(duty);
  Wire.endTransmission();
}

void servo_control(int angle) {
  // int microsec = (5 * (angle + offset)) + 1000;

  // for (int i = 0; i < 20; i++) {
  //   digitalWrite(SERVO_PIN, HIGH);
  //   delayMicroseconds(microsec);

  //   digitalWrite(SERVO_PIN, LOW);
  //   delayMicroseconds(10000 - microsec);
  // }

  LED_L;
  myServo.write(angle);
  LED_H;
}
