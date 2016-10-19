/* UART Communication Module
 *
 * Handles all communication with a UART terminal
 *
 * TODO
 * Write actual description of module
 *
 * Written by Alex Tavares <tavaresa13@gmail.com>
 */

#ifndef uart_h
#define uart_h
#include <arduino.h>
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

uint16_t getStartAddress();
/*
 * Repeatedly prompts user for start address of recording and returns a valid address
 *
 * This function expects the serial terminal to be on a new line.
 *
 * OUTPUT: Valid ISD1700 recording start address
 */

unsigned long getRecordDuration();
/*
 * Prompts the user for a duration of recording and returns a number of milliseconds
 *
 * This function expects the serial terminal to be on a new line.
 *
 * The chosen duration may be longer than is remaining on the ISD1700 device.
 *
 * OUTPUT: Desired recording duration
 */

byte getPlaybackVolume();
/*
 * Prompts the user for a playback volume
 *
 * This function expects the serial terminal to be on a new line.
 *
 * OUTPUT: Desired playback volume
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
 * OUTPUT: Unverified ISD1700 recording start address
 */

#endif