package com.xueersiwx.lib.sox;

public class SoxUtils {
    static {
        System.loadLibrary("xessox-lib");
    }

    public native void soxAudio(String inputPath, String outputPath);

    public native boolean initSoxAudio(int bufferSize, double sampleRate, int channels, int reverberance, int damping, int roomScale, int stereoDepth, int preDelay, int wetGain);

    public native short[] process(short[] audioSample, int size);

    public native void destory();


    public  native void init(int bufferSize);

    public native  short[] process_t(short[] audioSample, int size);

}
