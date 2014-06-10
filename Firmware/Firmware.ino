#include <SPI.h>

#include "AS5048.h"
#include "DCMotor.h"
#include "PID_v1.h"
#include "ServoMotor.h"

//our encoder
AS5048 encoder(8);

//Define the aggressive and conservative Tuning Parameters
double consKp=1.5, consKi=0.004, consKd=0.050;
double Setpoint, Input, Output;
PID pid(consKp, consKi, consKd, DIRECT);

//our motor
byte pin_a = 3;
byte pin_b = 4;
DCMotor motor(pin_a, pin_b);

//our servo controller
ServoMotor servo(pid, encoder, motor, Serial);

void setup()
{
  Serial.begin(57600);
 
  check_encoder(encoder);  
}

void loop()
{   
  //look for a set point.
  while (Serial.available() > 0)
  {
    int32_t target = Serial.parseInt();
    target = constrain(target, 0, 16383);

    servo.setTarget(target);
        
    if (Serial.read() == '\n')
      break;
  }
  
  //servo magic go!
  servo.update();
}

void check_encoder(AS5048 enc)
{
  word dat = enc.getDiagnostics();
  byte agc = dat & 0xFF;
  byte ocf = dat >> 8 & 0x01;
  byte cof = dat >> 9 & 0x01;
  byte comp_hi = dat >> 10 & 0x01;
  byte comp_low = dat >> 11 & 0x01;
  
  Serial.print("Automatic Gain Control: ");
  Serial.println(agc);
  Serial.print("OCF: ");
  Serial.println(ocf);
  Serial.print("COF: ");
  Serial.println(cof);
  Serial.print("Comp High: ");
  Serial.println(comp_hi);
  Serial.print("Comp Low: ");
  Serial.println(comp_low);
}
