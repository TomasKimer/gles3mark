package com.tomaskimer.gles3mark;

import java.util.Locale;
import org.apache.http.HttpResponse;
import org.apache.http.client.methods.HttpPost;
import org.apache.http.entity.StringEntity;
import org.apache.http.impl.client.DefaultHttpClient;
import org.apache.http.util.EntityUtils;
import org.json.JSONException;
import org.json.JSONObject;

import android.annotation.SuppressLint;
import android.app.ActionBar;
import android.app.AlertDialog;
import android.content.DialogInterface;
import android.content.Intent;
import android.net.Uri;
import android.os.AsyncTask;
import android.os.Bundle;
import android.support.v4.app.Fragment;
import android.app.FragmentTransaction; // support.v4 ?
import android.support.v4.app.FragmentActivity;
import android.support.v4.app.FragmentManager;
import android.support.v4.app.FragmentPagerAdapter;
import android.support.v4.view.ViewPager;
import android.text.Html;
import android.text.method.LinkMovementMethod;
import android.text.method.ScrollingMovementMethod;
import android.view.LayoutInflater;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.view.ViewGroup;
import android.webkit.WebView;
import android.webkit.WebViewClient;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;
import android.widget.Toast;

import com.tomaskimer.gles3mark.DeviceInfo;

public class MainActivity extends FragmentActivity implements ActionBar.TabListener {

	final static int BENCH_REQUEST_ID = 1234;
	final static String SERVER_URL = "http://gles3mark.appspot.com/";
	final static String USER_AGENT = "gles3mark_android_app";
	
	DeviceInfo deviceInfo = new DeviceInfo(this);
	JSONObject lastResult = null;
	String lastNickname = "";
	
	/**
	 * The {@link android.support.v4.view.PagerAdapter} that will provide
	 * fragments for each of the sections. We use a
	 * {@link android.support.v4.app.FragmentPagerAdapter} derivative, which
	 * will keep every loaded fragment in memory. If this becomes too memory
	 * intensive, it may be best to switch to a
	 * {@link android.support.v4.app.FragmentStatePagerAdapter}.
	 */
	SectionsPagerAdapter mSectionsPagerAdapter;

