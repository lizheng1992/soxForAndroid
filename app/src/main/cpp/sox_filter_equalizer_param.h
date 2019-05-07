#ifndef BASE_AUDIOEFFECT_SOX_EQUALIZER_FILTER_H_
#define BASE_AUDIOEFFECT_SOX_EQUALIZER_FILTER_H_


enum FILTER_TYPE {
	LOW_PASS, HIGH_PASS, STANDARD_EQ,
};

class SOXEqualizerFilterParam {

private:
	/** EQ类型  **/
	int type;
	/** EQ参数  **/
	int frequency;
	//注意这里单位是q
	float width;
	int gain;

public:
	SOXEqualizerFilterParam();
	SOXEqualizerFilterParam(int frequency, float width, int gain);
	SOXEqualizerFilterParam(enum FILTER_TYPE type, int frequency, float width);
	virtual ~SOXEqualizerFilterParam();

	bool equals(SOXEqualizerFilterParam* reverbFilterParam);
	int getFrequencyIntegerType(){
		return frequency;
	};
	float getWidthIntegerType(){
		return width;
	};
	int getGainIntegerType(){
		return gain;
	};
	int getType(){
		return type;
	};
	void getFrequency(char** frequency);
	void getWidth(char** width);
	void getGain(char** gain);
};

#endif /* BASE_AUDIOEFFECT_SOX_EQUALIZER_FILTER_H_ */
