# Grove - High Precision RTC_PCF85063TP
* Grove RTC base on PCD85063TP, it's usage is almost the same with ds1307.

## Methods

* void startClock(void);
* void stopClock(void);
* void setTime(void);
* void getTime(void);
* void setcalibration(int mode, float Fmeas);  <br>
  - mode 0 - offset is made once every 2 hours <br>
  - mode 1 - offset is made once every 4 minutes <br>
  - Fmeas - real clock frequency you detcet <br><br>
* uint8_t readCalibrationReg(void); <br>
  - Read offset register at 0x02 <br><br>
* uint8_t calibratBySeconds(int mode, float offset_sec);
  - offset_sec - need a long time running RTC and work out the offset by one second 
* void reset();
* void fillByHMS(uint8_t _hour, uint8_t _minute, uint8_t _second);
* void fillByYMD(uint16_t _year, uint8_t _month, uint8_t _day);
* void fillDayOfWeek(uint8_t _dow);
