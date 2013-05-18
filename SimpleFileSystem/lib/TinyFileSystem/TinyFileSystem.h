#ifndef TinyFileSystem_h
#define TinyFileSystem_h

#ifndef EEPROMSize 
#define EEPROMSize 1024			// The size of EEPROM in bytes
#endif

#ifndef MaxFilenameSize 
#define MaxFilenameSize 16		// The maximum length of a filename
#endif

#ifndef MaxFiles 
#define MaxFiles 5				// How many files can a user write
#endif

class TinyFileSystem 
{
private:

public:
	
	TinyFileSystem();
	static long GetEEPROMSize();
	// This number indicates the address of the Start of the data storage
	static long GetStartOfDataAddress();
	// Erases EEPROM
	static void EraseEEPROM();
	// Return's true if file exist
	static bool FileExists(String filename);
	// Return's the start address of the file
	static long* GetFileAddress(String filename);
	static uint8_t ReadFromEEPROM(int address);
	static String ReadStringBlockFromEEPROM(int addressStart, int addressEnd);
	static void WriteToEEPROM(int address, uint8_t value);
};

#endif
