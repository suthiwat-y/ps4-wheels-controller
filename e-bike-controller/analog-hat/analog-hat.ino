/* Written by:  Suthiwat Y.
               (Software Engineer)
   Motor control using left analog hat on PS4 joystick
   Date April 19,2021
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
    }
};

enum MovingState {
  idle,
  forward_accel,
  forward_move,
  backward_accel,
  backward_move,
  left_accel,
  left_move,
  right_accel,
  right_move
};

Wheel FRONT_LEFT(7, 22, 23, 21); //reversePin LOW for CW spin, HIGH for CCW spin
Wheel FRONT_RIGHT(6, 24, 25, 20);
Wheel BACK_LEFT(5, 26, 27, 19);
Wheel BACK_RIGHT(4, 28, 29, 18);

MovingState state = idle;
 
int SPEED = 45;
int TURNING_SPEED = 55;        

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

  FRONT_LEFT.setupPins();
  FRONT_RIGHT.setupPins();
  BACK_LEFT.setupPins();
  BACK_RIGHT.setupPins();
}

void loop() 
{
  Usb.Task();
  if (PS4.connected()) {
    Serial.print("PS4 connected");
    Serial.println(state);
    if (PS4.getButtonClick(CIRCLE)) {
        Serial.print(F("\r\nStop"));
        moveStop();
        state = idle;
    }
    if (PS4.getAnalogHat(LeftHatY) < 20) {
      if (state == idle) {
        state = forward_accel;
        moveForward();
        state = forward_move;
      } else if (state == forward_accel || state == forward_move) {
        //continue
      } else {
        moveStop();
        state = idle;
      }
    }
    else if (PS4.getAnalogHat(LeftHatY) > 235) {
      if (state == idle) {
        state = backward_accel;
        moveBackward();
        state = backward_move;
      } else if (state == backward_accel || state == backward_move) {
        //continue
      } else {
        moveStop();
        state = idle;
      }
    }
    else if (PS4.getAnalogHat(LeftHatX) < 20) {
      if (state == idle) {
        state = left_accel;
        moveLeft();
        state = left_move;
      } else if (state == left_accel || state == left_move) {
        //continue
      } else {
        moveStop();
        state = idle;
      }
    }
    else if (PS4.getAnalogHat(LeftHatX) > 235) {
      if (state == idle) {
        state = right_accel;
        moveRight();
        state = right_move;
      } else if (state == right_accel || state == right_move) {
        //continue
      } else {
        moveStop();
        state = idle;
      }
    }
    else if (PS4.getButtonClick(PS)) {
      Serial.print(F("\r\nPS"));
      PS4.disconnect();
    } else {
      moveStop();
      state = idle;
      Serial.print("else\n");
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
