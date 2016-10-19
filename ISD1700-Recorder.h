/* ISD1700-Recorder
 *
 * Records messages from an external analogue source to an ISD1700 series chip
 *
 * Prompts the user via UART for the starting address of the message and its duration.
 * Once these are specified, recording is automatically started upon detection of an audio signal.
 * After the specified amount of time elapses, the end address of the message is displayed,
 * along with an approximate sample frequency.
 *
 * After recording, the user is prompted for a volume level for playback.
 * These levels are set by the ISD1700, where 0 is loudest and 7 is quietest.
 * Any value greater than 7 will exit playback.
 *
 * If the end of memory is reached, audio recording is halted and the user is notified.
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

const unsigned int AUDIO_THRESHOLD = TODO;  // Volume threshold for beginning of recording

// UI Strings
const PROGMEM char[] START_ADDR_STR = "Start address: 0x";
const PROGMEM char[] DURATION_STR = "Duration of audio (in milliseconds): ";
const PROGMEM char[] WAITING_STR = "Waiting for audio signal... \r\n";
const PROGMEM char[] RECORDING_STR = "Recording...\r\n";
const PROGMEM char[] SR_ESTIMATE_1_STR = "The sample rate is approximately ";
const PROGMEM char[] SR_ESTIMATE_2_STR = " Hz.\r\n";
const PROGMEM char[] VOL_STR = "Playback volume (7 to 0, other to quit): ";
const PROGMEM char[] PLAYBACK_STR = "Playing back audio...\r\n";

// Error Strings
const PROGMEM char[] ERR_INV_ADDR_STR = "Invalid memory pointer!\r\n";
const PROGMEM char[] ERR_INV_DUR_STR = "Invalid duration!\r\n";
const PROGMEM char[] ERR_EOM_1_STR = "End of memory reached. Audio was still recorded for ";
const PROGMEM char[] ERR_EOM_2_STR = " milliseconds.";


/////////////////////////
// PIN DEFINITIONS
/////////////////////////

const byte AUDIO_IN_PIN = A0;


/////////////////////////
// ENUMS
/////////////////////////

typedef enum record_state {
	HALTED,
	REC_PROMPT,
	REC_WAIT,
	RECORDING,
	PB_READY,
	PLAYBACK
};


/////////////////////////
// INTERNAL FUNCTIONS
/////////////////////////

void setup();
// TODO

void loop();
// TODO

void initAudio();  // TODO
/*
 * Initializes audio playback
 * Must be called at startup
 *
 * Initialization involves setting status variables and pin configurations.
 * The ISD1700 configuration register is also set.
 */

void initSerial();  // TODO
/*
 * Initializes serial communications for both UART and SPI
 * Must be called before first serial usage
 */

void startRecording(uint16_t start_ptr);  // TODO
/*
 * Records audio to the ISD1700 chip until end of memory
 *
 * A stop command should be used to end recording.
 *
 * INPUT:  First row in memory to record to
 */

void stopRecording();  // TODO
/*
 * Stops recording audio on the ISD1700 chip
 *
 */


#endif