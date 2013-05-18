/*
   Copyright 2013 Michael Crosson
 
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

#ifndef SimpleFileSystem_h
#define SimpleFileSystem_h

#ifndef EEPROMSize 
#define EEPROMSize 1024			// The size of EEPROM in bytes
#endif

#ifndef MaxFilenameSize 
#define MaxFilenameSize 16		// The maximum length of a filename
#endif

#ifndef MaxFiles 
#define MaxFiles 5				// How many files can a user write
#endif

class SimpleFileSystem  {
public:
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
