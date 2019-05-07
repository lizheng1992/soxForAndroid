#include "./sox_reverb_effect.h"
#include "Log.h"

#define LOG_TAG "SoxReverbEffect"

SoxReverbEffect::SoxReverbEffect(double sampleRate, int channels) :
        SoxBaseEffect(sampleRate, channels) {
    reverbChain = NULL;
}

SoxReverbEffect::~SoxReverbEffect() {
}

short *SoxReverbEffect::preProcessSamples = NULL;
int SoxReverbEffect::size = 0;
short *SoxReverbEffect::responseData = NULL;

/** 初始化全局变量 **/
int SoxReverbEffect::init(int bufferSize) {
    responseData = new short[bufferSize * 2];
    return SoxBaseEffect::init(bufferSize);
}

/** 销毁 **/
void SoxReverbEffect::destroy() {
    if (NULL != responseData) {
        delete[] responseData;
        responseData = NULL;
    }
}


void SoxReverbEffect::initReverbOutputSignal(int channels) {
    reverb_out_signal.precision = 16;
    reverb_out_signal.channels = channels;
    reverb_out_signal.rate = inputSampleRate;
    reverb_out_signal.mult = NULL;
}

int SoxReverbEffect::process(short *audioSamples, int sizeParam) {
    size = sizeParam;
    preProcessSamples = audioSamples;
    drain_cnt_down = 1;
    flow_frequency = 0;
    sox_flow_effects(reverbChain, NULL, NULL);
    memcpy(audioSamples, responseData, size*2);
    return size;
}

void SoxReverbEffect::destroyChain() {
    if (NULL != reverbChain) {
        sox_delete_effects_chain(reverbChain);
    }
}

void SoxReverbEffect::initReverbChain(SOXReverbFilterParam *reverbFilterParam) {
    SoxBaseEffect::initSignal();
    SoxReverbEffect::initReverbOutputSignal(inputChannels);

    //-w表示只要混响，不要干声
//	LOGI("signal channels is %d reverb_output_signles channels is %d",  signal.channels, reverb_out_signal.channels);
    reverbChain = sox_create_effects_chain(&encoding, &encoding);
    sox_effect_t *e = sox_create_effect(reverb_input_handler());
    assert(sox_add_effect(reverbChain, e, &signal, &reverb_out_signal) == SOX_SUCCESS);
    free(e);
//	LOGI("signal channels is %d reverb_output_signles channels is %d",  signal.channels, reverb_out_signal.channels);

    if (NULL != reverbFilterParam) {
        int reverbArgs = 7;
        char *wetOnly = new char[10];
        reverbFilterParam->getWetOnly(&wetOnly);
        char *reverbrance = new char[10];
        reverbFilterParam->getReverbrance(&reverbrance);
        char *damping = new char[10];
        reverbFilterParam->getDamping(&damping);
        char *roomScale = new char[10];
        reverbFilterParam->getRoomScale(&roomScale);
        char *stereoDepth = new char[10];
        reverbFilterParam->getStereoDepth(&stereoDepth);
        char *preDelay = new char[10];
        reverbFilterParam->getPreDelay(&preDelay);
        char *wetGain = new char[10];
        reverbFilterParam->getWetGain(&wetGain);


        char *reverbArgv[] = {wetOnly, reverbrance, damping, roomScale, stereoDepth, preDelay,
                              wetGain};
        /*char *reverbArgv[] = {wetOnly, reverbrance, damping, roomScale, stereoDepth, preDelay,
                              wetGain};*/
        LOGE("reverbArgv is wetOnly  %s  reverbrance %s damping %s roomScale %s stereoDepth %s preDelay %s wetGain%s \n",
             reverbArgv[0], reverbArgv[1], reverbArgv[2],
             reverbArgv[3], reverbArgv[4], reverbArgv[5], reverbArgv[6]);
        e = sox_create_effect(sox_find_effect("reverb"));
        int number = sox_effect_options(e, reverbArgs, reverbArgv);

        LOGE("----------------------------------    %d", number);
        assert(number == SOX_SUCCESS);
        assert(sox_add_effect(reverbChain, e, &signal, &reverb_out_signal) == SOX_SUCCESS);
        free(e);
        for (int i = 0; i < reverbArgs; i++) {
            delete[] reverbArgv[i];
        }
    }

//	LOGI("signal channels is %d reverb_output_signles channels is %d",  signal.channels, reverb_out_signal.channels);
    e = sox_create_effect(reverb_output_handler());
    assert(sox_add_effect(reverbChain, e, &signal, &reverb_out_signal) == SOX_SUCCESS);
    free(e);
}
