/* ISD1700 Communication Module
 *
 * Handles all communication with the ISD1700 device
 *
 * Playback and recording commands will begin at the given address, and continue until end of memory.
 * It is expected stopISD() is used to end playback and recording before this happens.
 * In addition, ISDInterrupted() can be used to determine if end of memory was reached.
 *
 * The speaker is never enabled at the same time as the analogue input.
 * If a volume is selected for playback, the speaker is enabled and the analogue input disabled.
 * Similarly, if the speaker is disabled, the analogue input becomes enabled for recording.
 *
 * The majority of commands return the ISD1700 device's SR0 register as it was set at call time.
 * If the SR0 register is changed by the command, the updated value is not reflected.
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
const byte SPI_MODE = SPI_MODE3;

// ISD1700
const uint16_t ISD_MAX_ADDR = 0x14F;                                   // ISD1740
const uint16_t ISD_APC_DEFAULT_CONFIG = (B0110 << 8) + B10100000;
const byte ISD_DEFAULT_VOL = 7;


/////////////////////////
// PIN DEFINITIONS
/////////////////////////

const byte ISD_INT_PIN = 9;
const byte ISD_SS_PIN = SS;


/////////////////////////
// ISD1700 CONSTANTS
/////////////////////////

const SPISettings ISD_SPI_SETTINGS(SPI_RATE, SPI_ENDIAN, SPI_MODE);
const uint16_t ISD_MIN_ADDR = 0x010;
const byte ISD_MIN_VOL = 7;
const byte ISD_DISABLE_SPK = 8;

// Configuration Masks
const byte ISD_VOL_MASK = B111;
const uint16_t ISD_FT_MASK = (1 << 6);
const uint16_t ISD_SPK_MASK = (1 << 8);

// Time Delays
const byte ISD_T_PUD = 50;
const byte ISD_T_SET = 100;

// Commands
const byte ISD_PU = 0x01;
const byte ISD_STOP = 0x02;
const byte ISD_RESET = 0x03;
const byte ISD_CLR_INT = 0x04;
const byte ISD_PD = 0x07;
const byte ISD_G_ERASE = 0x43;
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
 * This involves setting pin states and configuring the SPI device.
 * In addition, the ISD1700 device is reset, set to SPI mode, and its speaker is disabled.
 *
 * Affects the ISD1700 device's APC register
 */

uint16_t eraseISD();
/*
 * Performs a global erase on the ISD1700 device
 *
 * OUTPUT: SR0 register
 */

uint16_t configISDAPC(byte volume);
/*
 * Configures the ISD1700 device's analogue path
 * This should be done when switching between playback and idle/recording states
 *
 * The volume is based upon the ISD1700 device. That is, a value of 7 is minimum, and 0 is maximum.
 * Values larger than 7 will disable the speaker and instead enable analogue input.
 * Therefore, the speaker must be disabled when recording.
 *
 * Affects the ISD1700 device's APC register
 * INPUT:  Speaker volume
 * OUTPUT: SR0 register
 */

uint16_t stopISD();
/*
 * Stops any active operation of the ISD1700 device
 *
 * If a stop command is sent while the ISD1700 device is currently busy,
 * the interrupt flag is set.
 *
 * Affects the ISD1700 device's interrupt flag
 * OUTPUT: SR0 register
 */

uint16_t beginISDRecording(uint16_t record_ptr);
/*
 * Begins recording indefinitely to the ISD1700 at a given address
 *
 * Recording should be stopped by using stopISD().
 * If end of memory is reached, recording stops and the ISD1700's interrupt register is set.
 *
 * An invalid recording address will cause undefined behavior.
 *
 * The analogue input must be enabled before this function is called.
 *
 * Affects the ISD1700 device's REC_PTR register
 * INPUT:  Recording start address
 * OUTPUT: SR0 register
 */

uint16_t beginISDPlayback(uint16_t play_ptr);
/*
 * Begins playback from the ISD1700 at a given address
 *
 * Playback should be stopped manually by using stopISD().
 * If stopISD() is not sent during or at the end of playback,
 * playback will continue to play to the end of the ISD1700 device's memory.
 *
 * An invalid playback address will default to playing the entirety of the ISD1700 device's memory.
 *
 * The speaker must be enabled before this function is called.
 *
 * Affects the ISD1700 device's SR0 register
 * INPUT:  Playback start address
 * OUTPUT: SR0 register
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

uint16_t clearIntISD();
/*
 * Clears the interrupt flag of the ISD1700 device
 *
 * Affects the ISD1700 device's interrupt flag
 * OUTPUT: SR0 register
 */

uint16_t getCurrPtrISD(uint16_t sr0_register);
/*
 * Gets the current pointer of the ISD1700 device given the sr0 register
 *
 * OUTPUT: Current address of the ISD1700 device
 */


/////////////////////////
// INTERNAL FUNCTIONS
/////////////////////////

uint16_t sendISDCommand(byte command);
/*
 * Sends a basic command to the ISD1700 device
 *
 * These commands consist only of a single command byte, followed by a null byte.
 *
 * All SPI actions are handled automatically.
 *
 * INPUT:  Command to execute
 * OUTPUT: SR0 register
 */


#endif