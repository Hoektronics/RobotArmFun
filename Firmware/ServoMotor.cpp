/*
 ServoMotor.cpp - library for encoder based control of dc motors

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
 
#include "ServoMotor.h"

#if defined(ARDUINO) && ARDUINO >= 100
  #include "Arduino.h"
#else
  #include "WProgram.h"
#endif

ServoMotor::ServoMotor(PID & pid, AS5048 & encoder, DCMotor & motor, Print & print) : _pid(pid), _encoder(encoder), _motor(motor)
{
  printer = &print; //operate on the adress of print

  _absolutePosition = 0;
  _lastPosition = _encoder.getAngle();

  setTarget(0);
    
  //default everything to where we are now.
  _PID_input = _PID_setpoint = 0;   
  _pid.LinkVars(&_PID_input, &_PID_output, &_PID_setpoint);
  _pid.SetMode(AUTOMATIC);
  _pid.SetSampleTime(1);
  _pid.Compute();
}

int32_t ServoMotor::getPosition(void)
{
  updatePosition();
  
  return _absolutePosition;
}

int32_t ServoMotor::getTarget(void)
{
  return _target;
}

void ServoMotor::setPosition(int32_t position)
{
  _absolutePosition = position;
}

void ServoMotor::setTarget(int32_t target)
{
  _target = target;
  _PID_setpoint = target;
}

void ServoMotor::update(void)
{
  //get our latest absolute position
  int32_t movement_delta = updatePosition();
  
  //what is our current input
  _PID_input = _absolutePosition;

  printer->print(" A: ");
  printer->print(_absolutePosition);  

  printer->print(" T: ");
  printer->print(_target);
  
  //where are we going?
  int32_t target_delta = _target - _absolutePosition;
  
  //is the delta even worth worrying over?
  if (abs(target_delta) > 5)
  {
    //PID goes up or down...
    if (target_delta >= 0)
      _pid.SetControllerDirection(DIRECT);
    else
      _pid.SetControllerDirection(REVERSE);
    
    //do our PID here.
    _pid.Compute();

    printer->print(" D: ");
    printer->print(target_delta);
    
    printer->print(" In: ");
    printer->print(_PID_input);
  
    printer->print(" Set: ");
    printer->print(_PID_setpoint);
  
    printer->print(" Out: ");
    printer->print(_PID_output);

    //okay, lets run the motors.
    if (target_delta > 0)
      _motor.setMotor(1, _PID_output);
    else
      _motor.setMotor(0, _PID_output);
  }
  else
  {
      _motor.setMotor(0, 0);
  }
  
  printer->println();
}

int32_t ServoMotor::updatePosition(void)
{
  int32_t newPosition = _encoder.getAngle();
  int32_t delta = 0;
  uint8_t dir = 0;
  
  //which direction did it move?
  if (_lastPosition > newPosition)
  {
    dir = 0;
    delta = _lastPosition - newPosition;
  }
  else
  {
    dir = 1;
    delta = newPosition - _lastPosition;
  }
  
//  printer->print(" R: ");
//  printer->print(newPosition);
//
//  printer->print(" D: ");
//  printer->print(delta);
//
  //did it loop around?
  if (delta >= 8192)
  {
    dir = !dir;
    delta = 16384 - delta;    
  }
  
  //record our last position for next time.
  _lastPosition = newPosition;

  //okay, add our delta to our absolute position now.
  if (dir)
  {
    _absolutePosition = _absolutePosition + delta;
    return delta;
  }
  else
  {
    _absolutePosition = _absolutePosition - delta;
    return -delta;
  }
}


