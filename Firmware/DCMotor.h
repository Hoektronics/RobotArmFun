/*
 DCMotor - library for Arduino
 
 A simple library for h-bridge control of a DC motor 
 
 Copyright (c) 2014 Zach Hoeken. All right reserved.
 
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
 
#ifndef H_DCMotor
#define H_DCMotor

#include <inttypes.h>


class DCMotor
{
public:

  DCMotor(uint8_t left_pin, uint8_t right_pin);

  void setMotor(uint8_t dir, uint8_t pwm);
  void invert();
  
private:
  uint8_t _inverted;
  uint8_t _left;
  uint8_t _right;
};
#endif

