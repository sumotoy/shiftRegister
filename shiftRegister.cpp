/*
                               _                
 ___  _   _  _ __ ___    ___  | |_  ___   _   _ 
/ __|| | | || '_ ` _ \  / _ \ | __|/ _ \ | | | |
\__ \| |_| || | | | | || (_) || |_| (_) || |_| |
|___/ \__,_||_| |_| |_| \___/  \__|\___/  \__, |
                                          |___/ 
universal shiftRegister basic Library
--------------------------------------------------------------------------------------
A simple C library for most shift register chips
that can be easily included in other libraries.
version 1.0b1
coded by Max MC Costa for s.u.m.o.t.o.y [sumotoy(at)gmail.com]
--------------------------------------------------------------------------------------
Library works with most arduino compatible micros and Teensy2/3
*/


#include "shiftRegister.h"

/*------------------ protected -----------------------------------------------
----------------------------------------------------------------------------*/

//------------------ low level -----------------------------------------------

//n bit shiftOut
void shiftRegister::_fastShiftOut(uint8_t val){
	uint8_t i;
	for (i = 0; i < this->_ports; i++)  {
		if (_bitOrder == 0){//upper first
			#if defined(__MK20DX128__)
			::digitalWriteFast(this->_data_pin, !!(val & (1 << i)));
			#else
			::digitalWrite(this->_data_pin, !!(val & (1 << i)));
			#endif
		} else {//lower first
			#if defined(__MK20DX128__)
			::digitalWriteFast(this->_data_pin, !!(val & (1 << ((this->_ports-1) - i))));
			#else
			::digitalWrite(this->_data_pin, !!(val & (1 << ((this->_ports-1) - i))));
			#endif
		}	
		#if defined(__MK20DX128__)
		::digitalWriteFast(this->_clock_pin, HIGH);
		::digitalWriteFast(this->_clock_pin, LOW);
		#else
		::digitalWrite(this->_clock_pin, HIGH);
		::digitalWrite(this->_clock_pin, LOW);
		#endif
	}
}


/*---------- public ----------------------------------------------------
----------------------------------------------------------------------*/
bool shiftRegister::srInit(const uint8_t data_pin,const uint8_t latch_pin,const uint8_t clock_pin,const uint8_t chips,const uint8_t SR_type){
	uint8_t i;
 	this->_data_pin = data_pin;
	this->_latch_pin = latch_pin;
	this->_clock_pin = clock_pin;
	switch(SR_type){
		case _74XX595:
			this->_ports = 8;
			this->_chipType = 1;
			this->_bitOrder= 1;//lower first
		break;
		default:
			this->_ports = 0;
			this->_chipType = 0;
			this->_bitOrder= 0;
	};
	if (chips > 0 && chips <= MAXSRCHIPS){
		this->_chips = chips;
	} else {
		this->_chips = 0;
		this->_chipType = 0;
	}
	if (_chipType > 0){
		::pinMode(this->_data_pin,OUTPUT);
		::pinMode(this->_latch_pin,OUTPUT);
		::pinMode(this->_clock_pin,OUTPUT);
		::digitalWrite(this->_latch_pin,HIGH);	
		::digitalWrite(this->_clock_pin,LOW);
		for (i=0;i<_chips;i++){
			_data[i] = SRDEFAULT;
		}
		srSend();
		return true;
	} else {
		return false;
	}
}

//
void shiftRegister::srPort(byte data,uint8_t chip){
	_data[chip-1] = data;
}

//
void shiftRegister::srPort(word data){
	_data[0] = (byte)(data >> 8);
	_data[1] = (byte)(data & 0x00FF);
}

//
byte shiftRegister::srRead(uint8_t chip){
	return _data[chip-1];
}

//
word shiftRegister::srRead(){
	return (word)_data[0] << 8 | (word)_data[1];
}

//
void shiftRegister::srDigitalWrite(uint8_t pin,bool data){
	uint8_t maxPorts = this->_ports * this->_chips;
	if (pin < maxPorts){
		int port = pin >> (this->_ports/2);
		int data_index = (pin & this->_ports) >> 3;
		int bit_number = pin - ((port << 1) + data_index)*this->_ports;
		if (data){
			bitSet(_data[data_index],bit_number);
		} else {
			bitClear(_data[data_index],bit_number);
		}
	}
}

//
void shiftRegister::srSend(void){
	uint8_t i;
#if defined(__MK20DX128__)
	::digitalWriteFast(this->_latch_pin,LOW);
#else
	::digitalWrite(this->_latch_pin,LOW);
#endif
	for (i=0;i<_chips;i++){
		_fastShiftOut(_data[i]);
	}
#if defined(__MK20DX128__)
	::digitalWriteFast(this->_latch_pin,HIGH);
#else
	::digitalWrite(this->_latch_pin,HIGH);
#endif
}