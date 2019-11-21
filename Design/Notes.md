Hardware Focused Notes
----------------------
- Simpler design to prevent over using ram/storage on AVR and other 8 bit microcontrollers
- Generally information is stored to an eeprom as a set of offsets with the layout being managed internal to a firmware
- The storage is just a set of "chunks" (ie. files) for each item you need to store
- There is generally an upper bound on what the hardware can handle and the chunks can be fixed sizes based on the upper bound
- Metadata about chunks can be stored in a special chunk or at the head/tail of a given chunk

CS Focused Notes
----------------
- Filesystem object / singleton
- Drivers
- Drivers do _NOT_ implement filesytem but have standard API
- Single filesystem per chip
	- Possibly multiple chips per filesystem (long-term)
- 2 byte identifier instead of filename which gives 65k files per filesystem
- Filenames would be a map stored as it's own file on the filesystem
- Implement seek and partial reads/writes long-term
- Files are variable sized compared to each other
- Use FAT (file allocation table) with fixed size
- Fixed fat size is proportinoal to total storage size
	- Percentage based
	- Logarithmic scale
	- Keep two byte id for files -> gives practical upper bound
- Use 1 byte file identifier for extremely small storage chips (<=32k)
- Use 2 byte file identifier for larger storage chips (>=64k)
- Want / need dynamically sized files
- Need inode implementation for dynamically sized files
- FAT stores
	- file id
	- file size
	- inode #
- Inodes are files in the filesystem and allocated as such
- Inode stores vector / list of block addresses / locations and size
	- Store blocks list in order
- 3 bytes per inode entry
	- 2 bytes for block address
	- 1 byte for # blocks stored
- Filesystem singleton as pure static methods, c style
- Drivers are objects with abstract class for API
- File is a pointer and size passed as parameters to methods
- FileIO methods are pure static, c style

Pre-existing filestems that may be worth looking closer at
----------------------------------------------------------
- http://www.nohau.co.uk/Suppliers/HCC-Embedded/tiny.htm
- http://ultra-embedded.com/?fat_filelib
- http://freedos-32.sourceforge.net/lean/downloads.php
