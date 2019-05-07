#ifndef SOX_REVERB_EFFECT_H_
#define SOX_REVERB_EFFECT_H_

#include "sox_base_effect.h"
#include "sox_filter_reverb_param.h"
#include "Log.h"

/**
 * 混响
 */

class SoxReverbEffect: public SoxBaseEffect {
public:
	static short* preProcessSamples;
	static int size;
	static short* responseData;

protected:
	sox_effects_chain_t * reverbChain;
	sox_effects_chain_t * equalizerChain;


	sox_signalinfo_t reverb_out_signal;
	void initReverbOutputSignal(int channels);
public:
	SoxReverbEffect(double sampleRate, int channels);
	~SoxReverbEffect();
	/** 初始化全局变量 **/
	virtual int init(int bufferSize);
	/** 销毁 **/
	virtual void destroy();
	/** delete chain **/
	virtual void destroyChain();
	/** 由边录边和调用的方法 **/
	virtual int process(short* audioSample, int size);

	void initReverbChain(SOXReverbFilterParam* reverbFilterParam);
};


static int drain_cnt_down = 1;
static int flow_frequency = 0;

static int reverb_input_drain(sox_effect_t * effp, sox_sample_t * obuf, size_t * osamp) {
//	LOGI("input_drain success...");
	*osamp = SoxReverbEffect::size;
	int i = 0;
	for (; i < *osamp; i++) {
		obuf[i] = ((sox_sample_t) (SoxReverbEffect::preProcessSamples[i]) << 16);
	}
	return SOX_SUCCESS;
}

static int reverb_output_flow(sox_effect_t *effp LSX_UNUSED, sox_sample_t const * ibuf, sox_sample_t * obuf LSX_UNUSED, size_t * isamp, size_t * osamp) {
//	LOGI("reverb_output_flow output_flow... *isamp is %d *osamp is %d", *isamp, *osamp);
	if (*isamp) {
		int i = 0;
		for (; i < *isamp; i++) {
			SoxReverbEffect::responseData[i] = (short) (ibuf[i] >> 16);
		}
	}
	*osamp = 0;
//	LOGI("output_flow success...");
	return *isamp == 0 ? SOX_SUCCESS : SOX_EOF;
}

static sox_effect_handler_t const * reverb_input_handler(void) {
	static sox_effect_handler_t handler = { "input", NULL, SOX_EFF_MCHAN | SOX_EFF_MODIFY, NULL, NULL, NULL, reverb_input_drain, NULL, NULL, 0 };
	return &handler;
}

static sox_effect_handler_t const * reverb_output_handler(void) {
	static sox_effect_handler_t handler = { "output", NULL, SOX_EFF_MCHAN | SOX_EFF_MODIFY | SOX_EFF_PREC, NULL, NULL, reverb_output_flow, NULL, NULL, NULL, 0 };
	return &handler;
}
#endif /* SOX_REVERB_EFFECT_H_ */
