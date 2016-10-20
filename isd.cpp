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

void eraseISD() {
	sendISDCommand(ISD_G_ERASE);
	return;
}

void configISDAPC(byte volume) {
	uint16_t ISD_APC_Config = ISD_APC_DEFAULT_CONFIG;

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
	SPI.transfer16(ISD_APC_Config);
	digitalWrite(ISD_SS_PIN, HIGH);
	SPI.endTransaction();

	return;
}

void stopISD() {
	sendISDCommand(ISD_STOP);
	return;
}

void beginISDRecording(uint16_t record_ptr) {
	SPI.beginTransaction(ISD_SPI_SETTINGS);
	digitalWrite(ISD_SS_PIN, LOW);
	SPI.transfer16((uint16_t) ISD_SET_REC);
	SPI.transfer16(record_ptr);
	SPI.transfer16(ISD_MAX_ADDR);
	SPI.transfer(0x00);
	digitalWrite(ISD_SS_PIN, HIGH);
	SPI.endTransaction();
	return;
}

void beginISDPlayback(uint16_t play_ptr) {
	if((play_ptr < ISD_MIN_ADDR) || (play_ptr > ISD_MAX_ADDR)) {
		play_ptr = ISD_MIN_ADDR;
	}

	SPI.beginTransaction(ISD_SPI_SETTINGS);
	digitalWrite(ISD_SS_PIN, LOW);
	SPI.transfer16((uint16_t) ISD_SET_PLAY);
	SPI.transfer16(play_ptr);
	SPI.transfer16(ISD_MAX_ADDR);
	SPI.transfer(0x00);
	digitalWrite(ISD_SS_PIN, HIGH);
	SPI.endTransaction();

	return;
}

uint16_t getCurrPtrISD() {
	uint16_t Record_Ptr;

	SPI.beginTransaction(ISD_SPI_SETTINGS);
	digitalWrite(ISD_SS_PIN, LOW);
	Record_Ptr = SPI.transfer16((uint16_t) ISD_CLR_INT);
	digitalWrite(ISD_SS_PIN, HIGH);
	SPI.endTransaction();

	Record_Ptr = (Record_Ptr >> 5);
	return Record_Ptr;
}

bool ISDInterrupted() {
	return(!digitalRead(ISD_INT_PIN));
}

void clearIntISD() {
	sendISDCommand(ISD_CLR_INT);
	return;
}

void sendISDCommand(byte command) {
	SPI.beginTransaction(ISD_SPI_SETTINGS);
	digitalWrite(ISD_SS_PIN, LOW);
	SPI.transfer16((uint16_t) command);
	digitalWrite(ISD_SS_PIN, HIGH);
	SPI.endTransaction();
	return;
}