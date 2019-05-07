package com.xueersiwx.lib.sox;

import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;

import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;

public class MainActivity extends AppCompatActivity {
    private EditText edt;
    private EditText edt1;
    private EditText edt2;


    boolean isSwitch=false;

    private float mParams[][];
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        Button button = findViewById(R.id.sample_text);
        edt=findViewById(R.id.edt_value);
        edt1=findViewById(R.id.edt_value1);
        edt2=findViewById(R.id.edt_value2);

        findViewById(R.id.btn_switch_jhq).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                isSwitch=true;
                mParams = new float[][]{
                        {200, 2.0f, Float.valueOf(edt.getText().toString()).floatValue()},
                        {300, 0.2f, Float.valueOf(edt1.getText().toString()).floatValue()},
                        {400, 0.1f, -Float.valueOf(edt2.getText().toString()).floatValue()}
                };
            }
        });
        button.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                SoxUtils soxUtils = new SoxUtils();
                soxUtils.soxAudio("/sdcard/test.wav", "/sdcard/out.wav");

            }
        });


        findViewById(R.id.btn_jhq).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {

                float params[][] = new float[][]{
                        {200, 2.0f, 3f},
                        {300, 0.2f, 10},
                        {400, 0.1f, -6}
                };
                SoxUtils._initEqualizerEffect(44100 * 2, 44100, 2,params);
            }
        });


    }


    public void init(View view) {
        SoxUtils.initReverbEffect(44100 * 2, 44100, 2,
                80,
                30,
                100,
                30,
                30,
                8);
    }

    public void process(View view) {
        new Thread(new Runnable() {
            @Override
            public void run() {
                try {
                    InputStream is = new FileInputStream("/sdcard/mydream.pcm");
                    OutputStream os = new FileOutputStream("/sdcard/mydreamequalizer.pcm");
                    byte[] b = new byte[2048];

                    int i = 0;
                    //定义一个记录索引的变量
                    int len = 0;
                    //循环读取每个数据
                    while ((len = is.read(b)) != -1) {//把读取的数据放到i中
                        short[] shorts = bytesToShort(b);
                        if(isSwitch){
                            isSwitch=false;
                            SoxUtils.destoryEqualChain();
                            SoxUtils._initEqualizerChain(mParams);
                        }
                        short[] process = SoxUtils.doEqualProcess(shorts, len / 2);
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
