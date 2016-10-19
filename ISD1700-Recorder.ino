#include "ISD1700-Recorder.h"

void setup() {
	initISD();
	initUART();
}

void loop() {
	uint16_t Start_Ptr = getStartAddress();
	unsigned long Record_Duration = getRecordDuration();

	printFlashString(WAITING_STR);
	while(analogRead(AUDIO_IN_PIN) < AUDIO_THRESHOLD) {
		// Do nothing
	}
	Serial.flush();
	powerUpISD();
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

	if(Interrupted) {
		powerDownISD();
		printFlashString(ERR_EOM_1_STR);
		Serial.print(Curr_Duration);
		printFlashString(ERR_EOM_2_STR);
	}
	else {
		unsigned long Mem_Used = getRecPtrISD() - Start_Ptr;
		powerDownISD();
		printFlashString(SR_ESTIMATE_1_STR);
		Serial.print(Mem_Used / Curr_Duration);
		printFlashString(SR_ESTIMATE_2_STR);

		bool Playback_State = true;
		while(Playback_State) {
			byte Playback_Volume = getPlaybackVolume();
			if(Playback_Volume > ISD_MIN_VOL) {
				Playback_State = false;
			}
			else {
				powerUpISD();
				beginISDPlayback(Start_Ptr, Playback_Volume);
				unsigned long Playback_Start = millis();
				printFlashString(PLAYBACK_STR);
				while((millis() - Playback_Start) < Curr_Duration) {
					// Do nothing
				}
				stopISD();
				powerDownISD();
			}
		}
	}
	Serial.write("\r\n");
}