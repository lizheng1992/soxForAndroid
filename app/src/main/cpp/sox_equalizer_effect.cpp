#include "./sox_equalizer_effect.h"

/**
 * 	std::list<SOXEqualizerFilterParam*> * equalizerFilterParams = new std::list<SOXEqualizerFilterParam*>();
	SOXEqualizerFilterParam* equalizerFilterParam = new SOXEqualizerFilterParam(1600, 2.0, 3);
	equalizerFilterParams->push_back(equalizerFilterParam);
	equalizerFilterParam = new SOXEqualizerFilterParam(5600, 0.2, 10);
	equalizerFilterParams->push_back(equalizerFilterParam);
	equalizerFilterParam = new SOXEqualizerFilterParam(17000, 0.1, -6);
	equalizerFilterParams->push_back(equalizerFilterParam);
 */


#include "Log.h"
SoxEqualizerEffect::SoxEqualizerEffect(double sampleRate, int channels) :
		SoxBaseEffect(sampleRate, channels) {
	equalizerChain = NULL;
}

SoxEqualizerEffect::~SoxEqualizerEffect() {
}


static int input_drain(sox_effect_t * effp, sox_sample_t * obuf, size_t * osamp) {
//	LOGI("input_drain success...");
	//note that:永远正常结束，但是当外边程序主动调用AudioEffectProcessor的destroy的时候停止
	*osamp = SoxEqualizerEffect::size;
	int i = 0;
	for (; i < *osamp; i++) {
		obuf[i] = ((sox_sample_t) (SoxEqualizerEffect::preProcessSamples[i]) << 16);
	}
	return SOX_SUCCESS;
}

static int output_flow(sox_effect_t *effp LSX_UNUSED, sox_sample_t const * ibuf, sox_sample_t * obuf LSX_UNUSED, size_t * isamp, size_t * osamp) {
	if (*isamp) {
		int i = 0;
		for (; i < *isamp; i++) {
			SoxEqualizerEffect::responseData[i] = (short) (ibuf[i] >> 16);
		}
	}
	*osamp = 0;
//	LOGI("output_flow success...");
	return *isamp == 0 ? SOX_SUCCESS : SOX_EOF;
}

static sox_effect_handler_t const * input_handler(void) {
	static sox_effect_handler_t handler = { "input", NULL, SOX_EFF_MCHAN | SOX_EFF_MODIFY, NULL, NULL, NULL, input_drain, NULL, NULL, 0 };
	return &handler;
}

static sox_effect_handler_t const * output_handler(void) {
	static sox_effect_handler_t handler = { "output", NULL, SOX_EFF_MCHAN | SOX_EFF_MODIFY | SOX_EFF_PREC, NULL, NULL, output_flow, NULL, NULL, NULL, 0 };
	return &handler;
}

short* SoxEqualizerEffect::preProcessSamples = NULL;
int SoxEqualizerEffect::size = 0;
short* SoxEqualizerEffect::responseData = NULL;

/** 初始化全局变量 **/
int SoxEqualizerEffect::init(int bufferSize) {
	responseData = new short[bufferSize * 2];
	return SoxBaseEffect::init(bufferSize);
}

/** 销毁 **/
void SoxEqualizerEffect::destroy() {
	if (NULL != responseData) {
		delete[] responseData;
		responseData = NULL;
        sox_quit();
	}
}

int SoxEqualizerEffect::process(short* audioSamples, int sizeParam) {
	size = sizeParam;
	preProcessSamples = audioSamples;
	sox_flow_effects(equalizerChain, NULL, NULL);
	memcpy(audioSamples, responseData, size * 2);
	return size;
}

void SoxEqualizerEffect::destroyChain() {
	if (NULL != equalizerChain) {
		sox_delete_effects_chain(equalizerChain);
	}
}

void SoxEqualizerEffect::initEqualizerChain(std::list<SOXEqualizerFilterParam*>* equalizerFilterParams) {
	equalizerChain = sox_create_effects_chain(&encoding, &encoding);
	sox_effect_t * e = sox_create_effect(input_handler());
	assert(sox_add_effect(equalizerChain, e, &signal, &signal) == SOX_SUCCESS);
	free(e);

	if (NULL != equalizerFilterParams) {
		std::list<SOXEqualizerFilterParam*>::iterator ite;
		for (ite = equalizerFilterParams->begin(); ite != equalizerFilterParams->end(); ++ite) {
			SOXEqualizerFilterParam* equalizerFilterParam = *ite;
			//下面这段代码 validate EQ参数
			int frequency = equalizerFilterParam->getFrequencyIntegerType();
			if (frequency > signal.rate / 2) {
				continue;
			}
			int filterType = equalizerFilterParam->getType();
			if (STANDARD_EQ == filterType) {
				//添加一个eq效果器
				int equalizerArgs = 3;
				char* frequency = new char[10];
				equalizerFilterParam->getFrequency(&frequency);
				char* width = new char[10];
				equalizerFilterParam->getWidth(&width);
				char* gain = new char[10];
				equalizerFilterParam->getGain(&gain);
				char * equalizerArgv[] = { frequency, width, gain };
				LOGE("equalizer is %s %s %s \n", equalizerArgv[0], equalizerArgv[1], equalizerArgv[2]);
				e = sox_create_effect(sox_find_effect("equalizer"));
				assert(sox_effect_options(e, equalizerArgs, equalizerArgv) == SOX_SUCCESS);
				for (int i = 0; i < equalizerArgs; i++) {
					delete[] equalizerArgv[i];
				}
			} else if (LOW_PASS == filterType) {
				//添加一个低通滤波器
				int lowpassArgs = 2;
				char* frequency = new char[10];
				equalizerFilterParam->getFrequency(&frequency);
				char* width = new char[10];
				equalizerFilterParam->getWidth(&width);
				char * lowpassArgv[] = { frequency, width };
				LOGE("lowpass is %s %s \n", lowpassArgv[0], lowpassArgv[1]);
				e = sox_create_effect(sox_find_effect("lowpass"));
				assert(sox_effect_options(e, lowpassArgs, lowpassArgv) == SOX_SUCCESS);
				for (int i = 0; i < lowpassArgs; i++) {
					delete[] lowpassArgv[i];
				}
			} else if (HIGH_PASS == filterType) {
				//添加一个高通滤波器
				int highpassArgs = 2;
				char* frequency = new char[10];
				equalizerFilterParam->getFrequency(&frequency);
				char* width = new char[10];
				equalizerFilterParam->getWidth(&width);
				char * highpassArgv[] = { frequency, width };
				LOGE("highpass is %s %s \n", highpassArgv[0], highpassArgv[1]);
				e = sox_create_effect(sox_find_effect("highpass"));
				assert(sox_effect_options(e, highpassArgs, highpassArgv) == SOX_SUCCESS);
				for (int i = 0; i < highpassArgs; i++) {
					delete[] highpassArgv[i];
				}
			}
			assert(sox_add_effect(equalizerChain, e, &signal, &signal) == SOX_SUCCESS);
			free(e);
		}
	}

	e = sox_create_effect(output_handler());
	assert(sox_add_effect(equalizerChain, e, &signal, &signal) == SOX_SUCCESS);
	free(e);
}
