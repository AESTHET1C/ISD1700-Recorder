// TODO
// Module documentation

#include <arduino.h>

/////////////////////////
// CONFIGURATION VARIABLES
/////////////////////////

// MCU
const unsigned int UART_BAUD = 9600;

// ISD1700
const byte ISD_DUR_MAX_DIGITS = 5;  // Maximum decimal digits of recording duration (using ms)


/////////////////////////
// ISD1700 CONSTANTS
/////////////////////////

// Memory information
const uint16_t ISD_MIN_ADDR = 0x010;
const uint8_t ISD_HEX_ADDR_WIDTH = 3;


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
 * The chosen duration may cause recording to stop early due to an end-of-memory condition.
 * It is expected the calling code can handle this.
 *
 * OUTPUT: Desired recording duration
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