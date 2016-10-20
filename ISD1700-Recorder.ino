#include "ISD1700-Recorder.h"

void setup() {
	initISD();
	initUART();
	Serial.write("\r\n");
	if(confirmISDFlashErase()) {
		clearIntISD();
		eraseISD();
		printFlashString(ERASING_STR);
		while(!ISDInterrupted()) {
			// Do Nothing
		}
		clearIntISD();
	}
	else {
		printFlashString(ERASE_NOTE_STR);
	}
}

void loop() {
	uint16_t Start_Ptr = getStartAddress();
	unsigned long Record_Duration = getRecordDuration();

	printFlashString(WAITING_STR);
	while(analogRead(AUDIO_IN_PIN) < AUDIO_THRESHOLD) {
		// Do nothing
	}
	Serial.flush();
	clearIntISD();
	beginISDRecording(Start_Ptr);
	unsigned long Record_Start = millis();
	printFlashString(RECORDING_STR);

	bool Interrupted = false;
	unsigned long Curr_Duration = 0;
	while((Curr_Duration < Record_Duration) && (!Interrupted)) {
		Interrupted = ISDInterrupted();
		Curr_Duration = millis() - Record_Start;
	}
	stopISD();
	clearIntISD();

	if(Interrupted) {
		printFlashString(ERR_EOM_1_STR);
		Serial.print(Curr_Duration);
		printFlashString(ERR_EOM_2_STR);
	}
	else {
		unsigned long Stop_Ptr = getCurrPtrISD();
		unsigned long Sample_Rate = ((Stop_Ptr - Start_Ptr) * 1000000) / Curr_Duration;

		printFlashString(RECORDING_END_STR);
		Serial.print(Stop_Ptr, HEX);
		Serial.write(".\r\n");
		printFlashString(SR_ESTIMATE_1_STR);
		Serial.print(Sample_Rate);
		printFlashString(SR_ESTIMATE_2_STR);

		bool Playback_State = true;
		while(Playback_State) {
			byte Playback_Volume = getPlaybackVolume();
			if(Playback_Volume > ISD_MIN_VOL) {
				Playback_State = false;
			}
			else {
				configISDAPC(Playback_Volume);
				beginISDPlayback(Start_Ptr);
				unsigned long Playback_Start = millis();
				printFlashString(PLAYBACK_STR);
				while((millis() - Playback_Start) < Curr_Duration) {
					// Do nothing
				}
				stopISD();
				clearIntISD();
				configISDAPC(ISD_DISABLE_SPK);
			}
		}
	}
	Serial.write("\r\n");
}