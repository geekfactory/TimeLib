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
#ifndef TIMELIB_H
#define TIMELIB_H

/*-------------------------------------------------------------*/
/*		Includes and dependencies			*/
/*-------------------------------------------------------------*/
#include "TimeLibPort.h"
#include "Tick/Tick.h"

/*-------------------------------------------------------------*/
/*		Macros and definitions				*/
/*-------------------------------------------------------------*/
#define TIME_SECS_PER_DAY		(86400UL)
#define TIME_SECS_PER_HOUR		(3600UL)
#define TIME_SECS_PER_MINUTE            (60UL)
#define TIME_DAYS_PER_WEEK		(7UL)
#define TIME_SECS_PER_WEEK		(TIME_SECS_PER_DAY * TIME_DAYS_PER_WEEK)
#define TIME_SECS_PER_YEAR              (TIME_SECS_PER_WEEK * 52UL)
#define TIME_SECS_YEAR_2K               (946684800UL)

/*-------------------------------------------------------------*/
/*		Typedefs enums & structs			*/
/*-------------------------------------------------------------*/
#if !defined(__time_t_defined)
typedef unsigned long time_t;
#endif

/**
 * @brief Stores human readable time and date information
 *
 * Simplified structure to store human readable components of time /date similar
 * to the standard C structure for time information.
 */
struct tm {
	uint8_t tm_sec;
	uint8_t tm_min;
	uint8_t tm_hour;
	uint8_t tm_wday; // day of week, sunday is day 1
	uint8_t tm_mday;
	uint8_t tm_mon;
	uint8_t tm_year; // offset from 1970;
};

/**
 * @brief Enumeration defines the current state of the system time
 */
enum time_status {
	E_TIME_NOT_SET = 0, //!< Time has not been set
	E_TIME_NEEDS_SYNC, //!< Time was set, but needs to be synced with timebase
	E_TIME_OK, //!< Time is valid and in sync with time source
};

/**
 * @brief Type definiton for the funtion pointer that gets precise time
 * 
 * Pointer to function that gets time from external time source or device:
 * GPS, NTP, RTC, etc. This is needed to automatically sync time with
 * an external source.
 */
typedef time_t(* time_callback_t)();

/*-------------------------------------------------------------*/
/*		Function prototypes				*/
/*-------------------------------------------------------------*/
#ifdef	__cplusplus
extern "C" {
#endif
	/**
	 * @brief Sets the current system time
	 *
	 * This function sets the time keeping system variable to the given value.
	 * The time is stored and mantained as an integral value representing the
	 * number of seconds elapsed since 00:00 hours, Jan 1, 1970 UTC (a unix
	 * timestamp).
	 *
	 * @param now Unix timestamp representing the number of seconds elapsed since
	 * 00:00 hours, Jan 1, 1970 UTC to the present date.
	 */
	void time_set(time_t now);

	/**
	 * @brief Gets the current system time
	 *
	 * This function reads the value of the time keeping system variable.
	 * The time is stored and mantained as an integral value representing the
	 * number of seconds elapsed since 00:00 hours, Jan 1, 1970 UTC (a unix
	 * timestamp).
	 *
	 * @return A Unix timestamp representing the number of seconds elapsed since
	 * 00:00 hours, Jan 1, 1970 UTC to the present date.
	 */
	time_t time_get();

	/**
	 * @brief Gets the status of the system time
	 *
	 * This function helps the application to determine if the system time is
	 * accurate or if the time has been set by the user.
	 *
	 * See enumeration time_status for details about return codes
	 *
	 * @return Returns a code that determines the TimeLib status.
	 */
	uint8_t time_get_status();

	/**
	 * @brief Compute the second at a given timestamp
	 *
	 * @param time The timestamp to calculate the second for
	 *
	 * @return The elapsed seconds
	 */
	uint8_t time_second_t(time_t time);

	/**
	 * @brief Compute the minute at a given timestamp
	 *
	 * @param time The timestamp to calculate the minute for
	 *
	 * @return The elapsed minutes
	 */
	uint8_t time_minute_t(time_t time);

	/**
	 * @brief Compute the minute at a givent timestamp
	 *
	 * @param time The timestamp to calculate the hour for
	 *
	 * @return
	 */
	uint8_t time_hour_t(time_t time);

	/**
	 *
	 * @param time
	 * @return
	 */
	uint8_t time_wday_t(time_t time);

	/**
	 *
	 * @param time
	 * @return
	 */
	uint8_t time_day_t(time_t time);

	/**
	 *
	 * @param time
	 * @return
	 */
	uint8_t time_month_t(time_t time);

	/**
	 *
	 * @param time
	 * @return
	 */
	uint8_t time_year_t(time_t time);

	/**
	 * @brief Gets the current second
	 *
	 * @return
	 */
	uint8_t time_second();

	/**
	 * @brief Gets the current minute
	 *
	 * @return
	 */
	uint8_t time_minute();

	/**
	 * @brief Gets the current hour
	 *
	 * @return
	 */
	uint8_t time_hour();

	/**
	 * 
	 * @return
	 */
	uint8_t time_wday();

	/**
	 *
	 * @return
	 */
	uint8_t time_day();

	/**
	 *
	 * @return
	 */
	uint8_t time_month();

	/**
	 *
	 * @return
	 */
	uint16_t time_year();

	/**
	 * @brief Generates a Unix Timestamp from the given time/date components
	 *
	 * This function generates the corresponding Unix timestamp for the provided
	 * date and time information. The timestamp is an integral value representing
	 * the number of seconds elapsed since 00:00 hours, Jan 1, 1970 UTC.
	 *
	 * @param timeinfo A structure containing the human readable elements of the
	 * date and time to convert to a UNIX timestamp.
	 *
	 * @return The UNIX Timestamp for the given time/date components
	 */
	time_t time_make(struct tm * timeinfo);

	/**
	 * @brief Get human readable time from Unix time
	 *
	 * This function performs the conversion from Unix timestamp to human readable
	 * time components and places the information on a standard time structure.
	 *
	 * @param timeinput The timestamp to convert
	 * @param timeinfo Pointer to tm structure to strore the resuling time
	 */
	void time_break(time_t timeinput, struct tm * timeinfo);

	/**
	 * @brief Sets the callback function that obtains precise time
	 *
	 * This function sets a callback that runs to keep internal CPU time in sync
	 * with an accurate time reference. This function also sets the time interval
	 * for time synchronization.
	 *
	 * @param callback The callback to get precise time information, this callback
	 * should return a timestamp.
	 *
	 * @param timespan The interval in seconds when this function should be called
	 */
	void time_set_provider(time_callback_t callback, time_t timespan);

#ifdef	__cplusplus
}
#endif

