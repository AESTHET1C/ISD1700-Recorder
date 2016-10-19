#include "uart.h"

void initUART() {
	Serial.begin(UART_BAUD);
	return;
}

uint16_t getStartAddress() {
	bool Valid_Addr = false;
	uint16_t Start_Addr;

	while(!Valid_Addr) {
		Start_Addr = promptStartAddress();
		if((Start_Addr >= ISD_MIN_ADDR) && (Start_Addr <= ISD_MAX_ADDR)) {
			Valid_Addr = true;
		}
		else {
			printFlashString(ERR_INV_ADDR_STR);
			Serial.flush();
		}
	}
	return Start_Addr;
}

unsigned long getRecordDuration() {
	bool Read_Complete = false;
	byte Input_Digits = 0;
	unsigned long Rec_Duration = 0;

	flushSerialReadBuffer();
	printFlashString(DURATION_STR);
	Serial.flush();
	while(!Read_Complete){
		while(!Serial.available()){
			// Do nothing
		}
		char Read_Digit = Serial.read();

		if(Input_Digits > 0) {
			if((Read_Digit == '\b') || (Read_Digit == 127)) {
				Serial.write(127);
				Rec_Duration = (Rec_Duration / 10);
				Input_Digits--;
			}
			if(Read_Digit == '\r' || Read_Digit == '\n') {
				Serial.write("\r\n");
				Read_Complete = true;
			}
		}

		if(Input_Digits < ISD_DUR_MAX_DIGITS) {
			if(Read_Digit >= '0' && Read_Digit <= '9') {
				Rec_Duration = (Rec_Duration * 10) + (Read_Digit - '0');
				Serial.write(Read_Digit);
				Input_Digits++;
			}
		}
	}
	return Rec_Duration;
}

byte getPlaybackVolume() {
	bool Read_Complete = false;
	bool Input_Digit = false;
	byte Volume = 0;

	flushSerialReadBuffer();
	printFlashString(VOL_STR);
	Serial.flush();
	while(!Read_Complete) {
		while(!Serial.available()){
			// Do nothing
		}
		char Read_Digit = Serial.read();
		if(Input_Digit) {
			if((Read_Digit == '\b') || (Read_Digit == 127)) {
				Serial.write(127);
				Volume = 0;
				Input_Digit = false;
			}
		}
		else {
			if(Read_Digit >= '0' && Read_Digit <= '9') {
				Serial.write(Read_Digit);
				Volume = (Read_Digit - '0');
				Input_Digit = true;
			}
		}
		if(Read_Digit == '\r' || Read_Digit == '\n') {
			Serial.write("\r\n");
			Read_Complete = true;
		}
	}
	return Volume;
}

void printFlashString(const char string[]) {
	for(unsigned int i = 0; i < strlen_P(string); i++) {
		Serial.write(pgm_read_byte_near(string + i));
	}
	return;
}

void flushSerialReadBuffer() {
	while(Serial.available() > 0) {
		Serial.read();
	}
	return;
}

uint16_t promptStartAddress() {
	bool Read_Complete = false;
	byte Input_Digits = 0;
	uint16_t Start_Addr = 0;

	flushSerialReadBuffer();
	printFlashString(START_ADDR_STR);
	Serial.flush();
	while(!Read_Complete){
		while(!Serial.available()){
			// Do nothing
		}
		char Read_Digit = Serial.read();
		if(Read_Digit >= 'a' && Read_Digit <= 'f') {
			Read_Digit -= ('a' - 'A');
		}

		if(Input_Digits > 0) {
			if((Read_Digit == '\b') || (Read_Digit == 127)) {
				Serial.write(127);
				Start_Addr = (Start_Addr >> 4);
				Input_Digits--;
			}
			if(Read_Digit == '\r' || Read_Digit == '\n') {
				Serial.write("\r\n");
				Read_Complete = true;
			}
		}

		if(Input_Digits < ISD_HEX_ADDR_WIDTH) {
			if(Read_Digit >= '0' && Read_Digit <= '9') {
				Start_Addr = (Start_Addr << 4) + (Read_Digit - '0');
				Serial.write(Read_Digit);
				Input_Digits++;
			}
			if(Read_Digit >= 'A' && Read_Digit <= 'F') {
				Start_Addr = (Start_Addr << 4) + (Read_Digit - 'A' + 10);
				Serial.write(Read_Digit);
				Input_Digits++;
			}
		}
	}
	return Start_Addr;
}