package com.tomaskimer.gles3mark;

import android.os.Bundle;

import android.app.Activity;
import android.app.ActivityManager;
import android.content.Context;
import android.content.Intent;
import android.content.pm.ConfigurationInfo;
import android.util.Log;
import android.view.Menu;
import android.view.View;
import android.widget.Toast;

public class MainActivity extends Activity {
	
	final static int BENCH_REQUEST_ID = 1234;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.main, menu);
        return true;
    }
    
    public int GetSupportedGLVersion() {
    	final ActivityManager activityManager = (ActivityManager)getSystemService(Context.ACTIVITY_SERVICE);
    	final ConfigurationInfo configurationInfo = activityManager.getDeviceConfigurationInfo();
    	return configurationInfo.reqGlEsVersion;    	
    }
    
    public void StartBenchmark(View view) {
    	int supportedGLversion = GetSupportedGLVersion();
    	if (supportedGLversion < 0x30000) {
    		Toast.makeText(getApplicationContext(), String.format("Unsupported GL ES version: %x.%x (req. 3.0)",
    				supportedGLversion >> 16, supportedGLversion & 0x00FF), Toast.LENGTH_LONG).show();
    		return;
    	} else {
    		Intent intent = new Intent(this, BenchmarkActivity.class);
        	startActivityForResult(intent, BENCH_REQUEST_ID); // startActivity(intent);    		
    	}
    }
    
    @Override
	protected void onActivityResult(int requestCode, int resultCode, Intent data) {
    	if (requestCode == BENCH_REQUEST_ID) {
            if (resultCode == RESULT_OK) {  // -1,  0 = RESULT_CANCELED
              String myValue = data.getStringExtra("score"); 
              Toast.makeText(getApplicationContext(), "Score: " + myValue, Toast.LENGTH_SHORT).show();
              Log.d("BenchmarkActivityResult", "Score: " + myValue);
            }
        }    	
    	super.onActivityResult(requestCode, resultCode, data);
	}
}
