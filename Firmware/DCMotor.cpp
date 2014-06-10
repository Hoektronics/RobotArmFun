/*
 DCMotor - library for Arduino
 
 A simple library for h-bridge control of a DC motor 

 Copyright (c) 2014 Zach Smith. All rights reserved.
 
 This library is free software; you can redistribute it and/or
 modify it under the terms of the GNU Lesser General Public
 License as published by the Free Software Foundation; either
 version 2.1 of the License, or (at your option) any later version.
 
 This library is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 Lesser General Public License for more details.
 
 You should have received a copy of the GNU Lesser General Public
 License along with this library; if not, write to the Free Software
 Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
 */
 
#include "DCMotor.h"

#if defined(ARDUINO) && ARDUINO >= 100
  #include "Arduino.h"
#else
  #include "WProgram.h"
#endif


DCMotor::DCMotor(uint8_t left_pin, uint8_t right_pin)
{
  _inverted = 0;
  _left = left_pin;
  _right = right_pin;
  
  pinMode(_left, OUTPUT);
  pinMode(_right, OUTPUT);
  digitalWrite(_left, LOW);
  digitalWrite(_right, LOW);
}

void DCMotor::setMotor(uint8_t dir, uint8_t pwm)
{
  if (_inverted)
    dir = !dir;
    
  if(dir)
  {
    digitalWrite(_right, LOW);    
    digitalWrite(_left, LOW);    
    analogWrite(_right, pwm);
  }
  else
  {
    digitalWrite(_left, LOW);    
    digitalWrite(_right, LOW);    
    analogWrite(_left, pwm);
  }
}

void DCMotor::invert(void)
{
  _inverted = !_inverted;
}
