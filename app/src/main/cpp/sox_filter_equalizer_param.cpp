#include <stdio.h>
#include "./sox_filter_equalizer_param.h"


SOXEqualizerFilterParam::SOXEqualizerFilterParam(){
}

SOXEqualizerFilterParam::SOXEqualizerFilterParam(int frequency, float width, int gain) {
	this->type = STANDARD_EQ;
	this->frequency = frequency;
	this->width = width;
	this->gain = gain;
}

SOXEqualizerFilterParam::SOXEqualizerFilterParam(enum FILTER_TYPE type, int frequency, float width) {
	this->type = type;
	this->frequency = frequency;
	this->width = width;
}


SOXEqualizerFilterParam::~SOXEqualizerFilterParam(){

}

bool SOXEqualizerFilterParam::equals(SOXEqualizerFilterParam* reverbFilterParam){
	return frequency == reverbFilterParam->getFrequencyIntegerType() &&
			width == reverbFilterParam->getWidthIntegerType() &&
			gain == reverbFilterParam->getGainIntegerType();
}

void SOXEqualizerFilterParam::getFrequency(char** frequency){
	sprintf(*frequency, "%d", this->frequency);
}
void SOXEqualizerFilterParam::getWidth(char** width){
	sprintf(*width, "%0.2fq", this->width);
}
void SOXEqualizerFilterParam::getGain(char** gain){
	sprintf(*gain, "%d", this->gain);
}
