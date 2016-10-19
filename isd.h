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
#include <SPI.h>

/////////////////////////
// CONFIGURATION VARIABLES
/////////////////////////

// SPI
const unsigned long SPI_RATE = 1000000;
const byte SPI_ENDIAN = LSBFIRST;
const byte SPI_MODE = SPI_MODE1;

// ISD1700
const uint16_t ISD_MAX_ADDR = 0x14F;                                   // ISD1740
const uint16_t ISD_APC_DEFAULT_CONFIG = (B00000100 << 8) + B10100000;
const byte ISD_DEFAULT_VOL = 7;


/////////////////////////
// PIN DEFINITIONS
/////////////////////////

const byte ISD_INT_PIN = 9;
const byte ISD_SS_PIN = SS;


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
 * Must be called before communicating with the ISD1700 device
 *
 * This involves setting pin states and resetting the ISD1700 device's configuration.
 *
 * Affects the ISD1700 device's APC register
 */

void powerUpISD();
/*
 * Powers up the ISD1700 device into SPI mode
 *
 * This function should be called before any other commands are sent to the ISD1700 device,
 * excluding initISD().
 */

void powerDownISD();
/*
 * Powers down the ISD1700 device from SPI mode
 *
 * This function should be called after all other operations complete.
 */

void stopISD();
/*
 * Stops any active operation of the ISD1700 device and resets it
 *
 * This function clears the ISD1700 device's interrupt flag, and resets its configuration.
 * This includes enabling audio feedthrough and setting volume.
 *
 * Affects the ISD1700 device's APC register and interrupt flag
 */

void beginISDRecording(uint16_t record_ptr);
/*
 * Begins recording indefinitely to the ISD1700 at a given address
 *
 * Recording should be stopped by using stopISD().
 * If end of memory is reached, recording stops and the ISD1700's interrupt register is set.
 *
 * It is assumed the APC register is already configured.
 *
 * An invalid recording address will cause undefined behavior.
 *
 * Affects the ISD1700 device's REC_PTR register
 * INPUT:  Recording start address
 */

void beginISDPlayback(uint16_t play_ptr, byte volume);
/*
 * Begins playback from the ISD1700 at a given address
 *
 * Playback should be stopped manually by using stopISD().
 * If stopISD() is not sent during or at the end of playback,
 * playback will continue to play to the end of the ISD1700 device's memory.
 *
 * An invalid playback address will default to playing the entirety of the ISD1700 device's memory.
 *
 * The volume is based upon the ISD1700 device. That is, a value of 7 is minimum, and 0 is maximum.
 * Invalid volume levels will default to minimum volume.
 *
 * Audio feedthrough is disabled before playback.
 * It should be enabled after playback stops.
 *
 * Affects the ISD1700 device's APC and PLAY_PTR registers
 * INPUT:  Playback start address
 *         Playback volume (7 to 0)
 */

uint16_t getRecPtrISD();
/*
 * Gets the current recording pointer of the ISD1700 device
 *
 * This function can only be called when the device is idle (and powered up).
 *
 * OUTPUT: Recording pointer of the ISD1700 device
 */

bool ISDInterrupted();
/*
 * Gets the state of the ISD1700 interrupt
 *
 * This is usually due to an unexpected end-of-memory event.
 * The interrupt can be cleared using stopISD().
 *
 * OUTPUT: Interrupt state
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
 * The volume is based upon the ISD1700 device. That is, a value of 7 is minimum, and 0 is maximum.
 * Invalid volume levels will default to minimum volume.
 *
 * Affects the ISD1700 device's APC register
 * INPUT:  Feedthrough setting
 *         Playback volume (7 to 0)
 */

void sendISDCommand(byte command);
/*
 * Sends a basic command to the ISD1700 device
 *
 * These commands consist only of a single command byte, followed by a null byte.
 *
 * All SPI actions are handled automatically.
 *
 * INPUT:  Command to execute
 */

void clearIntISD();
/*
 * Clears the interrupt flag of the ISD1700 device
 *
 * Affects the ISD1700 device's interrupt flag
 */


#endif