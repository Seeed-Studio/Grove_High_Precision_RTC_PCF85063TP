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

#ifndef __PCF85063TP_H__
#define __PCF85063TP_H__

#include <Arduino.h>

#define PCD85063TP_I2C_ADDRESS 0x51

#define MON 1
#define TUE 2
#define WED 3
#define THU 4
#define FRI 5
#define SAT 6
#define SUN 0

class PCD85063TP
{
private:
  uint8_t decToBcd(uint8_t val);
  uint8_t bcdToDec(uint8_t val);

public:
  void begin();
  void startClock(void);
  void stopClock(void);
  void setTime(void);
  void getTime(void);
  void reset();
  void fillByHMS(uint8_t _hour, uint8_t _minute, uint8_t _second);
  void fillByYMD(uint16_t _year, uint8_t _month, uint8_t _day);
  void fillDayOfWeek(uint8_t _dow);
  uint8_t second;
  uint8_t minute;
  uint8_t hour;
  uint8_t dayOfWeek;// day of week, 1 = Monday
  uint8_t dayOfMonth;
  uint8_t month;
  uint16_t year;
};

#endif
