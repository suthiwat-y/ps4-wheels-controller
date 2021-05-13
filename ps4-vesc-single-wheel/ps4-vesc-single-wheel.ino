/* Written by:  Suthiwat Y.
               (Machine Learning Engineer)
   Motor controls using PS4 controller
   Date May 11,2021
   ©2021 All Rights Reserved. mμ Space Corp® is a registered trademark of mμ Space and Advanced Technology Co., Ltd. 
*/

#include <PS4BT.h>
#include <usbhub.h>
#include <VescUart.h>

#ifdef dobogusinclude
#include <spi4teensy3.h>
#endif
#include <SPI.h>

//PS4
USB Usb;
BTD Btd(&Usb);
PS4BT PS4(&Btd, PAIR);

//VESC
VescUart VESC;

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
//  Serial3.begin(115200);
  VESC.setSerialPort(&Serial);
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
  VESC.setDuty(0.00);
}

void moveForward() {
  VESC.setDuty(0.1);
}

void moveLeft() {
  VESC.setDuty(-0.1);
}

void moveRight() {
  VESC.setDuty(0.1);
}

void moveBackward() {
  VESC.setDuty(-0.1);
}
