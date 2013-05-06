/*
   Copyright 2013 Nusku Networks

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/

/*
  Adapted from Michael Neiderhauser
  Microchip MicroChip I2C EEPROM library for Arduino
  
  https://github.com/mikeneiderhauser/I2C-EEPROM-Arduino

  Below contains original licencing information and copyright notices
*/

/*************************************************** 
  This is a library for the 24LC256 & 24LC512 i2c EEPROM under the
  Arduino Platform.
  
  It provides an easy way to use the above chips while supplying 
  the functionality to read and write pages (64 or 128 bytes) and 
  individual bytes to a page number or address on the EEPROM.
  
  Each page is written in a 16 byte burst mode to account for the
  buffer limit of the Wire library in the Arduino Platform.

  This library is to be used "as-is" without warranty. Please report
  bugs to the GitHub Project.
  
  Written by Michael Neiderhauser. October 10, 2012
  LGPL license, all text above must be included in any redistribution
 ****************************************************/

#ifndef DRIVERI2CMC24LCXXX_H
#define DRIVERI2CMC24LCXXX_H

// The chip testing method can be rather large, do not build it by default
#ifndef INCLUDE_TEST_CHIP
#define INCLUDE_TEST_CHIP true
#endif

#ifndef DEBUG
#define DEBUG true
#endif

#define I2CMC24LCXXX_PAGE256 64
#define I2CMC24LCXXX_PAGE512 128

#define I2CMC24LCXXX_PAGES 512

#define I2CMC24LCXXX_I2CBASEADDR 0x50

#define I2CMC24LCXXX_MINIBUFFER 16

class DriverI2CMC24LCxxx {
public:
	DriverI2CMC24LCxxx(uint8_t pin_address, uint8_t page_size);
	bool readPage(uint16_t page_number, byte* data);
	bool writePage(uint16_t page_number, byte* data);
	// bool read(uint16_t address, byte *buffer, uint16_t size);
	// bool write(uint16_t address, byte *buffer, uint16_t size);
	uint8_t pageSize();
	uint8_t pinAddress();
	uint8_t fullAddress();
	bool isPaged() { return true; }
	// bool isWriteProtected
	bool erase();
#if INCLUDE_TEST_CHIP
	bool testChip();
#endif
	
private:
	byte minibuffer[I2CMC24LCXXX_MINIBUFFER];
	uint8_t _pageSize;
	uint8_t _pinAddress;
	uint8_t _fullAddress;
	bool bufferedPageRead(uint16_t address);
	bool bufferedPageWrite(uint16_t address);
};

#endif