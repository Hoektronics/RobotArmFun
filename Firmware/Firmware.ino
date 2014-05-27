#include <SPI.h>
#include "AS5048.h"

AS5048 Sen1(8);

void setup()
{
  Serial.begin(57600);
 
  check_encoder(Sen1);
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

void loop()
{
  int angle = Sen1.getAngle();
  float dec_angle = ((float)angle / 16383.0) * 360.0;
  Serial.print("A: ");
  Serial.print(dec_angle, 2);  
  
  Serial.print(" D: ");
  Serial.print(angle);  

  //Serial.print(" M: ");
  //Serial.print(Sen1.getMagnitude());  
  
  Serial.println();
  delay(100);
}


