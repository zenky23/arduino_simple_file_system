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

#ifndef DriverSPIMC23X256_H
#define DriverSPIMC23X256_H

// The chip testing method can be rather large, do not build it by default
#ifndef INCLUDE_TEST_CHIP
#define INCLUDE_TEST_CHIP true
#endif

#ifndef DEBUG
#define DEBUG true
#endif

class DriverSPIMC23x256 {
public:
	DriverSPIMC23x256(byte ncsPin);
	bool read(uint16_t address, byte *buffer, uint16_t size);
	bool write(uint16_t address, byte *buffer, uint16_t size);
	bool isPaged() { return false; }
	bool isWriteProtected() { return false; }
	bool erase();
#if INCLUDE_TEST_CHIP
	bool testChip();
#endif

private:
	byte _ncsPin;
	byte _buf[4];
	void select(void);
	void deselect(void);
	byte transfer(byte length);
	bool write(uint16_t address, byte *data); // Needed for erase
#if INCLUDE_TEST_CHIP // Not needed outside of test chip
	bool read(uint16_t address, byte *data);
#endif
};

#endif
