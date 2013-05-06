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

#include "Arduino.h"
#include <Wire.h>

#include "DRIVERI2CMC24LCXXX.h"

DriverI2CMC24LCxxx::DriverI2CMC24LCxxx(uint8_t pin_address, uint8_t page_size) {
	_pageSize = page_size;
    _pinAddress = pin_address;
	_fullAddress = I2CMC24LCXXX_I2CBASEADDR | pin_address;

    for(uint8_t i=0; i<I2CMC24LCXXX_MINIBUFFER; i++){
        minibuffer[i]=0b00000000;
    }
}

uint8_t DriverI2CMC24LCxxx::pageSize() {
	return _pageSize;
}

uint8_t DriverI2CMC24LCxxx::pinAddress() {
    return _pinAddress;
}

uint8_t DriverI2CMC24LCxxx::fullAddress() {
    return _fullAddress;
}

bool DriverI2CMC24LCxxx::readPage(uint16_t page_number, byte* data) {
    // Check page out of bounds of chip memory
    if (page_number < 0 || page_number > (I2CMC24LCXXX_PAGES - 1)) {
        return false;
    }

    // Ensure data will not overflow past end of page
    if (sizeof(data) > _pageSize) {
        return false;
    }

    bool status = false;

    uint8_t chunk = _pageSize / I2CMC24LCXXX_MINIBUFFER;
    uint16_t address_base = page_number * _pageSize;

    uint16_t lower_address=0;
    uint8_t offset = 0;
    uint8_t location = 0;
	
    for (uint8_t itr=0; itr<chunk; itr++) {
		offset = I2CMC24LCXXX_MINIBUFFER * itr;
		lower_address = address_base + offset;
		status = bufferedPageRead(lower_address);
        if (!status) {
            return status;
        }
        location = I2CMC24LCXXX_MINIBUFFER + offset;
		for (uint8_t index=offset; index<location; index++) {
			data[index] = minibuffer[index-offset];
		}
    }

    return true;
}

bool DriverI2CMC24LCxxx::writePage(uint16_t page_number, byte* data) {
    // Check page out of bounds of chip memory
    if (page_number < 0 || page_number > (I2CMC24LCXXX_PAGES - 1)) {
        return false;
    }

    // Ensure data will not overflow past end of page
    if (sizeof(data) > _pageSize) {
        return false;
    }

    bool status = false;

    uint8_t chunk = _pageSize / I2CMC24LCXXX_MINIBUFFER;
    uint16_t address_base = page_number * _pageSize;

    uint16_t lower_address=0;
    uint8_t offset = 0;
    uint8_t location = 0;
    
    for (uint8_t itr=0; itr<chunk; itr++) {
		offset = I2CMC24LCXXX_MINIBUFFER * itr;
		lower_address = address_base + offset;
        location = I2CMC24LCXXX_MINIBUFFER + offset;
		for (uint8_t index=offset; index<location; index++) {
			minibuffer[index-offset] = data[index];
		}
		status = bufferedPageWrite(lower_address);
        if (!status) {
            return status;
        }
	}

    return true;
}

bool DriverI2CMC24LCxxx::bufferedPageRead(uint16_t address) {
    // Make sure the address matches a page boundry
    if ((address + 1) % _pageSize) {
        return false;
    }

    uint8_t ct = 0;

    Wire.beginTransmission(_fullAddress);

    //32768 bytes... addressed by 2 bytes (16bits)
    Wire.write(address >> 8);   // MSB write upper 8 bits to wire (by bit shifting to right)
    Wire.write(address & 0xFF); // LSB write lower 8 bits to wire (by bit & by 0xFF)

    Wire.endTransmission();
    
    //request 1 byte from EEPROM (memory location sent above)
    Wire.requestFrom(_fullAddress, (uint8_t)1);
    while (Wire.available()) {
        minibuffer[ct] = Wire.read();
        ct++;
    }

    return true;
}

