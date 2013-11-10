/*
Copyright (c) 2013, Michael Crosson
 All rights reserved.
 
 Redistribution and use in source and binary forms, with or without modification, are 
 permitted provided that the following conditions are met:
 
 1. Redistributions of source code must retain the above copyright notice, this list of 
 conditions and the following disclaimer.
 
 2. Redistributions in binary form must reproduce the above copyright notice, this list 
 of conditions and the following disclaimer in the documentation and/or other materials 
 provided with the distribution.
 
 3. Neither the name of the <ORGANIZATION> nor the names of its contributors may be used 
 to endorse or promote products derived from this software without specific prior 
 written permission.
 
 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS 
 OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
 MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE 
 COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, 
 EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF 
 SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) 
 HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR 
 TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, 
 EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

// Adapted from http://ucexperiment.wordpress.com/2013/02/23/interfacing-microchip-23k256-32kb-sram-spi-memory-to-the-arduino/

#include <SPI.h>

// SRAM opcodes
#define WRITE     0b00000010 // Write operation
#define READ      0b00000011 // Read operation
#define RDSR      0b00000101 // Read status register
#define WRSR      0b00000001 // Write status register
#define MODE_PAGE 0b10000000 // Page mode status byte
#define MODE_BYTE 0b00000000 // Byte mode status byte

// Read status register
uint8_t SpiRAMReadStsReg() {
  SPI.transfer(4, RDSR, SPI_CONTINUE);
  return SPI.transfer(4, 0xFF);
}

// Write status register
void SpiRAMWriteStsReg(uint8_t mode) {
  SPI.transfer(4, WRSR, SPI_CONTINUE);
  SPI.transfer(4, mode);
}

// Read 8 bits from the given address (byte mode)
uint8_t SpiRAMRead8(uint16_t address) {
  SPI.transfer(4, READ, SPI_CONTINUE);
  SPI.transfer(4, (uint8_t)(address >> 8), SPI_CONTINUE);
  SPI.transfer(4, (uint8_t)address, SPI_CONTINUE);
  return SPI.transfer(4, 0xFF);
}

// Write 8 bits to the given address (byte mode)
void SpiRAMWrite8(uint16_t address, uint8_t data_byte) {
  SPI.transfer(4, WRITE, SPI_CONTINUE);
  SPI.transfer(4, (uint8_t)(address >> 8), SPI_CONTINUE);
  SPI.transfer(4, (uint8_t)address, SPI_CONTINUE);
  SPI.transfer(4, data_byte);
}

void setup(void) {
  // Setup serial output
  Serial.begin(57600);

  // Setup SPI
  SPI.begin(4); // Chip select on Pin 4
  SPI.setClockDivider(4, 6); // Set SPI to run at 14mhz
  
  // Let SRAM settle -- just in case
  delay(10);
}

// Variables used during testing
uint8_t modeOfOperation;
uint8_t counter1;
uint8_t byteRead;

void loop() {
  Serial.println("Running tests");
  Serial.println("#############");
  
  Serial.println("Running mode set tests");
  
  // Get current mode of operation
  Serial.print("    Current mode: ");
  Serial.println(SpiRAMReadStsReg(), BIN);
  
  // Set page mode
  Serial.print("    Setting page mode (");
  Serial.print(MODE_PAGE, BIN);
  Serial.println(")");
  SpiRAMWriteStsReg(MODE_PAGE);
  modeOfOperation = SpiRAMReadStsReg();
  Serial.print("    Current mode: ");
  Serial.println(modeOfOperation, BIN);
  // Verify mode was set correctly (assert)
  if (modeOfOperation != MODE_PAGE) {
    Serial.println("    Mode set failed!");
  }
  
  // Set byte mode
  Serial.print("    Setting byte mode (");
  Serial.print(MODE_BYTE, BIN);
  Serial.println(")");
  SpiRAMWriteStsReg(MODE_BYTE);
  modeOfOperation = SpiRAMReadStsReg();
  Serial.print("    Current mode: ");
  Serial.println(modeOfOperation, BIN);
  // Verify mode was set correctly (assert)
  if (modeOfOperation != MODE_BYTE) {
    Serial.println("    Mode set failed!");
  }
  
  Serial.println("Running sequential mode tests");
  
  Serial.println("    Byte mode Write/Reads");
  Serial.println("    Writing 0 - 255 verifying each write with a read immediately following");
  SpiRAMWriteStsReg(MODE_BYTE); // Ensure we are in the proper mode
  counter1 = 0; // Set counter start
  Serial.print("    "); // Proper indentation for status line
  // Read/write each of the 32768 addresses
  for (uint16_t i; i<32768; i++) {
    SpiRAMWrite8(i, counter1); // Write a byte
    byteRead = SpiRAMRead8(i); // Read it for verification
    if (counter1 != byteRead) { // Verify written byte
      Serial.println("    Invalid read or write!");
    }
    if (i % 1024 == 0) { // Print status ever 1024 iterations to prevent console spew
      Serial.print("#");
    }
    // Resent counter at 255 (could just let the uint8_t overflow, but...)
    if (counter1 == 255) {
      counter1 = 0;
    }
    else {
      counter1++;
    }
  }
  Serial.println(); // Append new line after #'s indicating status
  
  // Delay before re-running tests
  delay(1000);
}

