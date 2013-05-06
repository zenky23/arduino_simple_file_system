To Do
=====

Below is a list of items that still need to be addressed

Examples
========

- Sketch that implements a test for each implemented driver

DriverSPIMC23x256
=================

- Paged mode of operation needs to be looked into and implemented

DriverI2CMC24LCxxx
==================

- Write protection polling
- Write protection pin with on/off toggle (default state set via constructor)
-- This requires the WP chip pin to be hooked up to an arduino pin 
-- configurable pin via constructor?
- Write leveling trick: http://arduino.cc/forum/index.php/topic,127174.msg956551.html#msg956551
- Need to do read/write similar to the DriverSPIMC23x256 driver instead of page based
- Move page read/write methods to private methods?
- Add method that returns write protection status
- Enabled write protection check in all write methods

![Creative Commons Attribution-ShareAlike 3.0 Unported (CC BY-SA 3.0)](https://raw.github.com/mcrosson/arduino_simple_file_system/master/cc-by-sa.png)