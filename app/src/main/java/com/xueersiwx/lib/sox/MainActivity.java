package com.xueersiwx.lib.sox;

import android.inputmethodservice.Keyboard;
import android.renderscript.Script;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.text.Editable;
import android.text.method.KeyListener;
import android.util.Log;
import android.view.KeyEvent;
import android.view.SoundEffectConstants;
import android.view.View;
import android.view.inputmethod.ExtractedText;
import android.view.inputmethod.InputMethodManager;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;

public class MainActivity extends AppCompatActivity {

    // Used to load the 'native-lib' library on application startup.
    SoxUtils soxUtils;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        Button button = findViewById(R.id.sample_text);
        button.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                SoxUtils soxUtils = new SoxUtils();
                soxUtils.soxAudio("/sdcard/test.wav", "/sdcard/out.wav");

            }
        });

    }


    public void init(View view) {
        if (soxUtils == null) {
            soxUtils = new SoxUtils();
 /*           int reverberance, int damping, int roomScale,
            int preDelay, int wetGain, int stereoDepth, int dryWetMix,
            int echoDelay, int echoPredecay, int echoPeriod, int echoDecayRatio,
            int echoPhaseDiff, int echoNum

            {reverbrance, hfDamping, roomScale, stereoDepth, preDelay, wetGain};*/
            soxUtils.initSoxAudio(44100 * 2, 44100, 2,
                    30,
                    30,
                    80,
                    30,
                    30,
                    0);
            // soxUtils.init(44100*2);
        }
    }

    public void process(View view) {
        new Thread(new Runnable() {
            @Override
            public void run() {
                try {
                    InputStream is = new FileInputStream("/sdcard/mydream.pcm");
                    OutputStream os = new FileOutputStream("/sdcard/mydreampppp.pcm");
                    byte[] b = new byte[2048];

                    int i = 0;
                    //定义一个记录索引的变量
                    int len = 0;
                    //循环读取每个数据
                    while ((len = is.read(b)) != -1) {//把读取的数据放到i中
                        short[] shorts = bytesToShort(b);
                        short[] process = soxUtils.process(shorts, len/2);
                        byte[] bytes = shortToBytes(process);
                        os.write(bytes, 0, len);
                       // Log.e("sox", "写入文件: " + len);
                    }
                    Log.e("sox", "文件写入完成: ");
                    is.close();
                    os.close();

                } catch (FileNotFoundException e) {
                    e.printStackTrace();
                } catch (IOException e) {
                    e.printStackTrace();
                }


            }
        }).start();
    }

    public static short[] bytesToShort(byte[] bytes) {
        if (bytes == null) {
            return null;
        }
        short[] shorts = new short[bytes.length / 2];
        ByteBuffer.wrap(bytes).order(ByteOrder.LITTLE_ENDIAN).asShortBuffer().get(shorts);
        return shorts;
    }

    public static byte[] shortToBytes(short[] shorts) {
        if (shorts == null) {
            return null;
        }
        byte[] bytes = new byte[shorts.length * 2];
        ByteBuffer.wrap(bytes).order(ByteOrder.LITTLE_ENDIAN).asShortBuffer().put(shorts);

        return bytes;
    }

}
