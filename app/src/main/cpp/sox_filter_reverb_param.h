#ifndef BASE_AUDIOEFFECT_SOX_REVERB_FILTER_H_
#define BASE_AUDIOEFFECT_SOX_REVERB_FILTER_H_


class SOXReverbFilterParam {

private:
    int reverberance;
    int damping;
    int roomScale;
    int stereoDepth;
    int preDelay;
    int wetGain;

public:
    SOXReverbFilterParam();

    SOXReverbFilterParam(int reverberance, int damping, int roomScale, int stereoDepth,
                         int preDelay, int wetGain);

    virtual ~SOXReverbFilterParam();

    void setStereoDepth(int stereoDepth) {
        this->stereoDepth = stereoDepth;
    };

    bool equals(SOXReverbFilterParam *reverbFilterParam);

    void getWetOnly(char **wetOnly);

    void getReverbrance(char **reverbRance);

    int getReverbranceIntegerType() {
        return reverberance;
    };

    void getDamping(char **damping);

    int getDampingIntegerType() {
        return damping;
    };

    void getRoomScale(char **roomScale);

    int getRoomScaleIntegerType() {
        return roomScale;
    };

    void getStereoDepth(char **stereoDepth);

    int getStereoDepthIntegerType() {
        return stereoDepth;
    };

    void getPreDelay(char **preDelay);

    int getPreDelayIntegerType() {
        return preDelay;
    };

    void getWetGain(char **wetGain);

    int getWetGainIntegerType() {
        return wetGain;
    };

};

#endif /* BASE_AUDIOEFFECT_SOX_REVERB_FILTER_H_ */
