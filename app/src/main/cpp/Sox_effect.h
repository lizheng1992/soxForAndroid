//
// Created by 李錚 on 2019/3/18.
//

#ifndef SOXCODE_SOX_EFFECT_H
#define SOXCODE_SOX_EFFECT_H

#include "sox.h"
#include "Log.h"
class Sox_effect {
/** 初始化全局变量 **/
public:

    static short* preProcessSamples;
    static int size;
    static short* responseData;

public:
	sox_encodinginfo_t encoding;
    sox_signalinfo_t signal;
    sox_effects_chain_t * chain;



     int init(int bufferSize);

      void initEncoding();

    /** 销毁 **/
    void destroy();




    /** init chain **/
      int initChain();



    /** delete chain **/
    void destroyChain();

    /** 由边录边和调用的方法 **/
     int process(short *audioSample, int size);

    void initSignal(){
        signal.precision = 16;
        signal.channels = 2;
        signal.rate = 44100;
        signal.length = 88200 * 15 * 60;
        signal.mult = NULL;

    }

};


#endif //SOXCODE_SOX_EFFECT_H
