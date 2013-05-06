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
  Adapted from arms22 (arms22 at gmail.com) 
  Microchip 23x256 SPI SRAM library for Arduino
  
  http://code.google.com/p/arms22/
*/

#include "Arduino.h"
#include <SPI.h>

#include "DriverSPIMC23x256.h"

#define SPIMC23x256_SIZE 262144 // Chip total size (bits)

// INSTRUCTION SET
#define SPIMC23x256_READ	0x03	// Read data from memory
#define SPIMC23x256_WRITE	0x02	// Write data to memory
#define SPIMC23x256_RDSR	0x05	// Read Status register
#define SPIMC23x256_WRSR	0x01	// Write Status register

// STATUS REGISTER
#define	SPIMC23x256_BYTE_MODE	0x00
#define	SPIMC23x256_PAGE_MODE	0x80
#define SPIMC23x256_SEQ_MODE	0x40

DriverSPIMC23x256::DriverSPIMC23x256(byte ncsPin)
	: _ncsPin(ncsPin) {
}

bool DriverSPIMC23x256::read(uint16_t address, byte *buffer, uint16_t size) {
	// Ensure we can read all of the data requested
	if (address + size > SPIMC23x256_SIZE || address < 0) {
		if (DEBUG) {
			Serial.println("DriverSPIMC23x256::read: FAILED: Overflow/Underflow");
		}
		return false;
	}

	_buf[0] = SPIMC23x256_WRSR;
	_buf[1] = SPIMC23x256_SEQ_MODE;
	transfer(2);
	select();
	SPI.transfer(SPIMC23x256_READ);
	SPI.transfer(address >> 8);
	SPI.transfer(address & 0xff);
	for(uint16_t i=0; i<size; i++){
		*buffer++ = SPI.transfer(0);
	}
	deselect();

	return true;
}

bool DriverSPIMC23x256::write(uint16_t address, byte *buffer, uint16_t size) {
	// Ensure we can write all of the data requested
	if (address + size > SPIMC23x256_SIZE || address < 0) {
		if (DEBUG) {
			Serial.println("DriverSPIMC23x256::write: FAILED: Overflow/Underflow");
		}
		return false;
	}

	_buf[0] = SPIMC23x256_WRSR;
	_buf[1] = SPIMC23x256_SEQ_MODE;
	transfer(2);
	select();
	SPI.transfer(SPIMC23x256_WRITE);
	SPI.transfer(address >> 8);
	SPI.transfer(address & 0xff);
	for(uint16_t i=0; i<size; i++){
		SPI.transfer(*buffer++);
	}
	deselect();

	return true;
}

bool DriverSPIMC23x256::erase() {
	bool blockWriteStatus = true;
	uint16_t address = 0x00;
	byte data = 0x00;
	uint16_t size = sizeof(byte);

	while (blockWriteStatus && address < SPIMC23x256_SIZE) {
		blockWriteStatus = write(address, &data);
		address += size;
	}

	return blockWriteStatus;
}

bool DriverSPIMC23x256::write(uint16_t address, byte *data) {
	// Ensure we can write all of the data requested
	if (address + sizeof(byte) > SPIMC23x256_SIZE || address < 0) {
		if (DEBUG) {
			Serial.println("DriverSPIMC23x256::write: FAILED: Overflow/Underflow");
		}
		return false;
	}

	_buf[0] = SPIMC23x256_WRSR;
	_buf[1] = SPIMC23x256_SEQ_MODE;
	transfer(2);
	select();
	SPI.transfer(SPIMC23x256_WRITE);
	SPI.transfer(address >> 8);
	SPI.transfer(address & 0xff);
	SPI.transfer(*data);
	deselect();

	return true;
}

#if INCLUDE_TEST_CHIP // Not needed outside of test chip
bool DriverSPIMC23x256::read(uint16_t address, byte *buffer) {
	// Ensure we can read all of the data requested
	if (address + sizeof(byte) > SPIMC23x256_SIZE || address < 0) {
		if (DEBUG) {
			Serial.println("DriverSPIMC23x256::read: FAILED: Overflow/Underflow");
		}
		return false;
	}

	_buf[0] = SPIMC23x256_WRSR;
	_buf[1] = SPIMC23x256_SEQ_MODE;
	transfer(2);
	select();
	SPI.transfer(SPIMC23x256_READ);
	SPI.transfer(address >> 8);
	SPI.transfer(address & 0xff);
	buffer[0] = SPI.transfer(0);
	deselect();

	return true;
}
#endif

void DriverSPIMC23x256::select(void) {
	digitalWrite(_ncsPin, LOW);
}

void DriverSPIMC23x256::deselect(void) {
	digitalWrite(_ncsPin, HIGH);
}

byte DriverSPIMC23x256::transfer(byte length) {
	byte ret,i;
	select();
	for(i=0; i<length; i++){
		ret = SPI.transfer(_buf[i]);
	}
	deselect();
	return ret;
}


