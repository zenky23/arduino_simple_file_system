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


// Nuke below includes
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h> 

// General info about the files stored in a filesystem
struct simplefile {
	uint8_t startAddress;
	uint8_t allocatedBytes;
	uint8_t sizeBytes;
	uint8_t* data;
};

// Simple way to interact with the filesystem
class simplefilesystem {
private:
	// The files on the filesystem
	simplefile* files;
	// The number of files
	uint8_t numberFiles;

public:
	// Some basic setup / house keeping if creating a dynamic set of files
	simplefilesystem(uint8_t number) {
		numberFiles = number;
		files = (simplefile*) malloc(numberFiles * sizeof(simplefile));
	}

	// Some basic setup / house keeping if using a static list of files
	simplefilesystem(simplefile* theFiles) {
		files = theFiles;
		numberFiles = sizeof(files) / sizeof(simplefile);
	}

	// Return the number of files
	int numberOfFiles() {
		return numberFiles;
	}

	// Get the file pointer so information about file can be populated
	simplefile* getFile(uint8_t index) {
		return &files[index];
	}

	// Set the file at a given position
	void setFile(uint8_t index, simplefile aFile) {
		files[index] = aFile;
	}

	// Return a byte array
	uint8_t* readFile(uint8_t index) {
		// Use struct at the given index to figure out how to read the file and return it as a pointer to an array of bytes
	}

	// Write the given byte array to storage
	void writeFile(uint8_t index, uint8_t* bytes) {
		// Use struct at the given index to figure out how to write the file
		// Update sizeBytes as appropriate
		// Write metadata to beginning of file
	}
};


// Nuke the below once the above proto code is vetted properly
int main(int argc, const char* argv[] ) {
	printf("\nRunning tests\n\n");

	printf("Creating file struct");
	simplefile test;
	test.startAddress = 1;
	test.allocatedBytes = 2;
	test.sizeBytes = 3;

	printf("Verify values of new struct\n");
	printf("Start address (1): %d\n", test.startAddress);
	printf("Allocated bytes (2): %d\n", test.allocatedBytes);
	printf("Size bytes (3): %d\n", test.sizeBytes);

	printf("Creating new filesystem object with just 10 files\n");
	simplefilesystem* aFilesystem = new simplefilesystem(12);

	printf("Populating new filesystem\n");
	int j = 0;
	for (int i=0; i<10; i++) {
		simplefile test2;
		j++;
		test2.startAddress = j;
		j++;
		test2.allocatedBytes = j;
		j++;
		test2.sizeBytes = j;
		aFilesystem->setFile(i, test2);
	}

	printf("Enumerating new filesystem\n");
	printf("  -- Each value should be previous + 1\n");
	for (int i=0; i<10; i++) {
		simplefile* aFile = aFilesystem->getFile(i);
		printf("%d: startAddress: %d\n", i, aFile->startAddress);
		printf("%d: allocatedBytes: %d\n", i, aFile->allocatedBytes);
		printf("%d: sizeBytes: %d\n", i, aFile->sizeBytes);
	}

	printf("Creating a set of files for next tests\n");
	simplefile* testFiles2 = (simplefile*) malloc(5 * sizeof(simplefile));
	j = 0;
	for (int i=0; i<5; i++) {
		simplefile test3;
		j--;
		test3.startAddress = j;
		j--;
		test3.allocatedBytes = j;
		j--;
		test3.sizeBytes = j;
		testFiles2[i] = test3;
	}

	printf("Creating new filesystem from preset list of files\n");
	simplefilesystem* testFilesystem2 = new simplefilesystem(testFiles2);

	printf("New filesystem # files: %d\n", testFilesystem2->numberOfFiles());

	printf("Enumerating new filesystem\n");
	printf("  -- Each value should be previous - 1\n");
	for (int i=0; i<5; i++) {
		simplefile* aFile = testFilesystem2->getFile(i);
		printf("%d: startAddress: %d\n", i, aFile->startAddress);
		printf("%d: allocatedBytes: %d\n", i, aFile->allocatedBytes);
		printf("%d: sizeBytes: %d\n", i, aFile->sizeBytes);
	}
}