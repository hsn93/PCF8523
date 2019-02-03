/* 
* PCF8523.cpp
*
* Created: 27/1/2019 11:18:28 AM
* Author: hasan.jaafar
*/


#include "PCF8523.h"


static uint8_t bcd2bin (uint8_t val) { return val - 6 * (val >> 4); }
static uint8_t bin2bcd (uint8_t val) { return val + 6 * (val / 10); }
	
	

// default constructor
PCF8523::PCF8523(Twi* pTWI, uint8_t addr)
{
	this->_I2Caddr = addr;
	this->_pTWI = pTWI;
} //PCF8523

	void PCF8523::batteryswitchover(uint8_t PM){
		PM &= 0xE0; //0b1110 0000
		_writeRegister8(PCF8523_REG_CONTROL_3,PM);
	}
	
	bool PCF8523::isrunning(void) {
		uint8_t reg1 = readRegister8(PCF8523_REG_CONTROL_3);
		return ((reg1 & 0xE0) != 0xE0);
	}
	
	void PCF8523::soft_reset(){
		_writeRegister8(PCF8523_REG_CONTROL_1,0x58);
	}
	
	void PCF8523::settime(datetime curr){
		uint8_t arr[] = {bin2bcd( curr.seconds), bin2bcd(curr.minutes), bin2bcd(curr.hours),
						 bin2bcd( curr.days ), 0, bin2bcd(curr.months), bin2bcd(curr.years)};
		_writeRegister8(PCF8523_REG_SECONDS,7,arr);
	}
	
	datetime PCF8523::gettime(void){
		
		datetime data;
		readRegister8(3,7,(uint8_t*)&data);
		
		datetime returnval;
		returnval.seconds =  bcd2bin(data.seconds & 0x7F);
		returnval.minutes =  bcd2bin(data.minutes);
		returnval.hours =  bcd2bin(data.hours);
		returnval.days = bcd2bin(data.days);
		returnval.months = bcd2bin(data.months);
		returnval.years = bcd2bin(data.years);
		
		return returnval;
	}


	void PCF8523::offset_ppm(int8_t ppm_offset){
		//Bit [7] mode = [0] every 2 Hour [1] every 1 minute
		// in mode 0 - each offset value takes 4.34 , in mode 1 each offset takes 4.069
		//Bits [6:0] = signed -64 to +63 offset value
		
		// in my board i had 6 sec faster every day.
		// this means i have 6 extra seconds per 86400 seconds
		// i say that this is 69.4 ppm!
		
		// so if i added offset of 17 in mode 1 , it should be 4.069 * 17 = 69.173 ppm
		
		//correction every two hours is better for energy saving!
		//ppm_offset = ppm_offset  1;
		
		uint8_t offset_6bit = ppm_offset & 0x7f;
		
		offset_6bit |= 0x80; //mode [1] every 1 minute
		
		_writeRegister8(PCF8523_REG_OFFSET,offset_6bit);
	}


	
	void PCF8523::_writeRegister8(uint8_t address, uint8_t value)
	{
		twi_packet_t packet;
		char data[] = {address,value};
		packet.buffer = &data;
		packet.length = 2;
		packet.chip = _I2Caddr;
		packet.addr_length = 0;
		twi_master_write(_pTWI, &packet);
	}


	void PCF8523::_writeRegister8(uint8_t address, uint8_t length, uint8_t* buf)
	{
		twi_packet_t packet;
		//char data[] = {reg_val};
		packet.buffer = buf;
		packet.length = length;
		packet.chip = _I2Caddr;
		packet.addr[0] = address;
		packet.addr_length = 1;
		twi_master_write(_pTWI, &packet);
	}




	uint8_t PCF8523::readRegister8(uint8_t address)
	{
		twi_packet_t packet;
		uint8_t data =0;
		packet.buffer = &data;
		packet.length = 1;
		packet.chip = _I2Caddr;
		packet.addr[0] = address;
		packet.addr_length = 1;
		
		if (twi_master_read(_pTWI,&packet) != TWI_SUCCESS)
			return -1;
		return(data);
	}
	

	uint8_t PCF8523::readRegister8(uint8_t address, uint8_t length, uint8_t* buf)
	{
		twi_packet_t packet;
		//uint8_t data[length] ;
		packet.buffer = buf;
		packet.length = length;
		packet.chip = _I2Caddr;
		packet.addr[0] = address;
		packet.addr_length = 1;
			
		if (twi_master_read(_pTWI,&packet) != TWI_SUCCESS)
		return -1;
		return 0;
	}
	


// default destructor
PCF8523::~PCF8523()
{
} //~PCF8523
