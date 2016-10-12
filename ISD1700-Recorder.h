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

#include <arduino.h>
#include <avr/pgmspace.h>

/////////////////////////
// CONFIGURATION VARIABLES
/////////////////////////

const unsigned int AUDIO_THRESHOLD = TODO;  // Volume threshold for beginning of recording

// ISD1700
const uint16_t ISD_MAX_ADDR = 0x14F;  // Last row of ISD1740

// UART
const unsigned int UART_BAUD = 9600;

// SPI
const unsigned long SPI_RATE = 1000000;
const byte SPI_ENDIAN = LSBFIRST;
const byte SPI_MODE = SPI_MODE1;


// UI Strings
const PROGMEM char[] START_ADDR_STR = "Start address : 0x";
const PROGMEM char[] DURATION_STR = "Duration of audio (in milliseconds): ";
const PROGMEM char[] RECORDING_STR = "Recording...\r\n";
const PROGMEM char[] SR_ESTIMATE_STR = "The sample rate is approximately ";
const PROGMEM char[] VOL_STR = "Playback volume (0 - 7, other to quit): ";
const PROGMEM char[] PLAYBACK_STR = "Playing back audio at volume ";

// Error Strings
const PROGMEM char[] ERR_INV_ADDR_STR = "Invalid memory pointer!\r\n";
const PROGMEM char[] ERR_EOM_STR = "End of memory reached.\r\n";


/////////////////////////
// PIN DEFINITIONS
/////////////////////////

const byte ISD1700_INT_PIN = 9;

const byte SPI_SS_PIN = 10;
// SPI_MOSI_PIN = 11
// SPI_MISO_PIN = 12
// SPI_SCLK_PIN = 13

const byte AUDIO_MONITOR = A0;

// SERIAL_RX_PIN = 0
// SERIAL_TX_PIN = 1

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
// ISD1700 CONSTANTS
/////////////////////////

// Memory information
const uint16_t ISD_MIN_ADDR = 0x010;
const uint8_t ISD_HEX_ADDR_WIDTH = 3;

// Commands list
// TODO
// Update this
const byte ISD_STOP = 0x02;
const byte ISD_WR_APC2 = 0x65;
const byte ISD_SET_PLAY = 0x80;

// Configuration data
// TODO
// Update this
const uint16_t ISD_AUDIO_CONFIG = B0000010010100000;


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

void configISD(uint16_t configuration);  // TODO
/*
 * Configures the ISD1700 device
 * Can be used to change volume
 *
 * INPUT:  configuration bytes to use
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

uint16_t getISDConfig(byte volume);
/*
 * Gets the configuration bytes for the ISD1700 chip, given a volume
 *
 * If the input volume is greater than 7, a volume of 7 is used.
 *
 * INPUT:  Volume, from 7 (quietest) to 0 (loudest)
 * OUTPUT: Two-byte configuration value
 */