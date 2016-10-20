/* UART Communication Module
 *
 * Handles configuration and much of the communication with a UART terminal
 *
 * Several functions are included to automate the prompting for input values.
 * These include recording start address, duration, and playback volume.
 *
 * Strings for prompts are taken from ISD1700-Recorder.h.
 * Basic sanity checks for ISD1700 memory ranges are taken from isd.h.
 *
 * Written by Alex Tavares <tavaresa13@gmail.com>
 */

#ifndef uart_h
#define uart_h
#include <arduino.h>
#include <avr/pgmspace.h>
#include "ISD1700-Recorder.h"
#include "isd.h"

/////////////////////////
// CONFIGURATION VARIABLES
/////////////////////////

// MCU
const unsigned int UART_BAUD = 9600;

// ISD1700
const byte ISD_DUR_MAX_DIGITS = 5;  // Maximum decimal digits of recording duration (using ms)
const byte ISD_HEX_ADDR_WIDTH = 3;  // Maximum hex digits of address


/////////////////////////
// AVAILABLE FUNCTIONS
/////////////////////////

void initUART();
/*
 * Initializes UART communications
 * Must be called before communicating with the UART terminal
 *
 * This involves setting pin states and configuring the UART device.
 */

bool confirmISDFlashErase();
/*
 * Prompts the user on whether to erase all memory of the ISD1700 device
 *
 * OUTPUT: Is an erase desired?
 */

uint16_t getStartAddress();
/*
 * Repeatedly prompts user for start address of recording and returns a valid address
 *
 * This function expects the serial terminal to be on a new line.
 *
 * Only hexadecimal inputs are acknowledged.
 *
 * OUTPUT: Valid ISD1700 recording start address
 */

unsigned long getRecordDuration();
/*
 * Prompts the user for a duration of recording and returns a roughly valid duration
 *
 * This function expects the serial terminal to be on a new line.
 *
 * Only numerical inputs are acknowledged.
 *
 * The chosen duration may be longer than is remaining on the ISD1700 device.
 * This is because it is impossible to accurately determine the remaining time on the device.
 * Input is limited to within the proper order of magnitude.
 *
 * OUTPUT: Desired recording duration in milliseconds
 */

byte getPlaybackVolume();
/*
 * Prompts the user for a playback volume
 *
 * This function expects the serial terminal to be on a new line.
 *
 * Only single-digit numerical inputs are acknowledged.
 *
 * OUTPUT: Desired playback volume
 */

void printFlashString(const char string[]);
/*
 * Prints a string from flash memory
 *
 * INPUT:  Pointer to string in flash memory
 */


/////////////////////////
// INTERNAL FUNCTIONS
/////////////////////////

void flushSerialReadBuffer();
/*
 * Clears all queued serial inputs
 */

uint16_t promptStartAddress();
/*
 * Prompts the user for a start address of recording and returns an unverified address
 *
 * This function expects the serial terminal to be on a new line.
 *
 * Only hexadecimal inputs are acknowledged.
 *
 * Input is limited to within the proper number of digits.
 *
 * OUTPUT: Unverified ISD1700 recording start address
 */


#endif