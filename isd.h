/* ISD1700 Communication Module
 *
 * Handles all communication with the ISD1700 device
 *
 * The default state of the ISD1700 device is to pass input from the analogue input to the speaker.
 * This is done at minimum volume by default. Passthrough is disabled during playback.
 *
 * Playback and recording commands will begin at the given address, and continue until end of memory.
 * It is expected stopISD() is used to end playback and recording before this happens.
 * In addition, ISDInterrupted() can be used to determine if end of memory was reached.
 *
 * Written by Alex Tavares <tavaresa13@gmail.com>
 */

#ifndef isd_h
#define isd_h
#include <arduino.h>
#include <spi.h>

/////////////////////////
// CONFIGURATION VARIABLES
/////////////////////////

// SPI
const unsigned long SPI_RATE = 1000000;
const byte SPI_ENDIAN = LSBFIRST;
const byte SPI_MODE = SPI_MODE1;

// ISD1700
const uint16_t ISD_MAX_ADDR = 0x14F;                        // ISD1740
const uint16_t ISD_APC_DEFAULT_CONFIG = B0000010010100000;
const byte ISD_DEFAULT_VOL = ISD_MIN_VOL;


/////////////////////////
// PIN DEFINITIONS
/////////////////////////

const byte ISD_INT_PIN = 9;
const byte ISD_SS_PIN = PIN_SPI_SS;


/////////////////////////
// CONSTANTS
/////////////////////////

// SPI
const SPISettings ISD_SPI_SETTINGS(SPI_RATE, SPI_ENDIAN, SPI_MODE);

// ISD1700
const uint16_t ISD_MIN_ADDR = 0x010;
const uint16_t ISD_FT_MASK = (1 << 6);
const byte ISD_VOL_MASK = B111;
const byte ISD_MIN_VOL = 7;

// ISD Commands
// TODO
// Verify+update this
const byte ISD_PU = 0x01;
const byte ISD_STOP = 0x02;
const byte ISD_CLR_INT = 0x04;
const byte ISD_PD = 0x07;
const byte ISD_RD_REC_PTR = 0x08;
const byte ISD_WR_APC2 = 0x65;
const byte ISD_SET_PLAY = 0x80;
const byte ISD_SET_REC = 0x81;


/////////////////////////
// AVAILABLE FUNCTIONS
/////////////////////////

void initISD();
/*
 * Initializes SPI communications and the ISD1700 device
 */

void powerUpISD();
/*
 * Powers up the ISD1700 device into SPI mode
 *
 * This function should be called before any other command is sent.
 */

void powerDownISD();
/*
 * Powers down the ISD1700 device from SPI mode
 *
 * This function should be called after all other operations complete.
 */

void stopISD();
/*
 * Stops any active operation of the ISD1700 device
 *
 * This function generates also clears any pre-existing interrupt, and resets device configuration.
 */

bool ISDInterrupted();
/*
 * Gets the state of the ISD1700 interrupt
 *
 * OUTPUT: Interrupt state
 */

void clearIntISD();
/*
 * Clears the interrupt register of the ISD1700 device
 */

uint16_t getRecPtrISD();
/*
 * Gets the current recording pointer of the ISD1700 device
 *
 * This function can only be called when the device is idle (and powered up).
 *
 * OUTPUT: Recording pointer of the ISD1700 device
 */

void beginISDRecording(uint16_t record_ptr);
/*
 * Begins recording to the ISD1700 at a given address
 *
 * Recording can be stopped by using stopISD().
 * If end of memory is reached, recording stops and the ISD1700's interrupt register is set.
 *
 * INPUT:  Recording start address
 */

void beginISDPlayback(uint16_t play_ptr, byte volume);
/*
 * Begins playback from the ISD1700 at a given address
 *
 * Playback must be stopped manually by using stopISD().
 * If stopISD() is not sent during or at the end of playback,
 * playback will continue to play to the end of the ISD1700 device's memory.
 *
 * An invalid playback address will default to playing the entirety of the ISD1700 device's memory.
 *
 * The volume is based upon the ISD1700 device. That is, a value of 7 is miniumum, and 0 is maximum.
 * Invalid volume levels will default to minimum volume.
 *
 * Audio feedthrough is disabled before playback.
 * It should be enabled after playback stops.
 *
 * INPUT:  Playback start address
 *         Playback volume (7 to 0)
 */


/////////////////////////
// INTERNAL FUNCTIONS
/////////////////////////

void configISD(bool feedthrough, byte volume);
/*
 * Configures the ISD1700 device
 *
 * Feedthrough connects the analogue input to the speaker output.
 * It is required to record from the analogue input.
 * It should be enabled by default.
 *
 * The volume is based upon the ISD1700 device. That is, a value of 7 is miniumum, and 0 is maximum.
 * Invalid volume levels will default to minimum volume.
 *
 * Affects the ISD1700 device's APC register and play pointer
 * INPUT:  Feedthrough setting
 *         Playback volume (7 to 0)
 */

void sendISDCommand(byte command);
/*
 * Sends a basic ISD1700 command
 *
 * These commands only consist of a single byte, followed by a null byte.
 *
 * All SPI transaction actions are handled.
 *
 * INPUT:  Command to execute
 */


#endif