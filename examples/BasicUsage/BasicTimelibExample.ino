#include "TimeLib.h"

// Structure that holds human readable time information;
struct tm_t tinfo;
// Local time to get
time_t now, initialt;
// Store last time we sent the information
uint32_t last = 0;

void setup()
{
	// Configure serial port
	Serial.begin(9600);
	while (!Serial);

	// Set time manually to 13:55:30 Jan 1st 2014
	tinfo.tm_year = 14;
	tinfo.tm_mon = 1;
	tinfo.tm_mday = 1;
	tinfo.tm_hour = 13;
	tinfo.tm_min = 55;
	tinfo.tm_sec = 30;
	// Convert time structure to timestamp
	initialt = time_make(&tinfo);
	// Set system time counter
	time_set(initialt);
	// Configure the library to update every day
	time_set_provider(time_provider, TIME_SECS_PER_DAY);
	// initialize digital pin 13 as an output.
	pinMode(13, OUTPUT);
}

void loop()
{
	// Display the time every second
	if (millis() - last > 1000) {
		last = millis();
		// Get the timestamp
		now = time_get();
		// Convert to human format
		time_break(now, &tinfo);
		// Send to serial port
		Serial.print(tinfo.tm_hour);
		printDigits(tinfo.tm_min);
		printDigits(tinfo.tm_sec);
		Serial.print(" ");
		Serial.print(tinfo.tm_mday);
		Serial.print(" ");
		Serial.print(tinfo.tm_mon);
		Serial.print(" ");
		Serial.print(tinfo.tm_year);
		Serial.println();
	}
}

void printDigits(int digits)
{
	// utility function for digital clock display: prints preceding colon and leading 0
	Serial.print(":");
	if (digits < 10)
		Serial.print('0');
	Serial.print(digits);
}

// Prototype if the function providing time information

time_t time_provider()
{
	return initialt;
}
