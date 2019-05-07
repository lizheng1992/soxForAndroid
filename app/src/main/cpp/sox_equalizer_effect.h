#ifndef SOX_EQUALIZER_EFFECT_H_
#define SOX_EQUALIZER_EFFECT_H_
#include "./sox_base_effect.h"
#include "sox_filter_equalizer_param.h"
#include <list>

/**
 * 均衡器
 */
class SoxEqualizerEffect: public SoxBaseEffect {
public:
	static short* preProcessSamples;
	static int size;
	static short* responseData;

protected:
	sox_effects_chain_t * equalizerChain;


public:
	SoxEqualizerEffect(double sampleRate, int channels);
	~SoxEqualizerEffect();
	/** 初始化全局变量 **/
	virtual int init(int bufferSize);
	/** 销毁 **/
	virtual void destroy();
	/** initReverbEffect chain **/
	void initEqualizerChain(std::list<SOXEqualizerFilterParam*>* equalizerFilterParams);
	/** delete chain **/
	virtual void destroyChain();
	/** 由边录边和调用的方法 **/
	virtual int process(short* audioSample, int size);
};
#endif /* SOX_EQUALIZER_EFFECT_H_ */
