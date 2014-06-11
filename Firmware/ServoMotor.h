/*
 ServoMotor - library for ATMega328
 
 A simple library for PID control of a DC motor + Encoder 
 
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
 
 
#ifndef H_ServoMotor
#define H_ServoMotor

#include <inttypes.h>

#include "AS5048.h"
#include "PID_v1.h"
#include "DCMotor.h"

#if defined(ARDUINO) && ARDUINO >= 100
  #include "Arduino.h"
#else
  #include "WProgram.h"
#endif

class ServoMotor
{
public:

  ServoMotor(PID& pid, AS5048& encoder, DCMotor& motor, Print & print);

  int32_t getPosition(void);
  int32_t getTarget(void);
  
  void setPosition(int32_t position);
  void setTarget(int32_t target);
  
  void update(void);
  int32_t updatePosition(void);
  
private:
  int32_t _target;
  int32_t _absolutePosition;
  int32_t _lastPosition;
  
  double _PID_input;
  double _PID_setpoint;
  double _PID_output;
  
  PID & _pid;
  AS5048 & _encoder;
  DCMotor & _motor;
  
  Print* printer;
};
#endif

