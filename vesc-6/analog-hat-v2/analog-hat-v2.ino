/* Written by:  Suthiwat Y.
               (Software Engineer)
   Improved version for motor controls using left analog hat on PS4 joystick
   Support every direction other than 4 basic directions in the previous version
   Date May 13,2021
*/

#include <PS4BT.h>
#include <usbhub.h>
#include <VescUart.h>
#include <SoftwareSerial.h>

#ifdef dobogusinclude
#include <spi4teensy3.h>
#endif
#include <SPI.h>

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
  
  FRONT_LEFT.setSerialPort(&Serial1);
  FRONT_RIGHT.setSerialPort(&Serial2);
  BACK_LEFT.setSerialPort(&Serial3);
  BACK_RIGHT.setSerialPort(&Serial);
}

void loop() 
{
  Usb.Task();
  if (PS4.connected()) {
    Serial.print("PS4 connected\t");
    if (PS4.getButtonClick(PS)) {
      Serial.print("Stop\n");
      moveStop();
      PS4.disconnect();
      Serial.print("Disconnected\t");
    } else {
      float leftHatX = PS4.getAnalogHat(LeftHatX);
      float leftHatY = PS4.getAnalogHat(LeftHatY);

      float linearDuty = ((128.00 - leftHatY)/128.00) * 0.20;
      float angularDuty = ((128.00 - leftHatX)/128.00) * 0.25;
      
      float leftWheelDuty = linearDuty - angularDuty;
      float rightWheelDuty = linearDuty + angularDuty;

      Serial.print("Left Duty:\t");
      Serial.print(leftWheelDuty);
      Serial.print("\tRight Duty:\t");
      Serial.print(rightWheelDuty);
      Serial.println();

      FRONT_LEFT.setDuty(leftWheelDuty);
      FRONT_RIGHT.setDuty(rightWheelDuty);
      BACK_LEFT.setDuty(leftWheelDuty);
      BACK_RIGHT.setDuty(rightWheelDuty);
    }
  } else {
    moveStop();
  }
}

void moveStop() {
  FRONT_LEFT.setDuty(0.00);
  FRONT_RIGHT.setDuty(0.00);
  BACK_LEFT.setDuty(0.00);
  BACK_RIGHT.setDuty(0.00);
}
