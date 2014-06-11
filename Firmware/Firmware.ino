#include <SPI.h>

#include "AS5048.h"
#include "DCMotor.h"
#include "PID_v1.h"
#include "ServoMotor.h"

//our encoder
AS5048 encoder(8);

//Define the Tuning Parameters
double consKp=1.00, consKi=0.0025, consKd=0.00025;
PID pid(consKp, consKi, consKd, DIRECT);

//our motor
byte pin_a = 3;
byte pin_b = 5;
DCMotor motor(pin_a, pin_b);

//our servo controller
ServoMotor servo(pid, encoder, motor, Serial);

void setup()
{
  Serial.begin(57600);
 
  check_encoder(encoder);  
}

uint32_t last_update = 0;

void loop()
{
  
  //look for a set point.
  while (Serial.available() > 0)
  {
    int32_t target = Serial.parseInt();

    Serial.print("New Target: ");
    Serial.print(target);
    Serial.println();

    servo.setTarget(target);
        
    if (Serial.read() == '\n')
      break;
  }
  
  /*
  for (int i=0; i<256; i++)
  {
    Serial.println(i);
    motor.setMotor(0, i);
    delay(2500);
  }
  */
  
  //servo magic go!
  uint32_t now = micros();
  if (now - last_update > 1000)
  {
    Serial.print("T: ");
    Serial.print(now);

    servo.update();    
    last_update = now;
  }
  
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
