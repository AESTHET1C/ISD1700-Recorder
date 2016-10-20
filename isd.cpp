#include "isd.h"

void initISD() {
	pinMode(ISD_INT_PIN, INPUT_PULLUP);
	digitalWrite(ISD_SS_PIN, HIGH);
	pinMode(ISD_SS_PIN, OUTPUT);
	SPI.begin();
	sendISDCommand(ISD_PU);
	delay(ISD_T_PUD);
	sendISDCommand(ISD_RESET);
	delay(ISD_T_SET);
	sendISDCommand(ISD_PU);
	delay(ISD_T_PUD);
	configISDAPC(ISD_DISABLE_SPK);
	return;
}

uint16_t eraseISD() {
	return sendISDCommand(ISD_G_ERASE);
}

uint16_t configISDAPC(byte volume) {
	uint16_t ISD_APC_Config = ISD_APC_DEFAULT_CONFIG;
	uint16_t Status_Register;

	if(volume > ISD_MIN_VOL) {
		ISD_APC_Config |= ISD_SPK_MASK;
		volume = ISD_DEFAULT_VOL;
	}
	else {
		ISD_APC_Config |= ISD_FT_MASK;
	}
	ISD_APC_Config |= ((uint16_t) volume);

	SPI.beginTransaction(ISD_SPI_SETTINGS);
	digitalWrite(ISD_SS_PIN, LOW);
	SPI.transfer(ISD_WR_APC2);
	Status_Register = SPI.transfer16(ISD_APC_Config);  // Bytes reversed
	digitalWrite(ISD_SS_PIN, HIGH);
	SPI.endTransaction();

	uint16_t Lower_Byte = Status_Register & 0xFF;
	return ((Status_Register >> 8) | (Lower_Byte << 8));
}

uint16_t stopISD() {
	return sendISDCommand(ISD_STOP);
}

uint16_t beginISDRecording(uint16_t record_ptr) {
	uint16_t Status_Register;

	SPI.beginTransaction(ISD_SPI_SETTINGS);
	digitalWrite(ISD_SS_PIN, LOW);
	Status_Register = SPI.transfer16((uint16_t) ISD_SET_REC);
	SPI.transfer16(record_ptr);
	SPI.transfer16(ISD_MAX_ADDR);
	SPI.transfer(0x00);
	digitalWrite(ISD_SS_PIN, HIGH);
	SPI.endTransaction();
	return Status_Register;
}

uint16_t beginISDPlayback(uint16_t play_ptr) {
	uint16_t Status_Register;
	if((play_ptr < ISD_MIN_ADDR) || (play_ptr > ISD_MAX_ADDR)) {
		play_ptr = ISD_MIN_ADDR;
	}

	SPI.beginTransaction(ISD_SPI_SETTINGS);
	digitalWrite(ISD_SS_PIN, LOW);
	Status_Register = SPI.transfer16((uint16_t) ISD_SET_PLAY);
	SPI.transfer16(play_ptr);
	SPI.transfer16(ISD_MAX_ADDR);
	SPI.transfer(0x00);
	digitalWrite(ISD_SS_PIN, HIGH);
	SPI.endTransaction();

	return Status_Register;
}

bool ISDInterrupted() {
	return(!digitalRead(ISD_INT_PIN));
}

uint16_t clearIntISD() {
	return sendISDCommand(ISD_CLR_INT);
}

uint16_t getCurrPtrISD(uint16_t sr0_register) {
	return (sr0_register >> 5);
}

uint16_t sendISDCommand(byte command) {
	uint16_t Status_Register;

	SPI.beginTransaction(ISD_SPI_SETTINGS);
	digitalWrite(ISD_SS_PIN, LOW);
	Status_Register = SPI.transfer16((uint16_t) command);
	digitalWrite(ISD_SS_PIN, HIGH);
	SPI.endTransaction();
	return Status_Register;
}