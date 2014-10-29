/*
	Davicom Products SMI access library
	Author: Russell Liu
	Copyright: GPL V2
	
	This library provide a SMI access to read/write register 
	from/to the DMavicom products with SMI.
	
	2014-10-29 <russell_liu@davicom.com.tw>
*/
#define __PROG_TYPES_COMPAT__

#ifndef epd_h
#define epd_h

#if defined(ARDUINO) && ARDUINO >= 100 // Arduino 1.0
#include "Arduino.h"
#else
#include "WProgram.h" // Arduino 0022
#endif

class EPD {
	public:
		EPD(int clockPin, int dataPin);
		void writeRegister(byte _nodeID, byte _address, byte _data);
		void allWhite(byte _nodeID);
		void allBlack(byte _nodeID);
		void powerSaving(byte _nodeID);
	private:
		int _dataPin;
		int _clockPin;
		void clockPulse();
		void startBit();
		void writeNode(byte _value);
		void checkAck();
		void writeData(byte _value);
		void stopBit();
};

#endif