	/**
	 * The {@link ViewPager} that will host the section contents.
	 */
	ViewPager mViewPager;

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);

		// Set up the action bar.
		final ActionBar actionBar = getActionBar();
		actionBar.setNavigationMode(ActionBar.NAVIGATION_MODE_TABS);

		// Create the adapter that will return a fragment for each of the three
		// primary sections of the app.
		mSectionsPagerAdapter = new SectionsPagerAdapter(getSupportFragmentManager());

		// Set up the ViewPager with the sections adapter.
		mViewPager = (ViewPager)findViewById(R.id.pager); // in activity_main.xml
		mViewPager.setAdapter(mSectionsPagerAdapter);
		mViewPager.setOffscreenPageLimit(3); 

		// When swiping between different sections, select the corresponding
		// tab. We can also use ActionBar.Tab#select() to do this if we have
		// a reference to the Tab.
		mViewPager.setOnPageChangeListener(
			new ViewPager.SimpleOnPageChangeListener() {
				@Override
				public void onPageSelected(int position) {
					actionBar.setSelectedNavigationItem(position);
				}
			}
		);

		// For each of the sections in the app, add a tab to the action bar.
		for (int i = 0; i < mSectionsPagerAdapter.getCount(); i++) {
			// Create a tab with text corresponding to the page title defined by
			// the adapter. Also specify this Activity object, which implements
			// the TabListener interface, as the callback (listener) for when
			// this tab is selected.
			actionBar.addTab(actionBar.newTab().setText(mSectionsPagerAdapter.getPageTitle(i)).setTabListener(this));
		}
	}

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.main, menu);
		return true;
	}

	@Override
	public void onTabSelected(ActionBar.Tab tab, FragmentTransaction fragmentTransaction) {
		// When the given tab is selected, switch to the corresponding page in
		// the ViewPager.
		mViewPager.setCurrentItem(tab.getPosition());
	}

	@Override
	public void onTabUnselected(ActionBar.Tab tab, FragmentTransaction fragmentTransaction) {}

	@Override
	public void onTabReselected(ActionBar.Tab tab, FragmentTransaction fragmentTransaction) {}
	
    // on start_button click
	public void StartBenchmark(View view) {
    	int supportedGLversion = deviceInfo.GetSupportedGLVersion();
    	if (supportedGLversion < 0x30000) {
    		Toast.makeText(getApplicationContext(), "Unsupported GL ES version: " +
    		    DeviceInfo.GlVersionToString(supportedGLversion) + " (req. 3.0)", Toast.LENGTH_LONG).show();
    		return;
    	} else {
    		Intent intent = new Intent(this, BenchmarkActivity.class);
        	startActivityForResult(intent, BENCH_REQUEST_ID); // startActivity(intent);    		
    	}
    }
	
	// http://developer.android.com/reference/android/os/AsyncTask.html
	private class UploadTask extends AsyncTask<JSONObject, Void, String> {
	     protected String doInBackground(JSONObject... json) {
	    	try {	
				HttpPost request = new HttpPost(SERVER_URL);

				request.setHeader("Content-Type", "application/json");
				request.setEntity(new StringEntity(json[0].toString()));

				HttpResponse response = (new DefaultHttpClient()).execute(request);
				
				return EntityUtils.toString(response.getEntity());
				
	    	} catch (Exception e) {
			    e.printStackTrace();
			    return "";
			}    	 
	     }
	     //protected void onProgressUpdate(Integer... progress) {}

	    protected void onPostExecute(String result) {
            if (result.equals("received with thanks")) {
            	Toast.makeText(getApplicationContext(), "Uploaded successfully, thank you!", Toast.LENGTH_LONG).show();
            }
            else { 
            	Toast.makeText(getApplicationContext(), "Upload to server failed, please try again.", Toast.LENGTH_LONG).show();
            	findViewById(R.id.upload_button).setEnabled(true);
            }	    	
	    }
	 }
	
	// on upload_button click
	public void Upload(View view) {
		AlertDialog.Builder alert = new AlertDialog.Builder(this);

		alert.setTitle("Upload results to the server");
		alert.setMessage("Please enter your nickname:");

		// Set an EditText view to get user input 
		final EditText input = new EditText(this);
		input.setHint("nickname");
		input.setText(lastNickname);
		
		//((InputMethodManager)getSystemService(Context.INPUT_METHOD_SERVICE))
        //.showSoftInput(input, InputMethodManager.SHOW_FORCED);
		
		alert.setView(input);

		alert.setPositiveButton("Upload", new DialogInterface.OnClickListener() {
		public void onClick(DialogInterface dialog, int whichButton) {
		  // Do something with value!
        	findViewById(R.id.upload_button).setEnabled(false);			
			lastNickname = input.getText().toString();
			try {
				JSONObject benchInfo = lastResult.getJSONObject("BenchInfo");
				JSONObject glInfo    = lastResult.getJSONObject("GLInfo");				
				JSONObject devInfo   = deviceInfo.GetJSON();
				
				JSONObject message = new JSONObject();
				message.put("Uploader", lastNickname);
				message.put("BenchInfo", benchInfo);
				message.put("GLInfo", glInfo);
				message.put("DeviceInfo", devInfo);
				
				//message.put("score", lastResult.getJSONObject("BenchInfo").getString("score"));
				//message.put("info", "I9505 " + lastNickname);
				
				new UploadTask().execute(message);
			} catch (JSONException e) {
				e.printStackTrace();
            	findViewById(R.id.upload_button).setEnabled(true);
			}
		  }
		});

		alert.setNegativeButton("Cancel", new DialogInterface.OnClickListener() {
		  public void onClick(DialogInterface dialog, int whichButton) {
		    // Canceled.
			  lastNickname = input.getText().toString();
		  }
		});

		alert.show();
	}
	
	@Override
	// http://stackoverflow.com/questions/5944987/popupwindow-in-android
	// http://stackoverflow.com/questions/18867029/how-to-show-pop-up-window-in-android
	public boolean onOptionsItemSelected(MenuItem item) {
	    switch (item.getItemId()) {
	        case R.id.action_about:
	        	final TextView tvInfo = new TextView(this);	    		
	        	tvInfo.setText(Html.fromHtml("<br/>&nbsp OpenGL ES 3.0 Benchmark for Android<br/>" + 
	        								 "&nbsp Author: Tomáš Kimer (FIT BUT)<br/>" +
	        								 "&nbsp Brno, Czech Republic, 2014<br/>" +
	        								 "&nbsp <a href=\"http://gles3mark.eu\">www.gles3mark.eu</a><br/>"));
	    		tvInfo.setMovementMethod(LinkMovementMethod.getInstance());
	        	
	        	AlertDialog.Builder alert = new AlertDialog.Builder(this);
	    		alert.setTitle("About gles3mark");
	    		alert.setView(tvInfo);
	    		alert.setPositiveButton("OK", null);	        	
	    		alert.show();
	        	
	        	return true;
	        case R.id.action_settings:
	            return true;
	        default:
	            return super.onOptionsItemSelected(item);
	    }
	}
	
	@Override
	protected void onActivityResult(int requestCode, int resultCode, Intent data) {
		if (requestCode == BENCH_REQUEST_ID) {
			if (resultCode == RESULT_OK) {  // -1,  0 = RESULT_CANCELED
				String resultStr = data.getStringExtra("Result"); 				
				JSONObject jsonObject = null;				
				try {
					jsonObject = new JSONObject(resultStr);
					JSONObject benchInfo = jsonObject.getJSONObject("BenchInfo");
					JSONObject glInfo = jsonObject.getJSONObject("GLInfo"); 
					
					Toast.makeText(getApplicationContext(), "Benchmark finished. Score: " + benchInfo.getString("score"),
							       Toast.LENGTH_SHORT).show();					
					
					TestSectionFragment f = (TestSectionFragment)mSectionsPagerAdapter.getActiveFragment(mViewPager, 0);
					if (f != null) // && f.isResumed() -- Interact with any views/data that must be alive
						f.SetLabels(benchInfo);
					// else  Flag something for update later, when this viewPagerFragment returns to onResume
					
					DeviceinfoSectionFragment d = (DeviceinfoSectionFragment)mSectionsPagerAdapter.getActiveFragment(mViewPager, 1);
					if (d != null)
						d.SetLabels(glInfo);
					
					lastResult = jsonObject;
					
					Button btnUpload = (Button)findViewById(R.id.upload_button);
					btnUpload.setEnabled(true);
					
				} catch (JSONException e) {
					e.printStackTrace();
				}
			}
			else {
				Toast.makeText(getApplicationContext(), "Benchmark aborted", Toast.LENGTH_SHORT).show();
			}
		}    	
		super.onActivityResult(requestCode, resultCode, data);
	}
	
	/**
	 * A {@link FragmentPagerAdapter} that returns a fragment corresponding to
	 * one of the sections/tabs/pages.
	 */
	public class SectionsPagerAdapter extends FragmentPagerAdapter {
		final FragmentManager fm;
		
		public SectionsPagerAdapter(FragmentManager fm) {
			super(fm);
			this.fm = fm;
		}
		
		// http://stackoverflow.com/questions/8785221/retrieve-a-fragment-from-a-viewpager/14530629#14530629
		public Fragment getActiveFragment(ViewPager container, int position) {
			return fm.findFragmentByTag(makeFragmentName(container.getId(), position));
		}

		private String makeFragmentName(int viewId, int index) {
			return "android:switcher:" + viewId + ":" + index;
		}
		
		@Override
		// called only when each Fragment needs to be created the first time
		public Fragment getItem(int position) {
			// getItem is called to instantiate the fragment for the given page.
			// Return a DummySectionFragment (defined as a static inner class
			// below) with the page number as its lone argument.
			Fragment fragment = null;
			Bundle args = new Bundle();
			
			switch (position) {
			case 0:
				fragment = new TestSectionFragment();
				break;
			case 1:
				fragment = new DeviceinfoSectionFragment();
				args.putString(DeviceinfoSectionFragment.ARG_DEVICE_INFO, deviceInfo.BuildJSON().toString());
				break;
			case 2:
				fragment = new RankingSectionFragment();
				break;
			}

			args.putInt(TestSectionFragment.ARG_SECTION_NUMBER, position);
			fragment.setArguments(args);
			return fragment;
		}

		@Override
		public int getCount() {
			return 3; // Show 3 total pages.
		}

		@Override
		public CharSequence getPageTitle(int position) {
			Locale l = Locale.getDefault();
			switch (position) {
				case 0: return getString(R.string.title_section_test).toUpperCase(l);
				case 1: return getString(R.string.title_section_deviceinfo).toUpperCase(l);
				case 2:	return getString(R.string.title_section_ranking).toUpperCase(l);
			}
			return null;
		}
	}


	/**
	 * A dummy fragment representing a section of the app, but that simply
	 * displays dummy text.
	 */
	public static class TestSectionFragment extends Fragment {
		/**
		 * The fragment argument representing the section number for this
		 * fragment.
		 */
		public static final String ARG_SECTION_NUMBER = "section_number";

		public TestSectionFragment() {}
		
		private TextView scoreTextView;
		private TextView tvFPSavg, tvFPSstddev, tvFPSbest, tvFPSworst;
		private TextView tvSPFavg, tvSPFstddev, tvSPFbest, tvSPFworst;
		
		public void SetLabels(JSONObject json) {
			try {
				scoreTextView.setText(json.getString("score"));
				
				tvFPSavg   .setText(json.getString("FPSavg"   ));
				tvFPSstddev.setText(json.getString("FPSstddev"));
				tvFPSbest  .setText(json.getString("FPSbest"  ));
				tvFPSworst .setText(json.getString("FPSworst" ));
				
				tvSPFavg   .setText(json.getString("SPFavg"   ) + " ms");
				tvSPFstddev.setText(json.getString("SPFstddev") + " ms");
				tvSPFbest  .setText(json.getString("SPFbest"  ) + " ms");
				tvSPFworst .setText(json.getString("SPFworst" ) + " ms");
			
			} catch (JSONException e) {
				e.printStackTrace();
			}
		}

		@Override
		public void onCreate(Bundle savedInstanceState) {
			super.onCreate(savedInstanceState);
			// http://stackoverflow.com/questions/20550016/savedinstancestate-is-always-null-in-fragment
			//setRetainInstance(true); // ???
		}

		@Override
		public void onSaveInstanceState(Bundle outState) {
			super.onSaveInstanceState(outState);
		}

		@Override
		public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {
			View rootView = inflater.inflate(R.layout.fragment_main_test, container, false);
			
			scoreTextView = (TextView)rootView.findViewById(R.id.score_label);
			
			tvFPSavg    = (TextView)rootView.findViewById(R.id.tvFPSavg);
			tvFPSstddev = (TextView)rootView.findViewById(R.id.tvFPSstddev);
			tvFPSbest   = (TextView)rootView.findViewById(R.id.tvFPSbest);
			tvFPSworst  = (TextView)rootView.findViewById(R.id.tvFPSworst);
			
			tvSPFavg    = (TextView)rootView.findViewById(R.id.tvSPFavg);
			tvSPFstddev = (TextView)rootView.findViewById(R.id.tvSPFstddev);
			tvSPFbest   = (TextView)rootView.findViewById(R.id.tvSPFbest);
			tvSPFworst  = (TextView)rootView.findViewById(R.id.tvSPFworst);
			
			//String lastResult = ((MainActivity)getActivity()).lastResult;
			//String score = lastResult == null ? "no score" : lastResult;

			return rootView;
		}

		@Override
		public void onViewCreated(View view, Bundle savedInstanceState) {
			super.onViewCreated(view, savedInstanceState);
		}
	}
	
	@SuppressLint("SetJavaScriptEnabled")
	public static class RankingSectionFragment extends Fragment {
		public static final String ARG_SECTION_NUMBER = "section_number";

		public RankingSectionFragment() {}
		
		private class MyWebViewClient extends WebViewClient {
		    @Override
		    public boolean shouldOverrideUrlLoading(WebView view, String url) {
		        // do not override; let my WebView load the page
		    	if (Uri.parse(url).getHost().equals(Uri.parse(SERVER_URL).getHost())) {
		        	//view.getSettings().setUserAgentString(USER_AGENT);
		        	return false;
		        }
		        // link is not for a page on my site, so launch another Activity that handles URLs
		        Intent intent = new Intent(Intent.ACTION_VIEW, Uri.parse(url));
		        startActivity(intent);
		        return true;
		    }
		}
		
		@Override
		public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {
			View rootView = inflater.inflate(R.layout.fragment_main_ranking, container, false);

			WebView rankingWebView = (WebView)rootView.findViewById(R.id.webViewRanking);
			
			rankingWebView.getSettings().setUserAgentString(USER_AGENT);
			rankingWebView.getSettings().setJavaScriptEnabled(true);			
			rankingWebView.setWebViewClient(new MyWebViewClient());  // enable follow links
			rankingWebView.loadUrl(SERVER_URL);			
			
	        return rootView;
		}
	}
	
	public static class DeviceinfoSectionFragment extends Fragment {
		public static final String ARG_SECTION_NUMBER = "section_number";
		public static final String ARG_DEVICE_INFO = "device_info";

		public DeviceinfoSectionFragment() { }
		
		private TextView deviceinfoTextView;
		
		public void SetLabels(JSONObject json) {
			GLInfo glInfo = new GLInfo(json);
			deviceinfoTextView.setText("--- GL Info ---\n" + glInfo.toString() + '\n' + getArguments().getString(ARG_DEVICE_INFO) +
										"\n--- GL Extensions ---\n" + glInfo.Extensions());
		}

		@Override
		public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {
			View rootView = inflater.inflate(R.layout.fragment_main_deviceinfo, container, false);
			
			deviceinfoTextView = (TextView)rootView.findViewById(R.id.deviceinfo_label);
			deviceinfoTextView.setMovementMethod(new ScrollingMovementMethod());
			deviceinfoTextView.setText("--- GL Info ---\n<please run benchmark first>\n\n" +
										getArguments().getString(ARG_DEVICE_INFO));// Integer.toString(getArguments().getInt(ARG_SECTION_NUMBER))
			
			return rootView;
		}
	}
}
