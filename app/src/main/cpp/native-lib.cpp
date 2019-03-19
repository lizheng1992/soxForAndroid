#include <jni.h>
#include <string>
#include <pthread.h>
#include "Log.h"
#include "stdlib.h"
#include "sox_reverb_effect.h"
#include "sox_filter_reverb_param.h"
#include "Sox_effect.h"

extern "C" {
#include "sox.h"
}

SoxReverbEffect *soxReverbEffect = NULL;
long long fileLength;
Sox_effect *effect = NULL;

int callBack(sox_bool all_done,
             void *client_data) {


    LOGE("callback  : %d ", all_done);
    return 0;
}

extern "C"
JNIEXPORT void JNICALL
Java_com_xueersiwx_lib_sox_SoxUtils_soxAudio(JNIEnv *env, jobject instance, jstring inputPath_,
                                             jstring outputPath_) {
    const char *inputPath = env->GetStringUTFChars(inputPath_, 0);
    const char *outputPath = env->GetStringUTFChars(outputPath_, 0);

    /*env->GetJavaVM(&g_jvm);
    g_obj = env->NewGlobalRef(instance);
 */
    LOGE("开始处理");
    // g_jvm->AttachCurrentThread(&env, NULL);

    int re = sox_init();

    if (re != SOX_SUCCESS) {
        LOGE("初始化 sox 失败");
        return;
    }

    //初始化输入文件
    sox_format_t *in;
    in = sox_open_read(inputPath, NULL, NULL, NULL);

    LOGE("length %lld", lsx_filelength(in));
    fileLength = in->signal.length;
    //初始化输出文件
    sox_format_t *out;
    int i = in->signal.channels;
    LOGE("%d", i);

    out = sox_open_write(outputPath, &in->signal, &in->encoding, NULL, NULL, NULL);


    //初始化效果器链
    sox_effects_chain_t *chain;
    chain = sox_create_effects_chain(&in->encoding, &out->encoding);

    //配置输入效果器
    sox_effect_t *inputEffect;
    inputEffect = sox_create_effect(sox_find_effect("input"));


    char *args[10];
    args[0] = (char *) in;
    sox_effect_options(inputEffect, 1, args);
    sox_add_effect(chain, inputEffect, &in->signal, &in->signal);
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
    sox_add_effect(chain, e, &in->signal, &in->signal);
    delete (e);


    //音量增强效果器
    sox_effect_t *volEffect;
    volEffect = sox_create_effect(sox_find_effect("vol"));
    args[0] = "5dB";
    sox_effect_options(volEffect, 1, args);
    sox_add_effect(chain, volEffect, &in->signal, &in->signal);
    delete (volEffect);
    //输出效果器
    sox_effect_t *outputEffect;
    outputEffect = sox_create_effect(sox_find_effect("output"));
    args[0] = (char *) out;
    sox_effect_options(outputEffect, 1, args);
    sox_add_effect(chain, outputEffect, &in->signal, &in->signal);
    delete (outputEffect);


    void *data;
    sox_flow_effects(chain, (sox_flow_effects_callback) (callBack), data);
    sox_delete_effects_chain(chain);
    sox_close(out);
    sox_close(in);
    sox_quit();
    LOGE("处理完成")

    env->ReleaseStringUTFChars(inputPath_, inputPath);
    env->ReleaseStringUTFChars(outputPath_, outputPath);
}extern "C"
JNIEXPORT jboolean JNICALL
Java_com_xueersiwx_lib_sox_SoxUtils_initSoxAudio(JNIEnv *env, jobject instance, jint bufferSize,
                                                 jdouble sampleRate, jint channels,
                                                 jint reverberance, jint damping, jint roomScale,
                                                 jint stereoDepth, jint preDelay, jint wetGain) {
    soxReverbEffect = new SoxReverbEffect(sampleRate, channels);
    soxReverbEffect->init(bufferSize);
    SOXReverbFilterParam *reverbFilterParam = new SOXReverbFilterParam(reverberance, damping,
                                                                       roomScale, stereoDepth,
                                                                       preDelay, wetGain);
    soxReverbEffect->initChain(reverbFilterParam);

    return true;

}extern "C"
JNIEXPORT jshortArray JNICALL
Java_com_xueersiwx_lib_sox_SoxUtils_process(JNIEnv *env, jobject instance, jshortArray audioSample_,
                                            jint size) {
    int len=size;
    jshort *audioSample = env->GetShortArrayElements(audioSample_, NULL);
    short *temp = static_cast<short *>(malloc(sizeof(short) * len));
    if (soxReverbEffect != NULL) {
        soxReverbEffect->process(audioSample, size);
        LOGE(" jni size %d",len)
        if (size > 0) {
            memcpy(temp, audioSample, len*2);
            jshortArray data = env->NewShortArray(len);
            env->SetShortArrayRegion(data, 0, len, temp);
          free(temp);
            env->ReleaseShortArrayElements(audioSample_, audioSample, 0);
            return data;
        }
    }
    free(temp);
    env->ReleaseShortArrayElements(audioSample_, audioSample, 0);
    return NULL;
}extern "C"
JNIEXPORT void JNICALL
Java_com_xueersiwx_lib_sox_SoxUtils_destory(JNIEnv *env, jobject instance) {

    if (soxReverbEffect != NULL) {
        soxReverbEffect->destroyChain();
        soxReverbEffect->destroy();
        delete (soxReverbEffect);
        soxReverbEffect = NULL;
    }

}extern "C"
JNIEXPORT void JNICALL
Java_com_xueersiwx_lib_sox_SoxUtils_init(JNIEnv *env, jobject instance, jint bufferSize) {

    if (effect == NULL) {
        effect = new Sox_effect();
        effect->init(bufferSize);
    }

}extern "C"
JNIEXPORT jshortArray JNICALL
Java_com_xueersiwx_lib_sox_SoxUtils_process_1t(JNIEnv *env, jobject instance,
                                               jshortArray audioSample_, jint size) {
    jshort *audioSample = env->GetShortArrayElements(audioSample_, NULL);
    LOGE("aaaaa");
    short *temp = static_cast<short *>(malloc(sizeof(short) * size));
    if (effect != NULL) {
        //int refsize = effect->process(audioSample, size);
        memcpy(temp, audioSample, size);
        jshortArray jshortArray = env->NewShortArray(size);
        env->SetShortArrayRegion(jshortArray, 0, size, temp);
        free(temp);
        env->ReleaseShortArrayElements(audioSample_, audioSample, 0);
        return jshortArray;

    }
    free(temp);

    env->ReleaseShortArrayElements(audioSample_, audioSample, 0);

}