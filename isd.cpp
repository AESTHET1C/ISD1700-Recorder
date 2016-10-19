#include "isd.h"

void initISD() {
	pinMode(ISD_INT_PIN, INPUT);
	digitalWrite(ISD_SS_PIN, HIGH);
	pinMode(ISD_SS_PIN, OUTPUT);
	SPI.begin();
	powerUpISD();
	configISD(true, ISD_DEFAULT_VOL);
	powerDownISD();
	return;
}

void powerUpISD() {
	sendISDCommand(ISD_PU);
	return;
}

void powerDownISD() {
	sendISDCommand(ISD_PD);
	return;
}

void stopISD() {
	sendISDCommand(ISD_STOP);
	clearIntISD();
	configISD(true, ISD_DEFAULT_VOL);
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

void beginISDPlayback(uint16_t play_ptr, byte volume) {
	if((play_ptr < ISD_MIN_ADDR) || (play_ptr > ISD_MAX_ADDR)) {
		play_ptr = ISD_MIN_ADDR;
	}

	configISD(false, volume);

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

uint16_t getRecPtrISD() {
	uint16_t Record_Ptr;

	SPI.beginTransaction(ISD_SPI_SETTINGS);
	digitalWrite(ISD_SS_PIN, LOW);
	SPI.transfer16((uint16_t) ISD_RD_REC_PTR);
	Record_Ptr = SPI.transfer16(0x0000);
	digitalWrite(ISD_SS_PIN, HIGH);
	SPI.endTransaction();

	return Record_Ptr;
}

bool ISDInterrupted() {
	return digitalRead(ISD_INT_PIN);
}

void configISD(bool feedthrough, byte volume) {
	uint16_t ISD_APC_Config = ISD_APC_DEFAULT_CONFIG;

	if(!feedthrough) {
		ISD_APC_Config |= ISD_FT_MASK;
	}

	if(volume != (volume & ISD_VOL_MASK)) {
		volume = ISD_VOL_MASK;
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

void sendISDCommand(byte command) {
	SPI.beginTransaction(ISD_SPI_SETTINGS);
	digitalWrite(ISD_SS_PIN, LOW);
	SPI.transfer16((uint16_t) command);
	digitalWrite(ISD_SS_PIN, HIGH);
	SPI.endTransaction();
	return;
}

void clearIntISD() {
	sendISDCommand(ISD_CLR_INT);
	return;
}