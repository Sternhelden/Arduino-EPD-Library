/*
	Davicom Products SMI access library
	Author: Russell Liu
	Copyright: GPL V2
	
	This library provide a SMI access to read/write register 
	from/to the DMavicom products with SMI.
	
	2014-10-29 <russell_liu@davicom.com.tw>
*/

#include "Arduino.h"
#include "epd.h"

	unsigned int _controlRegister;
    unsigned int _registerNumber;
	unsigned int _model;
	
EPD::EPD(int model, int clockPin, int dataPin) {
	pinMode(clockPin, OUTPUT);
	pinMode(dataPin, OUTPUT);
	_clockPin=clockPin;
	_dataPin=dataPin;
	_model =model;
    switch (model) {
		case 0:
			_controlRegister = 0x05;
			_registerNumber = 4;
			break;
		case 1:
			_controlRegister = 0x10;
			_registerNumber = 16;
			break;
  }
}

void EPD::writeRegister(byte _nodeID, byte _address, byte _data) {
  startBit();
  writeNode(_nodeID);
  checkAck();
  writeData(_address);
  checkAck();
  writeData(_data);
  checkAck();
  stopBit();
}

void EPD::clockPulse() {
  digitalWrite(_clockPin, HIGH);
  digitalWrite(_clockPin, LOW);
}

void EPD::startBit() {
  digitalWrite(_dataPin, LOW);
  digitalWrite(_clockPin, LOW);
}

void EPD::writeNode(byte _value) {
  int epd_i;
  _value = 0xf0+_value;
  for (epd_i=7; epd_i>=0; epd_i--){   
    if ((_value &(1<<epd_i))==0) {
      digitalWrite(_dataPin, LOW); 
    } else {
      digitalWrite(_dataPin, HIGH); 
    }
    clockPulse();
  }
}

void EPD::checkAck() {
  int epd_a;
  pinMode(_dataPin, INPUT);
  digitalWrite(_clockPin, HIGH);
  epd_a = digitalRead(_dataPin);
  while (epd_a != 0) {
    delayMicroseconds(1);
    //delay(1);
    epd_a = digitalRead(_dataPin);
  }
  digitalWrite(_clockPin, LOW);
  pinMode(_dataPin, OUTPUT);
}

void EPD::writeData(byte _value) {
  int epd_i;
  for (epd_i=7; epd_i>=0; epd_i--){   
    if ((_value &(1<<epd_i))==0) {
      digitalWrite (_dataPin, LOW); 
    } else {
      digitalWrite (_dataPin, HIGH); 
    }
    clockPulse();
  }
}

void EPD::stopBit() {
  digitalWrite(_clockPin, HIGH);
  digitalWrite(_dataPin, HIGH);
}

//void EPD::allWhite(unsigned int _model, byte _nodeID) {
void EPD::allWhite(byte _nodeID) {
  int epd_i;
  for (epd_i=0;epd_i<_registerNumber;epd_i++) {
    writeRegister(_nodeID, epd_i, 0x00);
  }
  switch (_model) {
	case 0:
		writeRegister(_nodeID, 4, 0xC0);
		break;
	case 1:
		writeRegister(_nodeID, 0x0F, 0x01);
		break;
	}
  writeRegister(_nodeID, _controlRegister, 0x88);
  writeRegister(_nodeID, _controlRegister, 0x80);
  delay(100);
  //powerSaving(_model, _nodeID);
  powerSaving(_nodeID);
}

//void EPD::allBlack(unsigned int _model, byte _nodeID) {
void EPD::allBlack(byte _nodeID) {
  int epd_i;
  for (epd_i=0;epd_i<_registerNumber;epd_i++) {
    writeRegister(_nodeID, epd_i, 0xFF);
  }
  switch (_model) {
	case 0:
		writeRegister(_nodeID, 4, 0x3F);
		break;
	case 1:
		writeRegister(_nodeID, 0x0F, 0x02);
		break;
	}
  writeRegister(_nodeID, _controlRegister, 0x88);
  writeRegister(_nodeID, _controlRegister, 0x80);
  delay(100);
  //powerSaving(_model, _nodeID);
  powerSaving(_nodeID);
}

//void EPD::powerSaving(unsigned int _model, byte _nodeID) {
void EPD::powerSaving(byte _nodeID) {
  int epd_i;
  for (epd_i=0; epd_i<_registerNumber+1; epd_i++) {
      writeRegister(_nodeID, epd_i, 0x00);     
  }
  writeRegister(_nodeID, _controlRegister, 0x88);
  writeRegister(_nodeID, _controlRegister, 0x80);
  writeRegister(_nodeID, _controlRegister, 0x00);
}