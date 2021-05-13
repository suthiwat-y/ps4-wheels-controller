/* Written by:  Suthiwat Y.
               (Machine Learning Engineer)
   Motor controls using PS4 controller
   Date May 11,2021
   ©2021 All Rights Reserved. mμ Space Corp® is a registered trademark of mμ Space and Advanced Technology Co., Ltd. 
*/

#include <PS4BT.h>
#include <usbhub.h>
#include <VescUart.h>
#include <SoftwareSerial.h>

#ifdef dobogusinclude
#include <spi4teensy3.h>
#endif
#include <SPI.h>

//SoftwareSerial
//SoftwareSerial Serial4(2, 3); // RX, TX/

//PS4
USB Usb;
BTD Btd(&Usb);
PS4BT PS4(&Btd, PAIR);

//VESC
VescUart FRONT_LEFT;
VescUart FRONT_RIGHT;
VescUart BACK_LEFT;
VescUart BACK_RIGHT;

void setup() {
  Serial.begin(115200);
#if !defined(__MIPSEL__)
  while (!Serial);
#endif
  if (Usb.Init() == -1) {
    Serial.print(F("\r\nOSC did not start"));
    while (1);
  }
  Serial.print(F("\r\nPS4 Bluetooth Library Started"));

  //VescUart
  Serial1.begin(115200);
  Serial2.begin(115200);
  Serial3.begin(115200);
//  Serial4.begin(115200);/
  
  FRONT_LEFT.setSerialPort(&Serial1);
  FRONT_RIGHT.setSerialPort(&Serial2);
  BACK_LEFT.setSerialPort(&Serial3);
  BACK_RIGHT.setSerialPort(&Serial);//
}

void loop() 
{
  Usb.Task();
  if (PS4.connected()) {
    Serial.print("PS4 connected\t");
    if (PS4.getAnalogHat(LeftHatY) < 20) {
      Serial.print("Forward\n");
      moveForward();
    }
    else if (PS4.getAnalogHat(LeftHatY) > 235) {
      Serial.print("Backward\n");
      moveBackward();
    }
    else if (PS4.getAnalogHat(LeftHatX) < 20) {
      Serial.print("Left\n");
      moveLeft();
    }
    else if (PS4.getAnalogHat(LeftHatX) > 235) {
      Serial.print("Right\n");
      moveRight();
    }
    else if (PS4.getButtonClick(PS)) {
      
      Serial.print("PS Pressed\t");
      Serial.print("Stop\n");
      moveStop();
      PS4.disconnect();
    } else {
      Serial.print("Stop\n");
      moveStop();
    }
  }
}

void moveStop() {
  FRONT_LEFT.setDuty(0.00);
  FRONT_RIGHT.setDuty(0.00);
  BACK_LEFT.setDuty(0.00);
  BACK_RIGHT.setDuty(0.00);
}

void moveForward() {
  FRONT_LEFT.setDuty(0.20);
  FRONT_RIGHT.setDuty(0.20);
  BACK_LEFT.setDuty(0.20);
  BACK_RIGHT.setDuty(0.20);
}

void moveLeft() {
  FRONT_LEFT.setDuty(-0.25);
  FRONT_RIGHT.setDuty(0.25);
  BACK_LEFT.setDuty(-0.25);
  BACK_RIGHT.setDuty(0.25);
}

void moveRight() {
  FRONT_LEFT.setDuty(0.25);
  FRONT_RIGHT.setDuty(-0.25);
  BACK_LEFT.setDuty(0.25);
  BACK_RIGHT.setDuty(-0.25);
}

void moveBackward() {
  FRONT_LEFT.setDuty(-0.20);
  FRONT_RIGHT.setDuty(-0.20);
  BACK_LEFT.setDuty(-0.20);
  BACK_RIGHT.setDuty(-0.20);
}
