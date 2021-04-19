/* Written by:  Suthiwat Y.
               (Machine Learning Engineer)
   Motor controls using PS4 controller
   Date April 19,2021
   ©2021 All Rights Reserved. mμ Space Corp® is a registered trademark of mμ Space and Advanced Technology Co., Ltd. 
*/

#include <PS4BT.h>
#include <usbhub.h>

#ifdef dobogusinclude
#include <spi4teensy3.h>
#endif
#include <SPI.h>

USB Usb;
BTD Btd(&Usb);
PS4BT PS4(&Btd, PAIR);

char type; 

class Wheel {
  public:
  int spinPin;
  int breakPin;
  int reversePin;
  int hallPin;

  Wheel(int s, int b, int r, int h) {
    spinPin = s;
    breakPin = b;
    reversePin = r;
    hallPin = h;
  }

  void setupPins() {
    pinMode(spinPin, OUTPUT);
    pinMode(breakPin, OUTPUT);
    pinMode(reversePin, OUTPUT);
    pinMode(hallPin, INPUT_PULLUP);
  }

  void cw(int spd) {
    analogWrite(spinPin, 0);
    digitalWrite(reversePin, HIGH);
    delay(100);
    digitalWrite(breakPin, HIGH);
    delay(100);
    
    for (int x = 0; x < spd; x++) {
      analogWrite(spinPin, x);
    }
    analogWrite(spinPin, spd);
  }

    void ccw(int spd) {
      analogWrite(spinPin, 0);
      digitalWrite(reversePin, LOW);
      delay(100);
      digitalWrite(breakPin, HIGH);
      delay(100);
  
      for (int x = 0; x < spd; x++) {
        analogWrite(spinPin, x);
      }
      analogWrite(spinPin, spd);
    }

    void spinStop() {
      digitalWrite(breakPin, LOW);
      digitalWrite(reversePin, LOW);
      analogWrite(spinPin, 0);
      Serial.println("h1");
    }
};

Wheel FRONT_LEFT(7, 25, 29, 21); //reversePin LOW for CW spin, HIGH for CCW spin
Wheel FRONT_RIGHT(6, 24, 28, 20);
Wheel BACK_LEFT(5, 23, 27, 19);
Wheel BACK_RIGHT(4, 22, 26, 18);
 
int SPEED = 45;
int TURNING_SPEED = 55;        

void setup() {
  Serial.begin(115200);
#if !defined(__MIPSEL__)
  while (!Serial); // Wait for serial port to connect 
#endif
  if (Usb.Init() == -1) {
    Serial.print(F("\r\nOSC did not start"));
    while (1);
  }
  Serial.print(F("\r\nPS4 Bluetooth Library Started"));

  FRONT_LEFT.setupPins();
  FRONT_RIGHT.setupPins();
  BACK_LEFT.setupPins();
  BACK_RIGHT.setupPins();
}

void loop() 
{
  Usb.Task();
  type = Serial.read(); 
  if (PS4.connected()) {
    Serial.print("PS4 connected");
    if (PS4.getButtonClick(UP)) {
        Serial.print(F("\r\nForward"));
        moveForward();
    }
    if (PS4.getButtonClick(DOWN)) {
        Serial.print(F("\r\nBackward"));
        moveBackward();
    }
    if (PS4.getButtonClick(LEFT)) {
        Serial.print(F("\r\nLeft"));
        moveLeft();
    }
    if (PS4.getButtonClick(RIGHT)) {
        Serial.print(F("\r\nRight"));
        moveRight();
    }
    if (PS4.getButtonClick(CIRCLE)) {
        Serial.print(F("\r\nStop"));
        moveStop();
    }
    if (PS4.getButtonClick(PS)) {
        PS4.disconnect();
        Serial.println("PS4 disconnected");
    }
  }
}

void moveStop() {
  FRONT_LEFT.spinStop();
  FRONT_RIGHT.spinStop();
  BACK_LEFT.spinStop();
  BACK_RIGHT.spinStop();
}

void moveForward() {
  moveStop();
  
  delay(100);

  BACK_LEFT.ccw(SPEED);
  BACK_RIGHT.cw(SPEED);
  FRONT_LEFT.cw(SPEED);
  FRONT_RIGHT.cw(SPEED);
}

void moveLeft() {
  moveStop();

  delay(100);

  BACK_LEFT.cw(TURNING_SPEED);
  BACK_RIGHT.cw(TURNING_SPEED);
  FRONT_LEFT.ccw(TURNING_SPEED);
  FRONT_RIGHT.cw(TURNING_SPEED);
}

void moveRight() {
  moveStop();

  delay(100);

  BACK_LEFT.ccw(TURNING_SPEED);
  BACK_RIGHT.ccw(TURNING_SPEED);
  FRONT_LEFT.cw(TURNING_SPEED);
  FRONT_RIGHT.ccw(TURNING_SPEED);
}

void moveBackward() {
  moveStop();

  delay(100);
  
  BACK_LEFT.cw(SPEED);
  BACK_RIGHT.ccw(SPEED);
  FRONT_LEFT.ccw(SPEED);
  FRONT_RIGHT.ccw(SPEED);
}