// Don't build the test chip method unless it is explicitly asked for
#if INCLUDE_TEST_CHIP
bool DriverSPIMC23x256::testChip() {
	// The test process is:.
	//  Run the below tests writing one byte at a time
	//  Run the below tests writing the full RAM amount at a time
	// Tests
	//  Erase the whole of the storage and verify every byte written reads as 0x00
	//  Write 0x79 to the whole of the storage and verify every byte written reads as 0x79

	// Assume worst case as a default
	bool status = false;

	// Some temp / house keeping variables used below
	uint16_t address = 0x00;
	byte data = 0x00;
	uint16_t size = sizeof(byte);
	byte values[8];

	// Erase the SRAM
	if (DEBUG) {
		Serial.println("DriverSPIMC23x256::testChip: erasing SRAM");
	}
	status = erase();
	if (!status) {
		return status;
	}

	// Delay for good measure
	delay(100);

	// Reset values to what we need for the next step
	address = 0x00;
	data = 0xFF;

	// Verify the erased values remained 0x00
	if (DEBUG) {
		Serial.println("DriverSPIMC23x256::testChip: verifying erase");
	}
	while (status && address < SPIMC23x256_SIZE) {
		status = read(address, &data);
		if (!status || data != 0x00) {
			return false;
		}
		address += size;
	}

	// Make sure we didn't fail
	if (!status) {
		return status;
	}

	// Delay for good measure
	delay(100);

	// Reset values to what we need for the next step
	address = 0x00;
	data = 0x79;

	// Write 0x79 to the whole of the storage
	if (DEBUG) {
		Serial.println("DriverSPIMC23x256::testChip: writing 0x79 to SRAM");
	}
	while (status && address < SPIMC23x256_SIZE) {
		status = write(address, &data);
	}

	// Make sure all writes didn't fail
	if (!status) {
		return status;
	}

	// Delay for good measure
	delay(100);

	// Reset values to what we need for the next step
	address = 0x00;
	data = 0x00;

	// Verify 0x79 values remained 0x79
	if (DEBUG) {
		Serial.println("DriverSPIMC23x256::testChip: verifying 0x79");
	}
	while (status && address < SPIMC23x256_SIZE) {
		status = read(address, &data);
		if (!status || data != 0x79) {
			return false;
		}
		address += size;
	}

	// Make sure reads didn't fail
	if (!status) {
		return status;
	}

	// Delay for good measure
	delay(100);

	// Reset values to what we need for the next step
	address = 0x00;
	size = sizeof(values);
	for (uint8_t i=0; i<8; i++) {
		values[i] = 0x00;
	}

	// Write values to whole of SRAM
	if (DEBUG) {
		Serial.println("DriverSPIMC23x256::testChip: writing 0x00 in blocks");
	}
	while (status && address < SPIMC23x256_SIZE) {
		status = write(address, values, size);
		if (!status) {
			return false;
		}
		address += size;
	}

	// Make sure all writes didn't fail
	if (!status) {
		return status;
	}

	// Delay for good measure
	delay(100);

	// Reset values to what we need for the next step
	address = 0x00;
	size = sizeof(values);
	for (uint8_t i=0; i<8; i++) {
		values[i] = 0x79;
	}

	// Verify 0x00 values remained 0x00
	if (DEBUG) {
		Serial.println("DriverSPIMC23x256::testChip: verifying 0x00");
	}
	while (status && address < SPIMC23x256_SIZE) {
		status = read(address, values, size);
		if (!status) {
			return false;
		}
		for (uint8_t i=0; i<8; i++) {
			if (values[i] != 0x00) {
				return false;
			}
		}

		address += size;
	}

	// Make sure reads didn't fail
	if (!status) {
		return status;
	}

	// Delay for good measure
	delay(100);

	// Reset values to what we need for the next step
	address = 0x00;
	size = sizeof(values);
	for (uint8_t i=0; i<8; i++) {
		values[i] = 0x79;
	}

	// Write values to whole of SRAM
	if (DEBUG) {
		Serial.println("DriverSPIMC23x256::testChip: writing 0x79 in blocks");
	}
	while (status && address < SPIMC23x256_SIZE) {
		status = write(address, values, size);
		if (!status) {
			return false;
		}
		address += size;
	}

	// Make sure all writes didn't fail
	if (!status) {
		return status;
	}

	// Delay for good measure
	delay(100);

	// Reset values to what we need for the next step
	address = 0x00;
	size = sizeof(values);
	for (uint8_t i=0; i<8; i++) {
		values[i] = 0x00;
	}

	// Verify 0x00 values remained 0x00
	if (DEBUG) {
		Serial.println("DriverSPIMC23x256::testChip: verifying 0x00");
	}
	while (status && address < SPIMC23x256_SIZE) {
		status = read(address, values, size);
		if (!status) {
			return false;
		}
		for (uint8_t i=0; i<8; i++) {
			if (values[i] != 0x79) {
				return false;
			}
		}

		address += size;
	}

	// Make sure reads didn't fail
	if (!status) {
		return status;
	}

	// Delay for good measure
	delay(100);
}
#endif