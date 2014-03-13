package com.tomaskimer.gles3mark;

import android.os.Bundle;

import android.app.Activity;
import android.content.Intent;
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
    
    public void StartBenchmark(View view) {
    	Toast.makeText(getApplicationContext(), "Benchmark started", Toast.LENGTH_SHORT).show();
    	Intent intent = new Intent(this, BenchmarkActivity.class);
    	startActivityForResult(intent, BENCH_REQUEST_ID); // startActivity(intent);
    }
    
    @Override
	protected void onActivityResult(int requestCode, int resultCode, Intent data) {
    	Log.e("onMainActivityResult", "req:" + String.format("%d", requestCode) + ", res: " + String.format("%d", resultCode));
    	if (requestCode == BENCH_REQUEST_ID) {
            if (resultCode == RESULT_OK) {  // -1,  0 = RESULT_CANCELED
              String myValue = data.getStringExtra("score"); 
              Toast.makeText(getApplicationContext(), "Score: " + myValue, Toast.LENGTH_SHORT).show();
              Log.e("MainActivityResult", "Score: " + myValue);
            }
        }    	
    	//super.onActivityResult(requestCode, resultCode, data);
	}
}
