#include "./sox_filter_reverb_param.h"
#include "Log.h"
#include <stdio.h>

#define LOG_TAG "SOXReverbFilterParam"

SOXReverbFilterParam::SOXReverbFilterParam() {
}

SOXReverbFilterParam::SOXReverbFilterParam(int reverberance, int damping, int roomScale,
                                           int stereoDepth, int preDelay, int wetGain) {
    this->reverberance = reverberance;
    this->damping = damping;
    this->roomScale = roomScale;
    this->stereoDepth = stereoDepth;
    this->preDelay = preDelay;
    this->wetGain = wetGain;
    /** echo **/

}

SOXReverbFilterParam::~SOXReverbFilterParam() {

}

bool SOXReverbFilterParam::equals(SOXReverbFilterParam *reverbFilterParam) {
    return reverberance == reverbFilterParam->getReverbranceIntegerType() &&
           damping == reverbFilterParam->getDampingIntegerType() &&
           roomScale == reverbFilterParam->getRoomScaleIntegerType() &&
           stereoDepth == reverbFilterParam->getStereoDepthIntegerType() &&
           preDelay == reverbFilterParam->getPreDelayIntegerType() &&
           wetGain == reverbFilterParam->getWetGainIntegerType();
}

void SOXReverbFilterParam::getWetOnly(char **wetOnly) {
    sprintf(*wetOnly, "%s", "-w");
}

void SOXReverbFilterParam::getReverbrance(char **reverbRance) {
    sprintf(*reverbRance, "%d", this->reverberance);
}

void SOXReverbFilterParam::getDamping(char **damping) {
    sprintf(*damping, "%d", this->damping);
}

void SOXReverbFilterParam::getRoomScale(char **roomScale) {
    sprintf(*roomScale, "%d", this->roomScale);
}

void SOXReverbFilterParam::getStereoDepth(char **stereoDepth) {
    sprintf(*stereoDepth, "%d", this->stereoDepth);
}

void SOXReverbFilterParam::getPreDelay(char **preDelay) {
    sprintf(*preDelay, "%d", this->preDelay);
}

void SOXReverbFilterParam::getWetGain(char **wetGain) {
    sprintf(*wetGain, "%d", this->wetGain);
}
