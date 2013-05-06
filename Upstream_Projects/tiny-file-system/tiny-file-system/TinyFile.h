#ifndef TinyFile_h
#define TinyFile_h
#include <Arduino.h>
#include <avr\eeprom.h>
#include "TinyFileSystem.h"


class TinyFile 
{
	private:
		String fName;
		String fData;
		unsigned long fAddressStartIndex;
		unsigned long fAddressEndIndex;
		
		String FixFileName(String filename);
		void SaveFileIndex(String fileIndexData);

	public:
		TinyFile();
		TinyFile(String filename);	
		void Save(String data);
		String Load(String filename);
};

// Constructor
TinyFile::TinyFile()
{
	Serial.print("Start of data address:");
	Serial.println(TinyFileSystem::GetStartOfDataAddress());
}

// Constructor
// filename is the name of the new file
TinyFile::TinyFile(String fileName)
{
	TinyFile();
	fName = FixFileName(fileName);
}

// Fix file name
String TinyFile::FixFileName(String filename)
{
	int i;
	if(filename.length() < MaxFilenameSize)
	{
		for(i = filename.length(); i <MaxFilenameSize; i ++)
		{
			filename +=' ';
		}
	}
	else
	{
		filename = filename.substring(0,MaxFilenameSize);
	}

	return filename;
}



// Save the file to EEPROM
void TinyFile::Save(String data)
{
	fData = data;
	bool fileExists = TinyFileSystem::FileExists(fName);
	int maximumAddressLength = TinyTools::GetNumberDigits(TinyFileSystem::GetEEPROMSize());
	long i;

	if(fileExists) // File exists Update file
	{
		long * fileAddress = TinyFileSystem::GetFileAddress(fName);
		long addressStartIndex = fileAddress[0];
		long addressEndIndex = fileAddress[1];

		long previusDataLength = addressEndIndex - addressStartIndex;
		Serial.print("Previus data length: " );
		Serial.println(previusDataLength);

		long newDataLength = data.length();
		Serial.print("New data length: " );
		Serial.println(newDataLength);

		long dataDifference = 0;
		if(newDataLength > previusDataLength)
		{
			dataDifference = newDataLength - previusDataLength;
			Serial.println("New data are bigger");		

		}
		else
		{
			dataDifference = previusDataLength - newDataLength;
			Serial.println("New data are smaller");
			// Write the file
			for(i = addressStartIndex+ dataDifference; i < addressEndIndex; i++)
			{
				TinyFileSystem::WriteToEEPROM(i,fData.charAt(i));
			}

			// Move all previus files left
			/*for(i = addressStartIndex; i > TinyFileSystem::GetStartOfDataAddress(); i --)
			{
				char tmp = TinyFileSystem::ReadFromEEPROM(i);
				TinyFileSystem::WriteToEEPROM(i-dataDifference,tmp);
				TinyFileSystem::WriteToEEPROM(i,0);
			}*/
		}

		// Delete previus data
		/*Serial.println("Erasing ..." + fName);
		for(int i=addressStartIndex; i <=addressEndIndex; i++)
		{
			TinyFileSystem::WriteToEEPROM(i,0);
		}*/
		
		// Close the gap with other files
		// Check if file is has other files written previusly in the EEPROM order


	}
	else // Create new file
	{
		// Find free space to save the new file
		long positionToStartWriting = -1;
		for(i = EEPROMSize-1; i > TinyFileSystem::GetStartOfDataAddress(); i --)
		{
			if(TinyFileSystem::ReadFromEEPROM(i) == 0)
			{
				positionToStartWriting = i;
				break;
			}
		}

		if(positionToStartWriting > -1)											// Free space found. Write file to EEPROM
		{
			fAddressStartIndex = positionToStartWriting - fData.length()+1;		// Start of data EEPROM address
			fAddressEndIndex = fAddressStartIndex + fData.length()-1;			// End of data EEPROM address
		
			for(i = 0; i< fData.length(); i++)									// Write data to EEPROM
			{
				TinyFileSystem::WriteToEEPROM(fAddressStartIndex+i, fData.charAt(i));
			}
		
	

			// Create file index data
			String indexData = fName;
	
			String strAddressStartIndex(fAddressStartIndex);					// Fix start address digits
			if(strAddressStartIndex.length() < maximumAddressLength)
			{
				for (i=0; i <maximumAddressLength - strAddressStartIndex.length(); i++)
				{
					strAddressStartIndex = "0" + strAddressStartIndex;
				}
			}
			indexData += strAddressStartIndex;

			String strAddressEndIndex(fAddressEndIndex);						// Fix end address digits
			if(strAddressEndIndex.length() < maximumAddressLength)
			{
				for (i=0; i <maximumAddressLength - strAddressEndIndex.length(); i++)
				{
					strAddressEndIndex = "0" + strAddressEndIndex;
				}
			}
			indexData += strAddressEndIndex;

			indexData += String(char(1));

			// Save file index
			SaveFileIndex(indexData);
		}
	}
}

void TinyFile::SaveFileIndex(String fileIndexData)
{
	int i;
	int maximumAddressLength = TinyTools::GetNumberDigits(TinyFileSystem::GetEEPROMSize());
	
	// Find last file index from the begining of EEPROM
	// and then write the new file index
	int positionToStartWriting = -1;
	for(i = 0; i < TinyFileSystem::GetStartOfDataAddress(); i += MaxFilenameSize + 1 + (2*maximumAddressLength))
	{
		if(TinyFileSystem::ReadFromEEPROM(i) == 0)
		{
			positionToStartWriting = i;
			break;
		}
	}

	if(positionToStartWriting>-1)
	{
		for(i=0; i <fileIndexData.length(); i ++)
		{
			TinyFileSystem::WriteToEEPROM(positionToStartWriting+i , fileIndexData.charAt(i));
		}
	}
}

// Load a new file
String TinyFile::Load(String filename)
{
	fName = filename;
	filename = FixFileName(filename);
	
	int i;
	int maximumAddressLength = TinyTools::GetNumberDigits(TinyFileSystem::GetEEPROMSize());


	// Check if file exists
	for(i = 0; i < TinyFileSystem::GetStartOfDataAddress(); i += MaxFilenameSize + 1 + (2*maximumAddressLength))
	{
		String portionToCheck = TinyFileSystem::ReadStringBlockFromEEPROM(i, i + MaxFilenameSize + 1 + (2 * maximumAddressLength));
		
		if(portionToCheck.startsWith(filename)) // File found
		{
			String startIndexStr = portionToCheck.substring(MaxFilenameSize, MaxFilenameSize + maximumAddressLength);
			String endIndexStr = portionToCheck.substring(MaxFilenameSize + maximumAddressLength, MaxFilenameSize+2*maximumAddressLength);
			
			return(TinyFileSystem::ReadStringBlockFromEEPROM(startIndexStr.toInt(), endIndexStr.toInt()));
		}
	}

	return NULL;
}


#endif














