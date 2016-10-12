// TODO
// configureUART();

uint16_t getStartAddress() {
	bool Valid_Addr = false;
	uint16_t Start_Addr;

	while(!Valid_Addr) {
		Start_Addr = promptStartAddress();
		if((Start_Addr >= ISD_MIN_ADDR) && (Start_Addr <= ISD_MAX_ADDR)) {
			Valid_Addr = true;
		}
		else {
			Serial.write(ERR_INV_ADDR_STR);
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
	Serial.write(DURATION_STR);
	Serial.flush();
	while(!Read_Complete){
		if(Serial.available()){
			char Read_Digit = Serial.read();

			if(Input_Digits > 0) {
				if(Read_Digit == '\b') {
					Serial.write('\b');
					Start_Addr = (Start_Addr / 10);
					Input_Digits--;
				}
				if(Read_Digit == '\r' || Read_Digit == '\n') {
					Serial.write("\r\n");
					Read_Complete = true;
				}
			}

			if(Input_Digits < ISD_DUR_MAX_DIGITS) {
				if(Read_Digit >= '0' && Read_Digit <= '9') {
					Start_Addr = (Start_Addr * 10) + (Read_Digit - '0');
					Serial.write(Read_Digit);
					Input_Digits++;
				}
			}
		}
	}
	return Rec_Duration;
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
	Serial.write(START_ADDR_STR);
	Serial.flush();
	while(!Read_Complete){
		if(Serial.available()){
			char Read_Digit = Serial.read();
			if(Read_Digit >= 'a' && Read_Digit <= 'f') {
				Read_Digit -= ('a' - 'A');
			}

			if(Input_Digits > 0) {
				if(Read_Digit == '\b') {
					Serial.write('\b');
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
	}
	return Start_Addr;
}