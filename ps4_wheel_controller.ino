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
                               
// FL
volatile long half_revolutions18; // to make half_revolutions can declare in ISRs( Interrupt Service Routines )
unsigned int               rpm18;          
unsigned long          timeold18;

// FR
volatile long half_revolutions19; // to make half_revolutions can declare in ISRs( Interrupt Service Routines )
unsigned int               rpm19;          
unsigned long          timeold19;

// BL
volatile long half_revolutions20; // to make half_revolutions can declare in ISRs( Interrupt Service Routines )
unsigned int               rpm20;          
unsigned long          timeold20;

// BR
volatile long half_revolutions21; // to make half_revolutions can declare in ISRs( Interrupt Service Routines )
unsigned int               rpm21;          
unsigned long          timeold21;

int require_rotation = 20; // accuracy for hall sensor
int t_rpm; // total rpm 
float velocity; // m/s
float d = 0.127; //diameter of wheel  5 in to 0.0254 *5 m

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
  attachInterrupt(digitalPinToInterrupt(FRONT_LEFT.hallPin), magnet_detect_FL, RISING);
  attachInterrupt(digitalPinToInterrupt(FRONT_RIGHT.hallPin), magnet_detect_FR, RISING);
  attachInterrupt(digitalPinToInterrupt(BACK_LEFT.hallPin), magnet_detect_BL, RISING);
  attachInterrupt(digitalPinToInterrupt(BACK_RIGHT.hallPin ), magnet_detect_BR, RISING);
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
        total_rpm();
    }
    if (PS4.getButtonClick(DOWN)) {
        Serial.print(F("\r\nForward"));
        moveBackward();
        total_rpm();
    }
    if (PS4.getButtonClick(LEFT)) {
        Serial.print(F("\r\nForward"));
        moveLeft();
        total_rpm();
    }
    if (PS4.getButtonClick(RIGHT)) {
        Serial.print(F("\r\nForward"));
        moveRight();
        total_rpm();
    }
    if (PS4.getButtonClick(CIRCLE)) {
        Serial.print(F("\r\nStop"));
        moveStop();
        t_rpm=0;
        velocity=0;
        Serial.println("velocity = ");
        Serial.println(velocity,DEC);
    }
    if (PS4.getButtonClick(PS)) {
        PS4.disconnect();
        Serial.println("PS4 disconnected");
    }
  }
}

 void magnet_detect_FL()//This function is called whenever a magnet/interrupt is detected by the arduino
{
   half_revolutions18++;
}

 void magnet_detect_FR()//This function is called whenever a magnet/interrupt is detected by the arduino
{
   half_revolutions19++;
}

void magnet_detect_BL()//This function is called whenever a magnet/interrupt is detected by the arduino
{
   half_revolutions20++;
}

void magnet_detect_BR()//This function is called whenever a magnet/interrupt is detected by the arduino
{
   half_revolutions21++;
}

void total_rpm()
{
   if (half_revolutions18 >= require_rotation ) //Update RPM every 20 counts, increase this for better RPM resolution,
   { 
     rpm18 = 30*1000/(millis() - timeold18)*half_revolutions18;
     timeold18 = millis();
     half_revolutions18 = 0;
   }

   if (half_revolutions19 >= require_rotation ) //Update RPM every 20 counts, increase this for better RPM resolution,
   { 
     rpm19 = 30*1000/(millis() - timeold19)*half_revolutions19;
     timeold19 = millis();
     half_revolutions19 = 0;
   }

   if (half_revolutions20 >= require_rotation ) //Update RPM every 20 counts, increase this for better RPM resolution,
   { 
     rpm20 = 30*1000/(millis() - timeold20)*half_revolutions20;
     timeold20 = millis();
     half_revolutions20 = 0;
   }

   if (half_revolutions21 >= require_rotation) //Update RPM every 20 counts, increase this for better RPM resolution,
   { 
     rpm21 = 30*1000/(millis() - timeold21)*half_revolutions21;
     timeold21 = millis();
     half_revolutions21 = 0;
   }
      
  t_rpm=(rpm18+rpm19+rpm20+rpm21)/4;

  velocity = (PI*d*(t_rpm))/60; //m/s

  Serial.println("velocity = ");
  Serial.println(velocity,DEC);
  rpm18 = 0;
  rpm19 = 0;
  rpm20 =0;
  rpm21 =0;
  t_rpm=0;
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
