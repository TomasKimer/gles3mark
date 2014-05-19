package com.tomaskimer.gles3mark;

import android.annotation.TargetApi;
import android.app.Activity;
import android.app.NativeActivity;
import android.content.Intent;
import android.os.Build;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.view.WindowManager;

public class BenchmarkActivity extends NativeActivity {
	
	static {
		System.loadLibrary("gles3mark");
	}	
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);
	}
	
//	@TargetApi(Build.VERSION_CODES.KITKAT)
	@Override
	public void onWindowFocusChanged(boolean hasFocus) {
		super.onWindowFocusChanged(hasFocus);
		if (hasFocus) {
			getWindow().getDecorView().setSystemUiVisibility(View.SYSTEM_UI_FLAG_LOW_PROFILE);
//			getWindow().getDecorView().setSystemUiVisibility( // TODO handle gl context resize, or call in onCreate?
//	        		  View.SYSTEM_UI_FLAG_LAYOUT_STABLE
//	        		| View.SYSTEM_UI_FLAG_LAYOUT_HIDE_NAVIGATION
//	                | View.SYSTEM_UI_FLAG_LAYOUT_FULLSCREEN
//	                | View.SYSTEM_UI_FLAG_HIDE_NAVIGATION  // 4.0 - lean back https://developer.android.com/training/system-ui/immersive.html
//	                | View.SYSTEM_UI_FLAG_FULLSCREEN       // 4.0
//	                | View.SYSTEM_UI_FLAG_IMMERSIVE_STICKY);
	    }
	}
	
	// called from JNI with JSON result string
    public void FinishMe(String _result) {  // final
    	final String result = _result;
        this.runOnUiThread(new Runnable() {
            public void run() {
            	Intent resultData = new Intent();
        		resultData.putExtra("Result", result);
        		setResult(Activity.RESULT_OK, resultData);
//        		if (getParent() == null) setResult(Activity.RESULT_OK, resultData);
//        		else getParent().setResult(Activity.RESULT_OK, resultData);            	
            	finish();
            }
        });
    }

	@Override
	protected void onStart() {
		super.onStart();
	}

	@Override
	public void onBackPressed() {
		super.onBackPressed();
	}
	
	@Override
	protected void onDestroy() {
		super.onDestroy();		
		Log.d("BenchmarkActivity", String.format("Activity destroyed"));
	}
}
