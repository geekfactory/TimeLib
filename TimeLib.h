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

/*-------------------------------------------------------------*/
/*		Typedefs enums & structs			*/
/*-------------------------------------------------------------*/

#if !defined(__time_t_defined) // avoid conflict with newlib or other posix libc
typedef unsigned long time_t;
#endif

struct tm_t{
uint8_t tm_sec;
uint8_t tm_min;
uint8_t tm_hour;
uint8_t tm_wday; // day of week, sunday is day 1
uint8_t tm_mday;
uint8_t tm_mon;
uint8_t tm_year; // offset from 1970;
};

/**
 * Enumeration defines the current state of the system time
 */
enum enTimeStatus {
    E_TIME_NOT_SET = 0, //!< Time has not been set
    E_TIME_NEEDS_SYNC, //!< Time was set, but needs to be synced with timebase
    E_TIME_OK, //!< Time is valid and in sync with time source
};
/**
 * @brief Type for funtion pointer that gets the precise time from a timebase
 */
typedef time_t(* time_callback_t)();

/*-------------------------------------------------------------*/
/*		Function prototypes				*/
/*-------------------------------------------------------------*/

/**
 * @brief Sets the current system time
 *
 * This function sets the time keeping system variable to the given value.
 * The time is stored and mantained as an integral value representing the number
 * of seconds elapsed since 00:00 hours, Jan 1, 1970 UTC (a unix timestamp).
 *
 * @param now Unix timestamp representing the number of seconds elapsed since
 * 00:00 hours, Jan 1, 1970 UTC to the present date.
 */
void time_set(time_t now);

/**
 * @brief Gets the current system time
 *
 * This function reads the value of the time keeping system variable.
 * The time is stored and mantained as an integral value representing the number
 * of seconds elapsed since 00:00 hours, Jan 1, 1970 UTC (a unix timestamp).
 *
 * @return A Unix timestamp representing the number of seconds elapsed since
 * 00:00 hours, Jan 1, 1970 UTC to the present date.
 */
time_t time_get();

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
time_t time_make(struct tm_t * timeinfo);

/**
 * @brief Get human readable time from Unix time
 *
 * This function performs the conversion from Unix timestamp to human readable
 * time components and places the information on a standard time structure.
 *
 * @param timeinput The timestamp to convert
 * @param timeinfo Pointer to tm structure to strore the resuling time
 */
void time_break(time_t timeinput, struct tm_t * timeinfo);

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

#define time_prev_midnight(_xTime_) (uint32_t)(( (uint32_t)_xTime_ / (uint32_t)TIME_SECS_PER_DAY) * (uint32_t)TIME_SECS_PER_DAY)
#define time_dow(_xTime_)  (( _xTime_ / TIME_SECS_PER_DAY + 4)  % TIME_DAYS_PER_WEEK) // 0 = Sunday
#define time_seconds_today(_xTime_)  (_xTime_ % TIME_SECS_PER_DAY)   // the number of seconds since last midnight
#define time_seconds_this_week(_xTime_)  (time_seconds_today(_xTime_) +  ((time_dow(_xTime_)) * TIME_SECS_PER_DAY) )
#define time_prev_sunday(_xTime_)  (_xTime_ - time_seconds_this_week(_xTime_))

#endif
// End of Header file
