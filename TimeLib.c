/*	Time management library for embedded devices
	Copyright (C) 2014 Jesus Ruben Santa Anna Zamudio.

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <http://www.gnu.org/licenses/>.

	Author website: http://www.geekfactory.mx
	Author e-mail: ruben at geekfactory dot mx
 */
#include "TimeLib.h"

/* Stores the day count for each month */
const unsigned char month_length[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

/* Unix like time counter, keeps track of absolute time */
time_t sys_time = 0;

/* The interval in seconds after which the system time variable should be updated
 * (synced) with an external (accurate) time base */
time_t sync_interval = 86400;

/* Unix timestamp when the sync should be done. */
time_t sync_next = 0;

/* Cache for current time */
time_t tcache;
struct tm telements;

/* Variable used to keep track of the last time the "seconds" or sys_time counter
 * was updated in "tick" units */
unsigned long last_update = 0;

/* Keeps the status of the system time (ok, needs sync, not set, etc). */
enum time_status tstatus = E_TIME_NOT_SET;

/**
 * Stores a pointer to a function that returns a precise unix timestamp to set
 * the internal clock to the returned timestamp. This update ocurs at the
 * interval given when the callback is configured.
 */
time_callback_t time_provider_callback = 0;

/**
 * @brief Computes if the given year is a leap year
 *
 * @param year The year to check
 *
 * @return Returns true if "year" is leap, false otherwise
 */
static int time_is_leap(unsigned int year)
{
	// Must be divisible by 4 but not by 100, unless it is divisible by 400
	return((year % 4 == 0 && year % 100 != 0) || year % 400 == 0);
}

/**
 * Updates the time structure if time has changed
 *
 * @param time The timestamp now.-
 */
static void time_update(time_t time)
{
	if (tcache != time) {
		tcache = time;
		time_break(time, &telements);
	}
}

/*-------------------------------------------------------------*/
/*	Public API, check TimeLib.h for documentation		*/
/*-------------------------------------------------------------*/
void time_set(time_t now)
{
	sys_time = now;
	sync_next = now + sync_interval;
	tstatus = E_TIME_OK;
	last_update = tick_get();
}

time_t time_get()
{
	time_t now = 0;

	// Check if time needs sync to timebase
	if (sync_next <= sys_time) {
		// Null pointer check
		if (time_provider_callback != 0) {
			// Invoke callback function
			now = time_provider_callback();
			// Got time from callback?
			if (now != 0)
				time_set(now);
			else {
				sync_next = sys_time + sync_interval;
				tstatus = (tstatus == E_TIME_NOT_SET) ? E_TIME_NOT_SET : E_TIME_NEEDS_SYNC;
			}
		}
	}

	// Check how many seconds have elapsed (if any) since the last call
	// and update the timestamp counter
	while (tick_get() - last_update >= TICK_SECOND) {
		// Increment timestamp
		sys_time++;
		last_update += TICK_SECOND;
	}

	return sys_time;
}

uint8_t time_get_status()
{
	time_get();
	return tstatus;
}

uint8_t time_second_t(time_t time)
{
	time_update(time);
	return telements.tm_sec;
}

uint8_t time_minute_t(time_t time)
{
	time_update(time);
	return telements.tm_min;
}

uint8_t time_hour_t(time_t time)
{
	time_update(time);
	return telements.tm_hour;
}

uint8_t time_wday_t(time_t time)
{
	time_update(time);
	return telements.tm_wday;
}

uint8_t time_day_t(time_t time)
{
	time_update(time);
	return telements.tm_mday;
}

uint8_t time_month_t(time_t time)
{
	time_update(time);
	return telements.tm_mon;
}

uint16_t time_year_t(time_t time)
{
	time_update(time);
	return telements.tm_mon;
}

uint8_t time_second()
{
	return time_second_t(now());
}

uint8_t time_minute()
{
	return time_minute_t(now());
}

uint8_t time_hour()
{
	return time_hour_t(now());
}

uint8_t time_wday()
{
	return time_wday_t(now());
}

uint8_t time_day()
{
	return time_day_t(now());
}

uint8_t time_month()
{
	return time_month_t(now());
}

uint16_t time_year()
{
	return time_year_t(now());
}

time_t time_make(struct tm * timeinfo)
{
	int i;
	uint32_t tstamp;

	// Compute the number of seconds since the year 1970 to the begining of
	// the given year on the structure, add to the output value
	tstamp = timeinfo->tm_year * (TIME_SECS_PER_DAY * 365);
	// Add the seconds corresponding to leap years (add extra days)
	for (i = 0; i < timeinfo->tm_year; i++) {
		if (time_is_leap(i + 1970))
			tstamp += TIME_SECS_PER_DAY;
	}
	// Add secconds for the monts elapsed
	for (i = 1; i < timeinfo->tm_mon; i++) {
		if (i == 2 && time_is_leap(timeinfo->tm_year + 1970))
			tstamp += TIME_SECS_PER_DAY * 29;
		else
			tstamp += TIME_SECS_PER_DAY * month_length[i - 1];
	}
	// Add seconds for past days
	tstamp += (uint32_t) (timeinfo->tm_mday - 1) * (uint32_t) TIME_SECS_PER_DAY;
	// Add seconds on this day
	tstamp += (uint32_t) timeinfo->tm_hour * (uint32_t) TIME_SECS_PER_HOUR;
	tstamp += (uint32_t) timeinfo->tm_min * (uint32_t) TIME_SECS_PER_MINUTE;
	tstamp += (uint32_t) timeinfo->tm_sec;

	return tstamp;
}

void time_break(time_t timeinput, struct tm * timeinfo)
{
	uint8_t year;
	uint8_t month, monthLength;
	uint32_t time;
	unsigned long days;

	time = (uint32_t) timeinput;
	timeinfo->tm_sec = time % 60;

	time /= 60; // now it is minutes
	timeinfo->tm_min = time % 60;

	time /= 60; // now it is hours
	timeinfo->tm_hour = time % 24;

	time /= 24; // now it is days
	timeinfo->tm_wday = ((time + 4) % 7) + 1; // Sunday is day 1

	year = 0;
	days = 0;
	while ((unsigned) (days += (time_is_leap(1970 + year) ? 366 : 365)) <= time)
		year++;

	timeinfo->tm_year = year; // year is offset from 1970

	days -= time_is_leap(1970 + year) ? 366 : 365;
	time -= days; // now it is days in this year, starting at 0

	days = 0;
	month = 0;
	monthLength = 0;
	for (month = 0; month < 12; month++) {
		// Check
		if (month == 1) {
			if (time_is_leap(1970 + year)) {
				monthLength = 29;
			} else {
				monthLength = 28;
			}
		} else {
			monthLength = month_length[month];
		}

		if (time >= monthLength) {
			time -= monthLength;
		} else {
			break;
		}
	}
	timeinfo->tm_mon = month + 1; // jan is month 1
	timeinfo->tm_mday = time + 1; // day of month
}

void time_set_provider(time_callback_t callback, time_t timespan)
{
	// Check null pointer
	if (callback == 0)
		return;
	// Set new callback
	time_provider_callback = callback;
	// Enforce sync interval restrictions
	sync_interval = (timespan == 0) ? TIME_SECS_PER_DAY : timespan;
	//Set next sync time to actual time
	sync_next = sys_time;
	// Force time sync
	time_get();
}
