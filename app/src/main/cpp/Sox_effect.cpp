//
// Created by 李錚 on 2019/3/18.
//


#include <assert.h>
#include <string.h>
#include "Sox_effect.h"
#include "math.h"

short *Sox_effect::preProcessSamples = NULL;
int Sox_effect::size = 0;
short *Sox_effect::responseData = NULL;


static int drain_cnt_down = 1;
static int flow_frequency = 0;

static int input_drain(sox_effect_t *effp, sox_sample_t *obuf, size_t *osamp) {

    //	LOGI("input_drain success...");
    //note that:永远正常结束，但是当外边程序主动调用AudioEffectProcessor的destroy的时候停止
    if(drain_cnt_down > 0){
        drain_cnt_down = 0;
        *osamp = Sox_effect::size;
        int i = 0;
        for (; i < *osamp; i++) {
            obuf[i] = ((sox_sample_t) (Sox_effect::preProcessSamples[i]) << 16);
        }
//		LOGI("input_drain ... *osamp is %d return SOX_SUCCESS", *osamp);
        return SOX_SUCCESS;
    } else{
//		LOGI("input_drain once execute must return SOX_EOF");
        *osamp = 0;
        return SOX_EOF;
    }
}

static int
output_flow(sox_effect_t *effp LSX_UNUSED, sox_sample_t const *ibuf, sox_sample_t *obuf LSX_UNUSED,
            size_t *isamp, size_t *osamp) {
//	LOGI("reverb_output_flow output_flow... *isamp is %d *osamp is %d", *isamp, *osamp);
    if (*isamp) {
        int i = 0;
        for (; i < *isamp; i++) {
            Sox_effect::responseData[i + (*isamp * flow_frequency)] = (short) (ibuf[i] >> 16);
        }
        flow_frequency++;
    }
    *osamp = 0;
//	LOGI("output_flow success...");
    return SOX_SUCCESS;
}


static sox_effect_handler_t const *input_handler(void) {
    static sox_effect_handler_t handler = {"input", NULL, SOX_EFF_MCHAN | SOX_EFF_MODIFY, NULL,
                                           NULL, NULL, input_drain, NULL, NULL, 0};
    return &handler;
}

static sox_effect_handler_t const *output_handler(void) {
    static sox_effect_handler_t handler = {"output", NULL,
                                           SOX_EFF_MCHAN | SOX_EFF_MODIFY | SOX_EFF_PREC, NULL,
                                           NULL, output_flow, NULL, NULL, NULL, 0};
    return &handler;
}


int Sox_effect::init(int bufferSize) {
    sox_init();
    sox_globals.bufsiz = bufferSize;
    responseData=new short[bufferSize];
    initSignal();
    initEncoding();
    initChain();
    return 0;
}


int Sox_effect::initChain() {

    LOGE("开始初始化");



    //初始化效果器链

    chain = sox_create_effects_chain(&encoding, &encoding);

    //配置输入效果器
    sox_effect_t *inputEffect;
    inputEffect = sox_create_effect(input_handler());

    assert(sox_add_effect(chain, inputEffect, &signal, &signal)==SOX_SUCCESS);
    delete (inputEffect);


    /* //回声处理器
     sox_effect_t *echoEffect;
     echoEffect = sox_create_effect(sox_find_effect("echo"));
     // char *wetOnly = "-w";
     char *inputVolume = "0.8";
     char *outputVolume = "0.88";
     char *delay = "100";

     char *attenuation = "0.3";//衰减

     char *args2[] = { inputVolume, outputVolume, delay, attenuation};
     sox_effect_options(echoEffect, 4, args2);
     sox_add_effect(chain, echoEffect, &in->signal, &in->signal);
     delete (echoEffect);*/

    //混音处理器 效果器
    sox_effect_t *e;
    e = sox_create_effect(sox_find_effect("reverb"));
    // char *wetOnly = "-w";
    char *reverbrance = "30";
    char *hfDamping = "30";
    char *roomScale = "30";

    char *stereoDepth = "30";
    char *preDelay = "30";
    char *wetGain = "0";
    char *args1[] = {reverbrance, hfDamping, roomScale, stereoDepth, preDelay, wetGain};
    int ref = sox_effect_options(e, 6, args1);
    sox_add_effect(chain, e, &signal, &signal);
    delete (e);


    //音量增强效果器
    char *args[10];
    sox_effect_t *volEffect;
    volEffect = sox_create_effect(sox_find_effect("vol"));
    args[0] = "5dB";
    sox_effect_options(volEffect, 1, args);
    sox_add_effect(chain, volEffect, &signal, &signal);
    delete (volEffect);



    //输出效果器
    e = sox_create_effect(output_handler());
    assert(sox_add_effect(chain, e, &signal, &signal) == SOX_SUCCESS);
    delete (e);

    LOGE("初始化完成")


    return 0;
}

void Sox_effect::destroyChain() {
    if (NULL != chain) {
        sox_delete_effects_chain(chain);
    }
}

int Sox_effect::process(short *audioSample, int sizeParam) {
    size=sizeParam;
    preProcessSamples=audioSample;
    sox_flow_effects(chain, NULL, NULL);
    memcpy(audioSample, responseData,size);
    return size;
}

void Sox_effect::initEncoding() {
    encoding.encoding = (sox_encoding_t) 1;
    encoding.bits_per_sample = 16;
    encoding.reverse_bytes = (sox_option_t) 0;
    encoding.reverse_nibbles = (sox_option_t) 0;
    encoding.reverse_bits = (sox_option_t) 0;
    encoding.compression = INFINITY;
    encoding.opposite_endian = (sox_bool) 0;
}

void Sox_effect::destroy() {
     if(NULL != responseData){
         delete[] responseData;
         responseData = NULL;
     }
}