#include "isd.h"

void initISD() {  // TODO
	pinMode(ISD_INT_PIN, INPUT);
	digitalWrite(ISD_SS_PIN, HIGH);
	pinMode(ISD_SS_PIN, OUTPUT);
	SPI.begin();
	configISD(true, ISD_DEFAULT_VOL);
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

bool ISDInterrupted() {
	return digitalRead(ISD_INT_PIN);
}

void clearIntISD() {
	sendISDCommand(ISD_CLR_INT);
	return;
}

uint16_t getRecPtrISD() {  // TODO
}

void beginISDRecording(record_ptr) {  // TODO
	return;
}

void beginISDPlayback(play_ptr, volume) {
	if((play_ptr < ISD_MIN_ADDR) || (play_ptr > ISD_MAX_ADDR)) {
		play_ptr = ISD_MIN_ADDR;
	}

	configISD(false, volume);

	SPI.beginTransaction(ISD_SPI_SETTINGS);
	digitalWrite(SPI_SS_PIN, LOW);
	SPI.transfer16((uint16_t) ISD_SET_PLAY);
	SPI.transfer16(play_ptr);
	SPI.transfer16(ISD_MAX_ADDR);
	SPI.transfer(0x00);
	digitalWrite(SPI_SS_PIN, HIGH);
	SPI.endTransaction();

	return;
}

void configISD(feedthrough, volume) {
	uint16_t ISD_APC_Config = ISD_APC_DEFAULT_CONFIG;

	if(!feedthrough) {
		ISD_APC_Config |= ISD_FT_MASK;
	}

	if(volume != (volume & ISD_VOL_MASK)) {
		volume = ISD_VOL_MASK;
	}
	ISD_APC_Config |= ((uint16_t) volume);

	SPI.beginTransaction(ISD_SPI_SETTINGS);
	digitalWrite(SPI_SS_PIN, LOW);
	SPI.transfer(ISD_WR_APC2);
	SPI.transfer16(ISD_APC_Config);
	digitalWrite(SPI_SS_PIN, HIGH);
	SPI.endTransaction();

	return;
}

void sendISDCommand(command) {
	SPI.beginTransaction(ISD_SPI_SETTINGS);
	digitalWrite(SPI_SS_PIN, LOW);
	SPI.transfer16((uint16_t) command);
	digitalWrite(SPI_SS_PIN, HIGH);
	SPI.endTransaction();
	return;
}