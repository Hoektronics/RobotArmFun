/*
AS5048 - library for ATMega328
 The AS5048 is an easy to use 360° angle position sensor with a 14-bit high resolution output.
 The maximum system accuracy is 0.05° assuming linearization and averaging is done by the external micro controller.
 
 Documentation link: http://www.ams.com/eng/content/download/438523/1341157/143015
 
 Copyright (c) 2013 GROZEA Ion. All right reserved.
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
#include <SPI.h>
#include "AS5048.h"
#include <util/parity.h>

#if defined(ARDUINO) && ARDUINO >= 100
  #include "Arduino.h"
#else
  #include "WProgram.h"
#endif

//Control and Error Register addresses
#define SPI_NOP			0x0000// No operation dummy information
#define SPI_REG_CLRERR  	0x0001// Error Register. All errors are cleared by access

//Read Registers addresses
#define SPI_READ_AGC	        0x3FFD//Diagnostics flags
#define SPI_REG_MAG		0x3FFE//Magnitude information after ATAN calculation
#define SPI_REG_DATA    	0x3FFF//Angle information after ATAN calculation and zero position adder

AS5048::AS5048(uint8_t devPin)
{
  _devPin = devPin;
  pinMode(_devPin, OUTPUT);
  digitalWrite(_devPin, HIGH);
  
  SPI.begin();
  SPI.setBitOrder(MSBFIRST);
  SPI.setDataMode(SPI_MODE1);

  clearErrors();
}

void AS5048::clearErrors(void)
{
  readCMD(SPI_REG_CLRERR);
  readCMD(SPI_NOP);
}

uint16_t AS5048::getAngle(void)
{
  readCMD(SPI_REG_DATA);
  return 0x3FFF & readCMD(SPI_NOP);
}

uint16_t AS5048::getMagnitude(void)
{
  readCMD(SPI_REG_MAG);//Received data is thrown away: this data comes from the precedent command (unknown)
  return 0x3FFF & readCMD(SPI_NOP);//Received data is the MAG value: this data comes from the precedent command (SPI_READ_MAG)
}

uint16_t AS5048::getDiagnostics(void)
{
  readCMD(SPI_READ_AGC);//Received data is thrown away: this data comes from the precedent command (unknown)
  return readCMD(SPI_NOP);//Received data is the AGC value: this data comes from the precedent command (SPI_READ_AGC)
}

uint16_t AS5048::readCMD(uint16_t cmd)
{
  //set our read and parity bits
  cmd = cmd | 0x4000;
  cmd = setParityBit(cmd);
  
  digitalWrite(_devPin, LOW);
  uint8_t _highByte = SPI.transfer(cmd >> 8);
  uint8_t _lowByte = SPI.transfer(cmd);
  digitalWrite(_devPin, HIGH);

  return word(_highByte, _lowByte);
}

uint16_t AS5048::writeCMD(uint16_t cmd)
{
  //set our read and parity bits
  cmd = cmd & 0xBFFF;
  cmd = setParityBit(cmd);
  
  digitalWrite(_devPin, LOW);
  uint8_t _highByte = SPI.transfer(cmd >> 8);
  uint8_t _lowByte = SPI.transfer(cmd);
  digitalWrite(_devPin, HIGH);

  return word(_highByte, _lowByte);
}

uint16_t AS5048::setParityBit(uint16_t cmd)
{
  return cmd | ((parity_even_bit(cmd) ^ parity_even_bit(cmd >> 8)) << 15);
}

