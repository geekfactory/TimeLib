# GeekFactory TimeLib Library #

This library contains Unix time routines and can be used as a software RTC on various microcontroller platforms. The library is itself very small in size and can be used in 8, 16 and 32 bit devices, including the AVR based arduino boards.

This work is based in Paul Stoffregen libraries for the Arduino platform.

## Basic library usage ##

In order to use the library we need to call a function to set the time. Internally the time is kept in the unix format, so the library expects you to provide it in that format. Functions are provided for converting between human readable time/date and the Unix time.

The following snippet shows how to convert and set the TimeLib clock:

```c
// Structure that holds human readable time information;
struct tm tinfo;
// Variable to hold Unix time
timelib_t initialt;

// Set time manually to 13:55:30 Jan 1st 2014
tinfo.tm_year = 14;
tinfo.tm_mon = 1;
tinfo.tm_mday = 1;
tinfo.tm_hour = 13;
tinfo.tm_min = 55;
tinfo.tm_sec = 30;

// Convert time structure to timestamp
initialt = timelib_make(&tinfo);

// Set system time counter
timelib_set(initialt);

// Configure the library to update every day
timelib_set_provider(time_provider, TIME_SECS_PER_DAY);
```

## Basic library example using arduino ##

```cpp

```

## Project Objectives ##

Our library should fulfill the following goals:

* Code should be portable to other platforms
* Library should be compact and have minimal dependencies
* Library should not include C++ code


## Supported devices ##

This library was developed/tested on the following boards:

* PIC24HJ128GA504 devices using MPLAB X and XC16 compilers
* Arduino UNO R3
* Arduino Mega 2560 R3
* Basic testing on ESP8266 boards

The library is meant to work in other CPU architectures where a C compiler is available, please tell us about your experience if you try it in other platforms.

## Contact me ##

* Feel free to write for any inquiry: ruben at geekfactory.mx
* Check our website: https://www.geekfactory.mx
