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
#ifndef shiftRegister_h
#define shiftRegister_h

#if ARDUINO < 100
	#include "WProgram.h"
#else
	#include "Arduino.h"
#endif

#define MAXSRCHIPS 4//change this if you are using more than 4 chips
#define SRDEFAULT 0b00000000
typedef enum SR_type{
     _74XX595 = 1
};
		
class shiftRegister
{
  public:

	bool 			srInit(const uint8_t data_pin,const uint8_t latch_pin,const uint8_t clock_pin,const uint8_t chips=1,const uint8_t SR_type=_74XX595);
    virtual void 	srSend(void);
	virtual void	srPort(byte data,uint8_t chip);			//byte send
	virtual void	srPort(word data);						//word send
	virtual byte	srRead(uint8_t chip);					//read n port
	virtual word	srRead();								//read port 0,1
	virtual void	srDigitalWrite(uint8_t pin,bool data);	//write data(0,1) into a single pin

  protected:
	volatile byte	_data[MAXSRCHIPS];
    uint8_t 		_data_pin;
	uint8_t 		_latch_pin;
	uint8_t 		_clock_pin;
	uint8_t 		_chips;
	uint8_t 		_chipType;
	uint8_t 		_ports;
	uint8_t 		_bitOrder;
	void 			_fastShiftOut(uint8_t val);
};

#endif 
