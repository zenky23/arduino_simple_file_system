#ifndef TinyFile_h
#define TinyFile_h

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

#endif
