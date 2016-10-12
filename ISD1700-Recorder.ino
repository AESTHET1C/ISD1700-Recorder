program_state Program_State = HALTED;

void setup() {
	// TODO
	// Init pins and such
	// Basically all the setup
}

void loop() {
	uint16_t Start_Ptr = getStartAddress();
	unsigned long Record_Duration = getRecordDuration();

	Serial.write(WAITING_STR);
	while(analogRead(AUDIO_IN_PIN) < AUDIO_THRESHOLD) {
		// Do nothing
	}

	Serial.flush();
	unsigned long Record_Start = millis();
	Serial.write(RECORDING_STR);


	// TODO
	// Redo structure such that there's no massive state machine
	switch(Program_State){
		case default:
			Program_State = HALTED;
		case HALTED:
			// TODO
			// Do nothing?
			break;
		case REC_READY:
			// TODO
			// Prompt for Start_Ptr
			// Prompt for Duration
			if(// TODO Invalid) {
				// TODO
				// Display ERR_INV_STR + ".\n"
				break;
			}
			Program_State = REC_WAIT;
		case REC_WAIT:

			break;
		case RECORDING:
			// TODO
			// Start recording
			Record_Start = millis();
			// Display RECORDING_STR
			while(Program_State == RECORDING) {
				if(/* TODO Interrupt*/) {
					// TODO
					// Clear interrupt
					// Display ERR_EOM_STR
					Program_State = REC_READY;
				}
				if(millis() - Record_Start >= Record_Duration) {
					// TODO
					// Stop recording
					// Get end address
					// Calculate sample rate
					// Display SR_ESTIMATE_STR + sample rate + ".\n"
					Program_State = PB_READY;
				}
			}
			break;
		case PB_READY:
			byte Volume;
			// TODO
			// Prompt with VOL_STR to Volume
			// Convert Volume to usable value
			if(Volume > 7) {
				Program_State = REC_READY;
			}
			else {
				configISD(getISDConfig(Volume));
				Program_State = PLAYBACK;
			}
			break;
		case PLAYBACK:
			// TODO
			// Display PLAYBACK_STR + volume + ".\n"
			Program_State = REC_READY;
			break;
	}

}

uint16_t getISDConfig(byte volume) {
	if(volume > 7) {
		volume = 7;
	}
	return (ISD_AUDIO_CONFIG | (volume & B00000111));
}