// Adapted from
// http://ucexperiment.wordpress.com/2013/02/23/interfacing-microchip-23k256-32kb-sram-spi-memory-to-the-arduino/

#include <SPI.h>
 
//SRAM opcodes
#define RDSR 5
#define WRSR 1
#define READ 3
#define WRITE 2
 
uint8_t SpiRAMRead8(uint16_t address) {
  uint8_t read_byte;
  SPI.transfer(4, READ, SPI_CONTINUE);
  SPI.transfer(4, (char)(address >> 8), SPI_CONTINUE);
  SPI.transfer(4, (char)address, SPI_CONTINUE);
  read_byte = SPI.transfer(4, 0xFF);
  return read_byte;
}
 
void SpiRAMWrite8(uint16_t address, uint8_t data_byte) {
  SPI.transfer(4, WRITE, SPI_CONTINUE);
  SPI.transfer(4, (char)(address >> 8), SPI_CONTINUE);
  SPI.transfer(4, (char)address, SPI_CONTINUE);
  SPI.transfer(4, data_byte);
}
 
void setup(void) {
  uint16_t addr;
  uint8_t i;
 
  Serial.begin(57600);
  SPI.begin(4);
  SPI.setClockDivider(4, 6); // 14mhz
  int dataByte = 255;
  for (addr=0; addr<32; addr++) {
    Serial.print("Write: ");
    SpiRAMWrite8(addr, (uint8_t)dataByte);
    dataByte--;
    Serial.print("Addr: ");
    Serial.print(addr);
    i = SpiRAMRead8(addr);
    Serial.print(" | Read: ");
    Serial.println((uint16_t)i);
  }
}
 
void loop() {
}
