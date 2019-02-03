/* 
* PCF8523.h
*
* Created: 27/1/2019 11:18:29 AM
* Author: hasan.alattar
*/


#ifndef __PCF8523_H__
#define __PCF8523_H__

#include <compiler.h>
// From module: TWI - Two-Wire Interface - SAM implementation
#include <sam_twi/twi_master.h>
#include <sam_twi/twi_slave.h>
#include <twi_master.h>
#include <twi_slave.h>

// From module: TWI - Two-wire Interface
#include <twi.h>

//	========================================		//   |   BIT 7   |   BIT 6   |   BIT 5   |   BIT 4   |   BIT 3   |   BIT 2   |   BIT 1   |   BIT 0   |
													//   |			 |			 |			 |			 |			 |			 |			 |			 |  
#define PCF8523_REG_CONTROL_1			0x00		//   |  CAP_SEL  |	   T	 |	 STOP	 |	  SR	 |	 12_24	 |	  SIE	 |	  AIE	 |	  CIE	 | 
#define PCF8523_REG_CONTROL_2			0x01		//   |	 WTAF	 |	 CTAF	 |	 CTBF	 |	  SF	 |	  AF	 |	 WTAIE	 |	 CTAIE	 |	 CTBIE	 | 
#define PCF8523_REG_CONTROL_3			0x02		//   |	 PM[2:0]						 |	  --	 |	  BSF	 |	  BLF	 |	  BSIE	 |	  BLIE	 | 
#define PCF8523_REG_SECONDS				0x03		//   |	  OS	 |							SECONDS (0 to 59)										 | 
#define PCF8523_REG_MINUTES				0x04		//   |	  --	 |							MINUTES (0 to 59)										 | 
#define PCF8523_REG_HOURS				0x05		//   |	  --	 |	  --	 |	 AMPM	 |			HOURS (1 to 12 in 12 hour mode)					 | 
												//		 |	  --	 |	  --	 |						HOURS (0 to 23 in 24 hour mode)					 | 
#define PCF8523_REG_DAYS				0x06		//   |	  --	 |	  --	 |					DAYS (1 to 31)										 | 
#define PCF8523_REG_WEEKDAYS			0x07		//   |	  --	 |	  --	 |	  --	 |	  --	 |	  --	 |			WEEKDAYS (0 to 6)		 | 
#define PCF8523_REG_MONTHS				0x08		//   |	  --	 |	  --	 |	  --	 |					MONTHS (1 to 12)						 | 
#define PCF8523_REG_YEARS				0x09		//   |											YEARS (0 to 99)										 | 
//#define PCF8523_REG_MINUTE_ALARM		0x0A		//   |	 AEN_M	 |			MINUTE_ALARM (0 to 59)													 |
//#define PCF8523_REG_HOUR_ALARM		0x0B		//   |	 AEN_H	 |	  --	 |	 AMPM	 |			HOUR_ALARM (1 to 12 in 12 hour mode)			 |	
													//   |	 AEN_H	 |	  --	 |						HOUR_ALARM (0 to 23 in 24 hour mode)			 |	
//#define PCF8523_REG_DAY_ALARM			0x0C		//   |	 AEN_D	 |	  --	 |					DAY_ALARM (1 to 31)									 |
//#define PCF8523_REG_WEEKDAY_ALARM		0x0D		//   |	 AEN_W	 |	  --	 |	  --	 |	  --	 |	  --	 |		WEEKDAY_ALARM (0 to 6)		 |
#define PCF8523_REG_OFFSET				0x0E		//   |	  MODE	 |					 OFFSET[6:0]													 |
//#define PCF8523_REG_TMR_CLKOUT_CTRL	0x0F		//   |	  TAM	 |	  TBM	 |			COF[2:0]				 |		TAC[1:0]		 |	  TBC	 | 
//#define PCF8523_REG_TMR_A_FREQ_CTRL	0x10		//   |	  --	 |	  --	 |	  --	 |	  --	 |				TAQ[2:0]						 | 
//#define PCF8523_REG_TMR_A_REG			0x11		//   |												T_A[7:0]										 | 
//#define PCF8523_REG_TMR_B_FREQ_CTRL	0x12		//   |	  --	 |				TBW[2:0]			 |	  --	 |				TBQ[2:0]			 | 
//#define PCF8523_REG_TMR_B_REG			0x13		//   |											T_B[7:0]											 | 



											// [ default-BIT ] DESCRIPTION / tab / [ BIT ] DESCRIPTION
