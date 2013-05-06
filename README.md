Arduino Simple Filesystem
=========================

This project is a simple filesystem meant to be used on top of an EEPROM or SRAM chip.  It provides basic functionality for files, complex data type storage and some other niceities without the overhead.  It can likely work with an SD card as a raw storage device, but its use is geared towards EEPROM's and SRAM.

Arduino Compatibility
=====================

This library has been tested on an Arduio Due as well as Arduino Leonardo.  Implementers can assume things will "just work" on the avr Arduino devices and the Due.  There are some practical differences between the avr and arm Arduino's that this library addresses behind the scenes.

For those implementing a new driver, data type sizes are different between avr and arm. For example an int is twice as large on arm compared to avr.  The pre-existing drivers address this difference, your driver should also take this difference into account.

Licensing
=========

All documentation and graphical content contained on this website and within the sources unless otherwise noted is licensed under the 
[Creative Commons Attribution-ShareAlike 3.0 Unported (CC BY-SA 3.0)](http://creativecommons.org/licenses/by-sa/3.0/deed.en_US) license.

Source code is licensed under the [Apache License, Version 2.0](http://www.apache.org/licenses/LICENSE-2.0.html) unless otherwise specified.

Key Point Regarding Library Size
================================

If you are concerned about the size of the library given it may have more chips supported than you need:  You can delete any drivers (or move them out of the way) from the project. Drivers are prefixed with "Driver" and can be safely removed prior to build.

Each driver also has a testChip method that is not compiled into the final binary by default.  If you go into the drivers code, you can enable the method to be build for initial testing of the chip.  After that you'll want to disable the method as they can be quite large and should not generally be used beyond an initial test.

API / Library Structure
=======================

The structure of the library is as follows:

FS <> IO API <> Drivers (I2C / SPI / Chip)

Because it is a layered approach, the underlying storage (EEPROM/SRAM) and any chip specific details are abstracted away from the program being developed.  All you need to ensure is that there is a driver available for the chip being used for storage.  I have implemented support for a few different chips already, you'll want to use these implementations as a template if adding support for a new chip.

The drivers are named according to the following:

DriverProtocolChip where "Driver" is a static prefix, "Protocol" is I2C, SPI or similar and Chip is "MC23x256".  For example the driver for the Microchip 23K256 chip is "DriverSPIMC23x256".

Inspiration / Upstream
======================

The following projects/guides/snippets were used in the development of this library.  They provided me with a lot pre-existing solutions and inspiration.

- http://playground.arduino.cc/code/I2CEEPROM
- http://ucexperiment.wordpress.com/2013/02/23/interfacing-microchip-23k256-32kb-sram-spi-memory-to-the-arduino/
- http://arduino.cc/forum/index.php/topic,162242.0.html
- http://playground.arduino.cc/Code/EEPROMex
- http://playground.arduino.cc/Code/EEPROMWriteAnything
- https://bitbucket.org/trunet/spieeprom/src
- https://bitbucket.org/spirilis/spieep/src
- https://code.google.com/p/tiny-file-system/
- https://code.google.com/p/arduino-edb/
- https://code.google.com/p/arduino-dblib/
- http://arduino.cc/forum/index.php/topic,162242.msg1228984.html#msg1228984
- http://code.google.com/p/arms22/
- https://github.com/mikeneiderhauser/I2C-EEPROM-Arduino


![Creative Commons Attribution-ShareAlike 3.0 Unported (CC BY-SA 3.0)](https://raw.github.com/mcrosson/arduino_simple_file_system/master/cc-by-sa.png)