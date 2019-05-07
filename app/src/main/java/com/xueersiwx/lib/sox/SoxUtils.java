package com.xueersiwx.lib.sox;

public class SoxUtils {
    static {
        System.loadLibrary("xessox-lib");
    }

    public native void soxAudio(String inputPath, String outputPath);

    private static native boolean initSoxAudio(int bufferSize, double sampleRate, int channels, int reverberance, int damping, int roomScale, int stereoDepth, int preDelay, int wetGain);

    //处理
    private static native short[] process(short[] audioSample, int size);

    //销毁
    public static  native void destory();




    public static boolean initReverbEffect(int bufferSize, double sampleRate, int channels, int reverberance, int damping, int roomScale, int stereoDepth, int preDelay, int wetGain) {
        return initSoxAudio(bufferSize, sampleRate, channels, reverberance, damping, roomScale, stereoDepth, preDelay, wetGain);
    }

    public static short[] doReverProcess(short[] audioSample, int size) {
        return process(audioSample, size);
    }




    public static boolean initEqualizerEffect(int bufferSize, double sampleRate, int channels,float[][] params){

        return _initEqualizerEffect(bufferSize,sampleRate,channels,params);

    }

    public static short[] doEqualProcess(short[] audioSample, int size) {
        return _equalProcess(audioSample, size);
    }



    public static  native  void destoryEqual();

    private static native short[] _equalProcess(short[] audioSample, int size);

    public static native boolean  _initEqualizerEffect(int bufferSize, double sampleRate, int channels,float[][] params);

    public static native boolean  _initEqualizerChain(float[][] params);


    public static  native  void destoryEqualChain();

    public static native  void test();


}
