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

// Define TinyFileSystem Variables
#define EEPROMSize 1024		// The size of EEPROM in bytes
#define MaxFilenameSize 16  // The maximum length of a filename
#define MaxFiles 5			// How many files can a user write

#include <TinyFileSystem.h>
#include <TinyFile.h>

#include <EEPROM.h>

void setup() {
	int i;
	Serial.begin(9600);
	while (!Serial);

	// Erase EEPROM
	TinyFileSystem::EraseEEPROM();
	
	// Create new file
	TinyFile myFile("File1");
	myFile.Save("File1 Data");

	TinyFile myFile2("File2");
	myFile2.Save("File2 Data");

	TinyFile myFile3("File3");
	myFile3.Save("File 3 Data");
	
	// Load a file
	TinyFile fileToLoad;
	if(TinyFileSystem::FileExists("File2")) {
		String data = fileToLoad.Load("File2");
		Serial.println("Previus data: " + data);

		fileToLoad.Save("Small!");
		Serial.println("New data: " + fileToLoad.Load("File2"));
	}

	for (i = 0; i < 1024; i++) {
		char value = char(EEPROM.read(i));
		Serial.print(i);
		Serial.print("- - - - - - - - -");
		if (value > char(13)) {
			Serial.print(value);
		}
		Serial.println();
	}
}

void loop() { }
