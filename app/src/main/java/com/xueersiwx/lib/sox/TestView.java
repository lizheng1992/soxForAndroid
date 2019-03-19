package com.xueersiwx.lib.sox;

import android.content.Context;
import android.support.annotation.Nullable;
import android.text.InputType;
import android.util.AttributeSet;
import android.util.Log;
import android.view.KeyEvent;
import android.view.View;
import android.view.inputmethod.BaseInputConnection;
import android.view.inputmethod.EditorInfo;
import android.view.inputmethod.InputConnection;
import android.view.inputmethod.InputMethodManager;

public class TestView extends View {
    public TestView(Context context) {
        super(context);
        init(context);
    }

    private void init(final Context context) {
        setOnClickListener(new OnClickListener() {
            @Override
            public void onClick(View v) {
                //InputMethodHelper.getInstance(context).showSoftInput(TestView.this);
                InputMethodManager imm = (InputMethodManager) context.getSystemService(Context.INPUT_METHOD_SERVICE);
                imm.toggleSoftInput(0, InputMethodManager.HIDE_NOT_ALWAYS);


            }
        });
    }

    public TestView(Context context, @Nullable AttributeSet attrs) {
        super(context, attrs);
        init(context);
    }

    public TestView(Context context, @Nullable AttributeSet attrs, int defStyleAttr) {
        super(context, attrs, defStyleAttr);
        init(context);
    }

    @Override
    public boolean onCheckIsTextEditor() {
        return true;
    }


    @Override
    public InputConnection onCreateInputConnection(EditorInfo outAttrs) {
        outAttrs.imeOptions = EditorInfo.IME_FLAG_NO_EXTRACT_UI;
        outAttrs.inputType = InputType.TYPE_CLASS_TEXT;
        return new MyInputConnection(this, false);
    }


    public static class MyInputConnection extends BaseInputConnection {

        public MyInputConnection(View targetView, boolean fullEditor) {
            super(targetView, fullEditor);
        }

        @Override
        public boolean commitText(CharSequence text, int newCursorPosition) {
            Log.e("lizheng", text.toString());
            return true;
        }

        @Override
        public boolean sendKeyEvent(KeyEvent event) {
            Log.e("lizheng", event.getAction()+"");
            return  true;
        }

        @Override
        public boolean finishComposingText() {
            Log.e("lizheng", "finishComposingText");
            return true;
        }
    }

    public static class InputMethodHelper {
        private InputMethodManager inputMethodManager;
        private static InputMethodHelper inputMethodHelper;

        InputMethodHelper(Context mContext) {
            inputMethodManager = (InputMethodManager) mContext.getSystemService(Context.INPUT_METHOD_SERVICE);
        }

        public synchronized static InputMethodHelper getInstance(Context mContext) {
            synchronized (InputMethodHelper.class) {
                if (inputMethodHelper == null) {
                    inputMethodHelper = new InputMethodHelper(mContext);
                }
                return inputMethodHelper;
            }
        }

        /**
         * 显示软键盘
         *
         * @param view
         */
        public void showSoftInput(View view) {
            inputMethodManager.showSoftInput(view, 0);
        }

        /**
         * 隐藏输入法
         */
        public void hideSoftInput(View view) {
            if (inputMethodManager.isActive()) {
                Log.d("hickey", "hideSoftInput:" + "hideSoftInputFromWindow");
                inputMethodManager.hideSoftInputFromWindow(view.getWindowToken(), 0);
            }
        }


    }
}
