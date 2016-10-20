/* ISD1700-Recorder
 *
 * Records messages from an external analogue source to an ISD1700 series chip
 *
 * First prompts user via serial terminal for recording address and duration.
 * After recording completes, user is prompted for a playback volume.
 * Playback can be repeated indefinitely at varying volumes.
 *
 * Estimated sample rate and end address are also displayed.
 *
 * Written by Alex Tavares <tavaresa13@gmail.com>
 */
#ifndef main_h
#define main_h
#include <arduino.h>
#include <avr/pgmspace.h>
#include "isd.h"
#include "uart.h"

/////////////////////////
// CONFIGURATION VARIABLES
/////////////////////////

const unsigned int AUDIO_THRESHOLD = 517;  // Lower is more sensitive

// UI Strings
const PROGMEM char ERASE_PROMPT_STR[] = "Reset (erase) all memory for writing? (Y/N) ";
const PROGMEM char ERASE_NOTE_STR[] = "Note that some memory locations may not be writable.\r\n";
const PROGMEM char ERASING_STR[] = "Erasing all memory...\r\n";
const PROGMEM char START_ADDR_STR[] = "Start address: 0x";
const PROGMEM char DURATION_STR[] = "Duration of audio (in milliseconds): ";
const PROGMEM char WAITING_STR[] = "Waiting for audio signal... \r\n";
const PROGMEM char RECORDING_STR[] = "Recording...\r\n";
const PROGMEM char RECORDING_END_STR[] = "Completed at address 0x";
const PROGMEM char SR_ESTIMATE_1_STR[] = "The sample rate is approximately ";
const PROGMEM char SR_ESTIMATE_2_STR[] = " Hz.\r\n";
const PROGMEM char VOL_STR[] = "Playback volume (7 to 0, other to quit): ";
const PROGMEM char PLAYBACK_STR[] = "Playing back audio...\r\n";

// Error Strings
const PROGMEM char ERR_INV_ADDR_STR[] = "Invalid memory pointer!\r\n";
const PROGMEM char ERR_INV_DUR_STR[] = "Invalid duration!\r\n";
const PROGMEM char ERR_EOM_1_STR[] = "End of memory reached. Audio was still recorded for ";
const PROGMEM char ERR_EOM_2_STR[] = " milliseconds.\r\n";


/////////////////////////
// PIN DEFINITIONS
/////////////////////////

const byte AUDIO_IN_PIN = A0;


/////////////////////////
// INTERNAL FUNCTIONS
/////////////////////////

void setup();
/*
 * Initializes all modules
 * Runs automatically on program startup
 */

void loop();
/*
 * Handles the main code actions
 * Automatically loops endlessly after setup()
 *
 * Prompts the user via UART for the starting address of the message and its duration.
 * Once these are specified, recording is automatically started upon detection of an audio signal.
 * After the specified amount of time elapses, the end address of the message is displayed,
 * along with the an approximate sample frequency.
 *
 * After recording, the user is prompted for a volume level for playback.
 * These levels are set by the ISD1700, where 0 is loudest and 7 is quietest.
 * Any value greater than 7 will exit playback.
 *
 * If the end of memory is reached, audio recording is halted and the user is notified.
 */

#endif