package com.tomaskimer.gles3mark;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.io.IOException;
import java.util.Iterator;

import org.json.JSONException;
import org.json.JSONObject;

import android.app.Activity;
import android.app.ActivityManager;
import android.app.ActivityManager.MemoryInfo;
import android.content.Context;
import android.content.pm.ConfigurationInfo;
//import android.telephony.TelephonyManager;
import android.util.DisplayMetrics;
import android.util.Log;

public class DeviceInfo {
	
	final Activity activity;
	JSONObject json;
	
	public DeviceInfo(Activity parent) {
		activity = parent;		
	}
    
	public int GetSupportedGLVersion() {
    	final ActivityManager activityManager = (ActivityManager)activity.getSystemService(Context.ACTIVITY_SERVICE);
    	final ConfigurationInfo configurationInfo = activityManager.getDeviceConfigurationInfo();
    	return configurationInfo.reqGlEsVersion;    	
    }
    
    public static String GlVersionToString(int version) {
    	return String.format("%x.%x", version >> 16, version & 0x00FF);    	
    }
    
    public JSONObject GetJSON() {
    	return json;
    }
    
    public DeviceInfo BuildJSON() {
    	json = new JSONObject();
    	
    	try {
    		// http://developer.android.com/reference/android/os/Build.html
    		JSONObject jsonHW = new JSONObject();
    		
    		jsonHW.put("Manufacturer", android.os.Build.MANUFACTURER);
			jsonHW.put("Brand", android.os.Build.BRAND);
			jsonHW.put("Model", android.os.Build.MODEL);
			jsonHW.put("Device", android.os.Build.DEVICE);
			jsonHW.put("Product", android.os.Build.PRODUCT);
			jsonHW.put("Display", android.os.Build.DISPLAY);
			jsonHW.put("Board", android.os.Build.BOARD);
			jsonHW.put("Fingerprint", android.os.Build.FINGERPRINT);
			jsonHW.put("Hardware", android.os.Build.HARDWARE);
			jsonHW.put("Id", android.os.Build.ID);
			jsonHW.put("Serial", android.os.Build.SERIAL);
			jsonHW.put("Tags", android.os.Build.TAGS);
			jsonHW.put("Type", android.os.Build.TYPE);
			
			json.put("HW", jsonHW);
			
			
			// http://developer.android.com/reference/java/lang/System.html#getProperty%28java.lang.String%29
			JSONObject jsonOS = new JSONObject();
			
			jsonOS.put("Android SDK version", android.os.Build.VERSION.SDK_INT);
			jsonOS.put("Android version", android.os.Build.VERSION.RELEASE);
			jsonOS.put("Kernel", getInfo("/proc/version"));			
			jsonOS.put("OS", System.getProperty("os.name"   ) + " " +
					       System.getProperty("os.version") + " " +
					       System.getProperty("os.arch"   ));
			jsonOS.put("VM implementation", System.getProperty("java.vm.name") + " " +
					   				      System.getProperty("java.vm.version"));
			jsonOS.put("VM specification", System.getProperty("java.vm.specification.name") + " " +
					   				     System.getProperty("java.vm.specification.version"));
			jsonOS.put("Bootloader", android.os.Build.BOOTLOADER);			
			
			json.put("OS", jsonOS);
			
			
	    	// --- Display ---
			JSONObject jsonDisplay = new JSONObject();
			
			DisplayMetrics metrics = activity.getResources().getDisplayMetrics();			
			jsonDisplay.put("Resolution", metrics.heightPixels + "x" + metrics.widthPixels);
			jsonDisplay.put("DPI", metrics.densityDpi);			
			double x = Math.pow(metrics.widthPixels  / metrics.xdpi, 2);  // TODO glcontext? smaller on tegra - bar?
	        double y = Math.pow(metrics.heightPixels / metrics.ydpi, 2);			
			jsonDisplay.put("Size", Math.round(Math.sqrt(x + y)) + "inch");			
			jsonDisplay.put("OpenGL ES version", GlVersionToString(GetSupportedGLVersion()));
			
			json.put("Display", jsonDisplay);
			
			
	    	// --- CPU ---
	        // TODO Arch (Krait), Model (Qualcomm Snapdragon APQ8064T), bogoMIPS? (/proc/cpuinfo),
			JSONObject jsonCPU = new JSONObject();
			
			jsonCPU.put("CPU cores", Runtime.getRuntime().availableProcessors());			
			int cpuMinFreqHz = Integer.parseInt(getInfo("/sys/devices/system/cpu/cpu0/cpufreq/cpuinfo_min_freq")); // scaling_cur_freq
	    	int cpuMaxFreqHz = Integer.parseInt(getInfo("/sys/devices/system/cpu/cpu0/cpufreq/cpuinfo_max_freq"));			
			jsonCPU.put("CPU freq", cpuMinFreqHz / 1000 + " ~ " +  
	                                cpuMaxFreqHz / 1000 + " MHz");
			jsonCPU.put("CPU ABI", android.os.Build.CPU_ABI);
			jsonCPU.put("CPU ABI2", android.os.Build.CPU_ABI2);
			
			json.put("CPU", jsonCPU);
			
			
	    	// --- MEM ---
	    	// TODO /proc/meminfo - free etc?
			JSONObject jsonMem = new JSONObject();
			
			ActivityManager activityManager = (ActivityManager)activity.getApplicationContext().getSystemService(Context.ACTIVITY_SERVICE);
	    	MemoryInfo memoryInfo = new ActivityManager.MemoryInfo();
	    	activityManager.getMemoryInfo(memoryInfo);	    	
			jsonMem.put("Total memory", (float)memoryInfo.totalMem / (1 << 20) + " MB");
			
			json.put("Mem", jsonMem);
			
			// --- Telephony ---
			//TelephonyManager telephonyManager = (TelephonyManager)activity.getSystemService(Context.TELEPHONY_SERVICE);
			//json.put("IMEI", telephonyManager.getDeviceId());
			//json.put("IMSI", telephonyManager.getSubscriberId());
			
		} catch (JSONException e) {
			e.printStackTrace();
		}
    	
    	return this;
    }
    
    public String toString() {
    	String result = new String();
    	
    	try {
    		result += "HW info: \n";
    		result += jsonToString((JSONObject)json.get("HW"));
    		result += "\nOS info: \n";
    		result += jsonToString((JSONObject)json.get("OS"));
    		result += "\nDisplay info: \n";
    		result += jsonToString((JSONObject)json.get("Display"));
    		result += "\nCPU info: \n";
    		result += jsonToString((JSONObject)json.get("CPU"));
    		result += "\nMemory info: \n";
    		result += jsonToString((JSONObject)json.get("Mem"));
    		
		} catch (JSONException e) {
			e.printStackTrace();
		}
    	
    	return result;
    }
    
    private static String jsonToString(JSONObject json) {
    	String result = new String();
    	
    	Iterator<?> keys = json.keys();
    	while (keys.hasNext()) {
            String key = (String)keys.next();
            try {
				result += '\t' + key + ": " + json.get(key) + '\n';
			} catch (JSONException e) {
				e.printStackTrace();
			}
        }
    	return result;    	
    }

    private static String getInfo(final String filename) {
    	StringBuffer sb = new StringBuffer();
    	try {
    		BufferedReader br = new BufferedReader(new FileReader(new File(filename)));
    		String line;
    		while ((line = br.readLine()) != null) {
    			sb.append(line);
    		}
   			br.close();
    	} catch (IOException e) {
    		Log.e("GetInfo " + filename, e.getMessage());
    	} 
    	return sb.toString();
    }
}
