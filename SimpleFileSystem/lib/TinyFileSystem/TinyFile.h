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

#ifndef TinyFile_h
#define TinyFile_h

#include "TinyFileSystem.h"

class TinyFile  {
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