#define PCF8523_CONTROL_1_CAP_SEL_BIT	7	// [0] 7pF									[1] 12.5pF
#define PCF8523_CONTROL_1_T_BIT			6	// [0] Must always 0.						[ ] UNUSED
#define PCF8523_CONTROL_1_STOP_BIT		5	// [0] RTC time circuits running			[1] RTC time circuits frozen;
#define PCF8523_CONTROL_1_SR_BIT		4	// [0] no software reset					[1] initiate software reset
#define PCF8523_CONTROL_1_1224_BIT		3	// [0] 24 hour mode is selected				[1] 12 hour mode is selected
#define PCF8523_CONTROL_1_SIE_BIT		2	// [0] second interrupt disabled			[1] second interrupt enabled
#define PCF8523_CONTROL_1_AIE_BIT		1	// [0] alarm interrupt disabled				[1] alarm interrupt enabled
#define PCF8523_CONTROL_1CIE_BIT		0	// [0] no correction interrupt generated	[1] interrupt pulses are generated at every correction cycle
//For a software reset, 01011000 (58h) must be sent to register Control_1 (see Section 8.3 ). Bit SR always returns 0 when read.

#define PCF8523_CONTROL_2_WTAF_BIT		7	// [0] no watchdog timer A interrupt generated	[1] flag set when watchdog timer A interrupt generated
#define PCF8523_CONTROL_2_CTAF_BIT		6	// [0] no countdown timer A interrupt generated [1] flag set when countdown timer A interrupt generated
#define PCF8523_CONTROL_2_CTBF_BIT		5	// [0] no countdown timer B interrupt generated	[1] flag set when countdown timer B interrupt generated
#define PCF8523_CONTROL_2_SF_BIT		4	// [0] no second interrupt generated			[1] flag set when second interrupt generated
#define PCF8523_CONTROL_2_AF_BIT 		3	// [0] no alarm interrupt generated				[1] flag set when alarm triggered
#define PCF8523_CONTROL_2_WTAIE_BIT		2	// [0] watchdog timer A interrupt is disabled	[1] watchdog timer A interrupt is enabled
#define PCF8523_CONTROL_2_CTAIE_BIT		1	// [0] countdown timer A interrupt is disabled	[1] 
#define PCF8523_CONTROL_2_CTBIE_BIT		0	// [0] countdown timer B interrupt is disabled	[1]

#define PCF8523_CONTORL_3_PM				// 111 
/*
PM[2:0] Power management function control bits
000
		battery switch-over function is enabled in standard mode; battery low detection function is enabled
001
		battery switch-over function is enabled in direct switching mode; battery low detection function is enabled
010,011
		battery switch-over function is disabled - only one power supply (VDD); battery low detection function is enabled
100
		battery switch-over function is enabled in standard mode; battery low detection function is disabled
101
		battery switch-over function is enabled in direct switching mode; battery low detection function is disabled
[111]
		battery switch-over function is disabled - only one power supply (VDD); battery low detection function is disabled
*/


/*
		• 32.768 kHz CLKOUT active
		• 24 hour mode is selected
		• Register Offset is set logic 0
		• No alarms set
		• Timers disabled
		• No interrupts enabled
		• Battery switch-over is disabled
		• Battery low detection is disabled
		• 7 pF of internal oscillator capacitor selected
*/


typedef struct{
	uint8_t seconds;
	uint8_t minutes;
	uint8_t hours;
	uint8_t days;
	uint8_t weekdays;
	uint8_t months;
	uint8_t years;
}datetime;

class PCF8523
{
//variables
public:
protected:
private:
	uint8_t _I2Caddr;
	Twi* _pTWI;

//functions
public:
	PCF8523(Twi* pTWI, uint8_t addr);
	datetime gettime(void);
	void settime(datetime curr);
	void soft_reset();
	void batteryswitchover(uint8_t PM);
	void offset_ppm(int8_t ppm_offset);
	~PCF8523();
	bool isrunning(void);
protected:
private:
	void _writeRegister8(uint8_t address, uint8_t value);
	void _writeRegister8(uint8_t address, uint8_t length, uint8_t* buf);
	uint8_t readRegister8(uint8_t address);
	uint8_t readRegister8(uint8_t address, uint8_t length, uint8_t* buf);
	//PCF8523( const PCF8523 &c );
	//PCF8523& operator=( const PCF8523 &c );

}; //PCF8523

#endif //__PCF8523_H__
