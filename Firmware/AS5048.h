/*
AS5048 - library for ATMega328
 The AS5048 is an easy to use 360° angle position sensor with a 14-bit high resolution output.
 The maximum system accuracy is 0.05° assuming linearization and averaging is done by the external micro controller.
 
 Documentation link: http://www.ams.com/eng/content/download/438523/1341157/143015
 
 Copyright (c) 2013 GROZEA Ion. All right reserved.
 
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
 
 
#ifndef H_AS5048
#define H_AS5048
#include <inttypes.h>
class AS5048
{
public:
  AS5048(uint8_t devPin);
  void clearErrors(void);
  uint16_t getAngle(void);
  uint16_t getMagnitude(void);
  uint16_t getDiagnostics(void);
  uint16_t readCMD(uint16_t cmd);
  uint16_t writeCMD(uint16_t cmd);
  uint16_t setParityBit(uint16_t cmd);
  
private:
  uint8_t _devPin;
  uint16_t _data;
};
#endif

