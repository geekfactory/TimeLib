/**
   GeekFactory - "Construye tu propia tecnologia"
   Distribucion de materiales para el desarrollo e innovacion tecnologica
   www.geekfactory.mx

   Example of the TimeLib library in Arduino. This code should display the time on
   the serial monitor. User can set the time on the tinfo structure. This is a basic
   example that shows how the library can be used as software only RTC.
*/
#include "TimeLib.h"

// Structure that holds human readable time information;
struct timelib_tm tinfo;

timelib_t now, initialt;
// Store last time we sent the information
uint32_t last = 0;

void setup()
{
  // Configure serial port
  Serial.begin(115200);
  while (!Serial);

  // Set time manually to 13:55:30 Jan 1st 2014
  // YOU CAN SET THE TIME FOR THIS EXAMPLE HERE
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

  // Configure the library to update / sync every day (for hardware RTC)
  timelib_set_provider(time_provider, TIMELIB_SECS_PER_DAY);
}

void loop()
{
  // Display the time every second
  if (millis() - last > 1000) {
    // Keep track of the time we last sent data to serial monitor
    last = millis();
    // Get the timestamp from the library
    now = timelib_get();
    // Convert to human readable format
    timelib_break(now, &tinfo);
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

timelib_t time_provider()
{
  // Prototype if the function providing time information
  return initialt;
}