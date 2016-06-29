/****************************************************************************/
//  Function: Header file for PCD85063TP
//  Hardware: Grove - RTC v2.0
//  Arduino IDE: Arduino-1.6.6
//  Author:   FrankieChu
//  Date:    June 14,2016
//  Version: v1.0
//  by www.seeedstudio.com
//
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License as published by the Free Software Foundation; either
//  version 2.1 of the License, or (at your option) any later version.
//
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
//
/****************************************************************************/

#include <Wire.h>
#include "PCF85063TP.h"

uint8_t PCD85063TP::decToBcd(uint8_t val)
{
  return ( (val/10*16) + (val%10) );
}

//Convert binary coded decimal to normal decimal numbers
uint8_t PCD85063TP::bcdToDec(uint8_t val)
{
  return ( (val/16*10) + (val%16) );
}

void PCD85063TP::begin()
{
  Wire.begin();
}
/*Function: The clock timing will start */
void PCD85063TP::startClock(void)        // set the ClockHalt bit low to start the rtc
{
  uint8_t data;
  Wire.beginTransmission(PCD85063TP_I2C_ADDRESS);
  Wire.write((uint8_t)0x00);                      // Register 0x00 holds the oscillator start/stop bit
  Wire.endTransmission();
  Wire.requestFrom(PCD85063TP_I2C_ADDRESS, 1);
  data = Wire.read() & ~0x20;       // save actual control_1 regitser and AND sec with bit 7 (sart/stop bit) = clock started
  Wire.beginTransmission(PCD85063TP_I2C_ADDRESS);
  Wire.write((uint8_t)0x00);
  Wire.write((uint8_t)data);                    // write seconds back and start the clock
  Wire.endTransmission();
}
/*Function: The clock timing will stop */
void PCD85063TP::stopClock(void)         // set the ClockHalt bit high to stop the rtc
{
  uint8_t data;
  Wire.beginTransmission(PCD85063TP_I2C_ADDRESS);
  Wire.write((uint8_t)0x00);                      // Register 0x00 holds the oscillator start/stop bit
  Wire.endTransmission();
  Wire.requestFrom(PCD85063TP_I2C_ADDRESS, 1);
  data = Wire.read() | 0x20;       // save actual control_1 regitser and AND sec with bit 7 (sart/stop bit) = clock started
  Wire.beginTransmission(PCD85063TP_I2C_ADDRESS);
  Wire.write((uint8_t)0x00);
  Wire.write((uint8_t)data);                    // write seconds back and stop the clock
  Wire.endTransmission();
}
/****************************************************************/
/*Function: Read time and date from RTC  */
void PCD85063TP::getTime()
{
  // Reset the register pointer
  Wire.beginTransmission(PCD85063TP_I2C_ADDRESS);
  Wire.write((uint8_t)0x04);
  Wire.endTransmission();
  Wire.requestFrom(PCD85063TP_I2C_ADDRESS, 7);
  // A few of these need masks because certain bits are control bits
  second     = bcdToDec(Wire.read() & 0x7f);
  minute     = bcdToDec(Wire.read());
  hour     = bcdToDec(Wire.read() & 0x3f);// Need to change this if 12 hour am/pm
  dayOfMonth = bcdToDec(Wire.read());
  dayOfWeek  = bcdToDec(Wire.read());
  month      = bcdToDec(Wire.read());
  year     = bcdToDec(Wire.read());
}
/*******************************************************************/
/*Frunction: Write the time that includes the date to the RTC chip */
void PCD85063TP::setTime()
{
  Wire.beginTransmission(PCD85063TP_I2C_ADDRESS);
  Wire.write((uint8_t)0x40);
  Wire.write(decToBcd(second));// 0 to bit 7 starts the clock, bit 8 is OS reg
  Wire.write(decToBcd(minute));
  Wire.write(decToBcd(hour));  // If you want 12 hour am/pm you need to set bit 6
  Wire.write(decToBcd(dayOfMonth));
  Wire.write(decToBcd(dayOfWeek));
  Wire.write(decToBcd(month));
  Wire.write(decToBcd(year));
  Wire.endTransmission();
}
void PCD85063TP::fillByHMS(uint8_t _hour, uint8_t _minute, uint8_t _second)
{
  // assign variables
  hour = _hour;
  minute = _minute;
  second = _second;
}
void PCD85063TP::fillByYMD(uint16_t _year, uint8_t _month, uint8_t _day)
{
  year = _year-2000;
  month = _month;
  dayOfMonth = _day;
}
void PCD85063TP::fillDayOfWeek(uint8_t _dow)
{
  dayOfWeek = _dow;
}

void PCD85063TP::reset()
{
  Wire.beginTransmission(PCD85063TP_I2C_ADDRESS);
  Wire.write((uint8_t)0x00);
  Wire.write(decToBcd(0x10));// software reset at bit 4
  Wire.endTransmission();
}

// mode: calibration cycle
//       0 - 2 hours
//       1 - 4 minutes
// Fmeas: Real frequency you detect
void PCD85063TP::setcalibration(int mode, float Fmeas)
{
  float offset = 0;
  float Tmeas = 1.0/Fmeas;
  float Dmeas = 1.0/32768 - Tmeas;
  float Eppm = 1000000.0 * Dmeas/Tmeas;
  if (mode == 0) offset = Eppm/4.34;
  else if (mode == 1) offset = Eppm/4.069;

  uint8_t data = (mode << 7) & 0x80 | ((int)(offset+0.5) & 0x7f);

  Wire.beginTransmission(PCD85063TP_I2C_ADDRESS);
  Wire.write(PCD85063TP_OFFSET);
  Wire.write(data);
  Wire.endTransmission();
}

uint8_t PCD85063TP::readCalibrationReg(void)
{
  Wire.beginTransmission(PCD85063TP_I2C_ADDRESS);
  Wire.write((uint8_t)0x02);
  Wire.endTransmission();
  Wire.requestFrom(PCD85063TP_I2C_ADDRESS, 1);

  return Wire.read();
}
