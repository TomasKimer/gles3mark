package com.tomaskimer.gles3mark;

import java.util.ArrayList;
import java.util.Locale;

import android.app.ActionBar;
import android.content.Context;
import android.content.Intent;
import android.os.Bundle;
import android.support.v4.app.Fragment;
import android.app.FragmentTransaction; // support.v4 ?
import android.support.v4.app.FragmentActivity;
import android.support.v4.app.FragmentManager;
import android.support.v4.app.FragmentPagerAdapter;
import android.support.v4.app.ListFragment;
import android.support.v4.view.ViewPager;
import android.text.method.ScrollingMovementMethod;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.Menu;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ArrayAdapter;
import android.widget.ListView;
import android.widget.TextView;
import android.widget.Toast;

import com.tomaskimer.gles3mark.DeviceInfo;

public class MainActivity extends FragmentActivity implements ActionBar.TabListener {

	final static int BENCH_REQUEST_ID = 1234;
	public DeviceInfo deviceInfo = new DeviceInfo(this);
	public String lastResult = null; 
	
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
	public void onTabUnselected(ActionBar.Tab tab, FragmentTransaction fragmentTransaction) {
	}

	@Override
	public void onTabReselected(ActionBar.Tab tab, FragmentTransaction fragmentTransaction) {
	}
	
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
	
	@Override
	protected void onActivityResult(int requestCode, int resultCode, Intent data) {
		//Log.e("onActivityResult", String.format("%d, %d", requestCode, resultCode));
		if (requestCode == BENCH_REQUEST_ID) {
			if (resultCode == RESULT_OK) {  // -1,  0 = RESULT_CANCELED
				String myValue = data.getStringExtra("XML Result"); 
				Toast.makeText(getApplicationContext(), "Benchmark finished. Score: " + myValue, Toast.LENGTH_SHORT).show();
				//Log.d("BenchmarkActivityResult", "Score: " + myValue);
				//textView1.setText(textView1.getText() + "\nBenchmark score: " + myValue);
				TestSectionFragment f = (TestSectionFragment)mSectionsPagerAdapter.getActiveFragment(mViewPager, 0);
				if (f != null) //  && f.isResumed() -- Interact with any views/data that must be alive
					f.SetScoreLabel(myValue); // else  Flag something for update later, when this viewPagerFragment returns to onResume
				lastResult = myValue;
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
				fragment = new RankingSectionFragment();
				break;
			case 2:
				fragment = new DeviceinfoSectionFragment();
				args.putString(DeviceinfoSectionFragment.ARG_DEVICE_INFO, deviceInfo.GetDeviceInfo());
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
				case 0: return getString(R.string.title_section1).toUpperCase(l);
				case 1: return getString(R.string.title_section2).toUpperCase(l);
				case 2:	return getString(R.string.title_section3).toUpperCase(l);
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

		public TestSectionFragment() { Log.d("FRAGMENT", "Test constructor");}
		
		TextView scoreTextView;
		public void SetScoreLabel(String score) {
			scoreTextView.setText(score);
		}

		@Override
		public void onCreate(Bundle savedInstanceState) {
			super.onCreate(savedInstanceState);
			//setRetainInstance(true); // ???
		}

		@Override
		public void onSaveInstanceState(Bundle outState) {
			super.onSaveInstanceState(outState);
			Log.d("FRAGMENT", "save instance state");
		}

		@Override
		public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {
			Log.d("FRAGMENT", "Test onCreateView " + getTag());
			
			View rootView = inflater.inflate(R.layout.fragment_main_test, container, false);
			scoreTextView = (TextView)rootView.findViewById(R.id.score_label);
			
			//String lastResult = ((MainActivity)getActivity()).lastResult;
			//String score = lastResult == null ? "no score" : lastResult;
			
			scoreTextView.setText("no score");  //Integer.toString(getArguments().getInt(ARG_SECTION_NUMBER))
			return rootView;
		}

		@Override
		public void onViewCreated(View view, Bundle savedInstanceState) {
			// TODO Auto-generated method stub
			super.onViewCreated(view, savedInstanceState);
			Log.d("FRAGMENT", "onViewCreated");
		}
	}
	
	public static class RankingSectionFragment extends ListFragment {  // TODO ListFragment
		public static final String ARG_SECTION_NUMBER = "section_number";

		public RankingSectionFragment() {}
		
		public class RankItem {
		    public String device, score;
		    public RankItem(String device, String score) {
		    	this.device = device; this.score = score;
		    }
		}
		
		public class RanksArrayAdapter extends TwoLineArrayAdapter<RankItem> {
		    public RanksArrayAdapter(Context context, RankItem[] ranks) {
		        super(context, ranks);
		    }
		    @Override
		    public String lineOneText(RankItem e) { return e.device; }
		    @Override
		    public String lineTwoText(RankItem e) { return e.score ; }
		}

		@Override
		public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {
			View rootView = inflater.inflate(R.layout.fragment_main_ranking, container, false); // for custom listview (defined in rankingfragment.xml)
			/////ListView rankingTextView = (ListView) rootView.findViewById(R.id.ranking_listView);
			
			/** Creating an array adapter to store String[] **/
	        //ArrayAdapter<String> adapter = new ArrayAdapter<String>(inflater.getContext(), android.R.layout.simple_list_item_1, strings);
			
	        RankItem[] ranks = new RankItem[] {
	        		new RankItem("1. Sony Xperia Z1", "986"),
	        		new RankItem("2. Google Nexus 5", "856"),
	        		new RankItem("3. Samsung Galaxy S4 I9505", "750"),
	        		new RankItem("4. <placeholder>", "732"),
	        		new RankItem("5. <placeholder>", "698"),
	        		new RankItem("6. <placeholder>", "542"),
	        		new RankItem("7. <placeholder>", "421"),
	        		new RankItem("8. <placeholder>", "345"),
	        		new RankItem("9. <placeholder>", "145"),
	        		new RankItem("10. <placeholder>", "145"),
	        		new RankItem("11. <placeholder>", "145"),
	        		new RankItem("12. <placeholder>", "145"),
	        };
	        
	        
	        /** Setting the list adapter for the ListFragment */
	        setListAdapter(new RanksArrayAdapter(inflater.getContext(), ranks)); //adapter.notifyDataSetChanged();
			
	        return rootView; // super.onCreateView(inflater, container, savedInstanceState);
		}
	}
	
	public static class DeviceinfoSectionFragment extends Fragment {
		public static final String ARG_SECTION_NUMBER = "section_number";
		public static final String ARG_DEVICE_INFO = "device_info";

		public DeviceinfoSectionFragment() { }

		@Override
		public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {
			View rootView = inflater.inflate(R.layout.fragment_main_deviceinfo, container, false);
			TextView deviceinfoTextView = (TextView)rootView.findViewById(R.id.deviceinfo_label);
			deviceinfoTextView.setMovementMethod(new ScrollingMovementMethod());
			deviceinfoTextView.setText(getArguments().getString(ARG_DEVICE_INFO));// Integer.toString(getArguments().getInt(ARG_SECTION_NUMBER))
			return rootView;
		}
	}
}
