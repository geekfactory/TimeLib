/************************* http://geekfactory.mx *******************************
 *
 * This program demonstrates the transmission and reception of data through the
 * USART peripheral. You can review the code on UART.h and UART-PIC16.c to see
 * how each function is implemented.
 *
 * Este programa demuestra la transmisión y recepción de datos a través de la
 * USART del PIC. Puede revisarse el código fuente de las funciones en los
 * archivos UART.h y UART-PIC16.c
 *
 * AUTHOR/AUTOR: Jesus Ruben Santa Anna Zamudio
 * COMPILER/COMPILADOR: Microchip XC8 http://www.microchip.com/compilers
 *
 ********************************************************************************/
#include <xc.h>			// Encabezado para el compilador XC8
#include <stdio.h>
#include <stdlib.h>
#include "../TimeLib.h"

#pragma config FOSC = XT        // Oscillator Selection bits (XT oscillator)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = ON       // RA5/MCLR/VPP Pin Function Select bit (RA5/MCLR/VPP pin function is MCLR)
#pragma config BOREN = OFF      // Brown-out Reset Enable bit (BOR disabled)
#pragma config LVP = OFF        // Low-Voltage Programming Enable bit (RB3 is digital I/O, HV on MCLR must be used for programming)
#pragma config CPD = OFF        // Data EE Memory Code Protection bit (Code protection off)
#pragma config WRT = OFF        // Flash Program Memory Write Enable bits (Write protection off)
#pragma config CCPMX = RB0      // CCP1 Pin Selection bit (CCP1 function on RB0)
#pragma config CP = OFF         // Flash Program Memory Code Protection bit (Code protection off)

#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enable bit (Fail-Safe Clock Monitor disabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal External Switchover mode disabled)

// Initial timestamp, make it global so we can use it with update function
timelib_t initialt;
// Time provider function
timelib_t time_provider();

void main()
{
	ANSEL = 0x00;
	TRISA = 0xFF;
	TRISB = 0b00000100;

	// Structure that holds human readable time information;
	struct tm tinfo;

	// Local time to get
	timelib_t now;

	// Store last time we sent the information
	uint32_t last = 0;

	// Set time manually to 13:55:30 Jan 1st 2014
	tinfo.tm_year = 14;
	tinfo.tm_mon = 1;
	tinfo.tm_mday = 1;
	tinfo.tm_hour = 13;
	tinfo.tm_min = 55;
	tinfo.tm_sec = 30;

	// Tick Initialization
	tick_init();
	// Convert time structure to timestamp
	initialt = timelib_make(&tinfo);
	// Set system time counter
	timelib_set(initialt);
	// Set the function to get accurate time
	timelib_set_provider(&time_provider, TIMELIB_SECS_PER_DAY);

	printf("TimeLib Test Program\r\n");

	E_

	for (;;) {
		// Display the time every second
		if (tick_get() - last > TICK_SECOND) {
			last = tick_get();
			// Send to serial port
			printf("Time: %02d:%02d:%02d Date: %02d/%02d/%02d\r\n", hour(), minute(), second(), day(), month(), year());
		}
	}

}

timelib_t time_provider()
{
	/* Return precise time information: This function should query a precise
	 * time reference such as: GPS, NTP or Real time clock for demo purposes
	 * the function returns the same timestamp */

	return initialt;
}

/**
 * Helper fuction to use printf
 * 
 * @param data
 */
void putch(unsigned char data)
{
	TXSTA = 0x26;
	RCSTAbits.SPEN = 1;

	TXREG = data; // Write to Tx register
	while (!(PIR1bits.TXIF)); // Wait while transmission ends
}

/**
 * Interrupt vector implementation. Here we must update the internal counter on
 * the tick library (this is needed on PIC16 or PIC18 parts).
 */
void interrupt isr(void)
{
	// Update the system counter
	tick_update();
	// Other interrupts can be handled here
}