bool DriverI2CMC24LCxxx::bufferedPageWrite(uint16_t address) {
    // Make sure the address matches a page boundry
    if ((address + 1) % _pageSize) {
        return false;
    }

	uint8_t ct=0;

    Wire.beginTransmission(_fullAddress);

    //32768 bytes... addressed by 2 bytes (16bits)
    Wire.write(address >> 8);   // MSB write upper 8 bits to wire (by bit shifting to right)
    Wire.write(address & 0xFF); // LSB write lower 8 bits to wire (by bit & by 0xFF)

    while(ct < 16) {
		Wire.write(minibuffer[ct]);
		ct++;
    }

    Wire.endTransmission();

    // Wait max write time of chip
	delay(5);

    return true;
}

bool DriverI2CMC24LCxxx::erase() {
    bool status = false;

    byte values[_pageSize];
    uint8_t currentPage = 0;

    for (uint8_t i=0; i<_pageSize; i++) {
        values[i] = 0x00;
    }

    for (uint8_t i=0; i<I2CMC24LCXXX_PAGES; i++) {
        status = writePage(i, values);
        if (!status) {
            return status;
        }
    }

    return true;
}

#if INCLUDE_TEST_CHIP
bool DriverI2CMC24LCxxx::testChip() {
    byte values[_pageSize];
    uint8_t currentPage = 0;
    bool status = false;

    // Erase whole chip
    if (DEBUG) {
        Serial.println("DriverI2CMC24LCxxx::testChip: Erasing whole of storage");
    }
    status = erase();
    if (!status) {
        return status;
    }

    // Delay for good measure
    delay(100);

    // Verify chip is still erased
    if (DEBUG) {
        Serial.println("DriverI2CMC24LCxxx::testChip: Verifying erase took");
    }

    currentPage = 0;
    for (uint8_t i=0; i<_pageSize; i++) {
        values[i] = 0x79;
    }
    
    for (uint8_t i=0; i<I2CMC24LCXXX_PAGES; i++) {
        status = readPage(i, values);
        if (!status) {
            return status;
        }
        for (uint8_t j=0; j<_pageSize; j++) {
            if (values[j] != 0x00) {
                return false;
            }
        }
    }

    // Delay for good measure
    delay(100);

    // Write 0x79 to chip
    if (DEBUG) {
        Serial.println("DriverI2CMC24LCxxx::testChip: Writing 0x79 to whole of chip");
    }
    currentPage = 0;
    for (uint8_t i=0; i<_pageSize; i++) {
        values[i] = 0x79;
    }

    for (uint8_t i=0; i<I2CMC24LCXXX_PAGES; i++) {
        status = writePage(i, values);
        if (!status) {
            return status;
        }
    }

    // Delay for good measure
    delay(100);

    // Verify values are still 0x79
    if (DEBUG) {
        Serial.println("DriverI2CMC24LCxxx::testChip: Verifying 0x79 took");
    }

    currentPage = 0;
    for (uint8_t i=0; i<_pageSize; i++) {
        values[i] = 0x00;
    }
    
    for (uint8_t i=0; i<I2CMC24LCXXX_PAGES; i++) {
        status = readPage(i, values);
        if (!status) {
            return status;
        }
        for (uint8_t j=0; j<_pageSize; j++) {
            if (values[j] != 0x79) {
                return false;
            }
        }
    }

    // Delay for good measure
    delay(100);

    // Erase whole chip
    if (DEBUG) {
        Serial.println("DriverI2CMC24LCxxx::testChip: Erasing whole of storage");
    }
    status = erase();
    if (!status) {
        return status;
    }

    // Delay for good measure
    delay(100);

    // Verify chip is still erased
    if (DEBUG) {
        Serial.println("DriverI2CMC24LCxxx::testChip: Verifying erase took");
    }

    currentPage = 0;
    for (uint8_t i=0; i<_pageSize; i++) {
        values[i] = 0x79;
    }
    
    for (uint8_t i=0; i<I2CMC24LCXXX_PAGES; i++) {
        status = readPage(i, values);
        if (!status) {
            return status;
        }
        for (uint8_t j=0; j<_pageSize; j++) {
            if (values[j] != 0x00) {
                return false;
            }
        }
    }
}
#endif