/*-------------------------------------------------------------*/
/*		Function like macros				*/
/*-------------------------------------------------------------*/
/**
 * Alias for time_get() function
 */
#define now()		time_get()

/**
 * @brief Alias for time_second() function
 */
#define second()	time_second()

/**
 * @brief Alias for time_minute() function
 */
#define minute()	time_minute()

/**
 * @brief Alias for time_hour() function
 */
#define hour()		time_hour()

/**
 * @brief Alias for time_wday() function
 */
#define wday()		time_wday()

/**
 * @brief Alias for time_day() function
 */
#define day()		time_day()

/**
 * @brief Alias for time_month() function
 */
#define month()		time_month()

/**
 * @brief Alias for time_year() function
 */
#define year()		time_year()

/**
 * Computes the day of the week. Sunday is day 0 and saturday is 6
 */
#define time_dow(t)		((t / TIME_SECS_PER_DAY + 4) % TIME_DAYS_PER_WEEK)

/**
 * Computes the number of elapsed days for the given timestamp
 */
#define time_elapsed_days(t)	(t / TIME_SECS_PER_DAY)

/**
 * Computes the number of elapsed seconds since midnight today
 */
#define time_seconds_today(t)	(t % TIME_SECS_PER_DAY)

/**
 * @brief Calculates the timestamp of the previous midnight for the given time
 */
#define time_prev_midnight(t)	(uint32_t)(( (uint32_t)t / (uint32_t)TIME_SECS_PER_DAY) * (uint32_t)TIME_SECS_PER_DAY)

/**
 * @brief Calculates the timestamo of the next midnight for the given time
 */
#define time_next_midnight(t)	(time_prev_midnight(t) + TIME_SECS_PER_DAY)

/**
 * @brief Calculates the number of seconds elapsed since the start of the week
 */
#define time_secs_this_week(t)	(time_seconds_today(t) + ((time_dow(t)) * TIME_SECS_PER_DAY))

/**
 * @brief Calculates the timestamp at the begining of the last sunday
 */
#define time_prev_sunday(t)	(t - time_seconds_this_week(t))

/**
 * @brief Calculates the timestamp at the begining of the next sunday
 */
#define time_next_sunday(t)	(time_prev_sunday(t)+TIME_SECS_PER_WEEK)

#endif
// End of Header file
