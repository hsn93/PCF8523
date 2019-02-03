# PCF8523 library
RTC Real time clock for ASF project CPP

there is allready a [library](https://github.com/SpellFoundry/PCF8523/blob/master/PCF8523.cpp) for this chip:
i used some definitions from there ..

but this project is for ATMEL ASF project.

# Usage:

## struct:
```
typedef struct{
	uint8_t seconds;
	uint8_t minutes;
	uint8_t hours;
	uint8_t days;
	uint8_t weekdays;
	uint8_t months;
	uint8_t years;
}datetime;
```

## include and declare object:
```
#include "PCF8523.h"

#define RTC_ADDR 0x68
PCF8523 ext_RTC(TWI0, RTC_ADDR);
```
## to set up the RTC:
  * NOTE: falling slew rate of Vdd must be 0.7V/ms check PCF8523 datasheet 14.1 Battery switch-over applications
```
// for first run only..
  datetime mydt; //datetime
  mydt.years = 19; mydt.months = 1; mydt.days = 28;
  mydt.hours =12 ; mydt.minutes = 0; mydt.seconds = 0;
  ext_RTC.settime(mydt);
  ext_RTC.batteryswitchover(0);
```
## to get time.. 
```
 // to get time:
 datetime mydt; //datetime
 mydt = ext_RTC.gettime();
```

of course you have to set up the TWI first but that is in ASF.
