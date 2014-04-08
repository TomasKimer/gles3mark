package com.tomaskimer.gles3mark;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.io.IOException;

import android.app.Activity;
import android.app.ActivityManager;
import android.app.ActivityManager.MemoryInfo;
import android.content.Context;
import android.content.pm.ConfigurationInfo;
import android.telephony.TelephonyManager;
import android.util.DisplayMetrics;
import android.util.Log;

public class DeviceInfo {
	
	final Activity activity;
	
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


    public String GetDeviceInfo() {
    	String result = new String();
    	
    	// http://developer.android.com/reference/android/os/Build.html
    	result =
    			"Manufacturer: " + android.os.Build.MANUFACTURER + '\n' +
    			"Brand: "        + android.os.Build.BRAND 		 + '\n' +
    			"Model: " 		 + android.os.Build.MODEL 		 + '\n' +
    			"Device: " 	  	 + android.os.Build.DEVICE 		 + '\n' +
    			"Product: " 	 + android.os.Build.PRODUCT 	 + '\n' +
    			"Display: " 	 + android.os.Build.DISPLAY 	 + '\n' +
    			"Board: " 	 + android.os.Build.BOARD 	 + '\n' +
    			"Bootloader: " 	 + android.os.Build.BOOTLOADER 	 + '\n' +
    			"CPU ABI: " 	 + android.os.Build.CPU_ABI 	 + '\n' +
    			"CPU ABI2: " 	 + android.os.Build.CPU_ABI2 	 + '\n' +
    			"Fingerprint: " 	 + android.os.Build.FINGERPRINT 	 + '\n' +
    			"Hardware: " 	 + android.os.Build.HARDWARE 	 + '\n' +
    			"Id: " 	 + android.os.Build.ID 	 + '\n' +
    			"Serial: " 	 + android.os.Build.SERIAL 	 + '\n' +
    			"Tags: " 	 + android.os.Build.TAGS 	 + '\n' +
    			"Type: " 	 + android.os.Build.TYPE  	 + '\n' +
    			"Android SDK version: " 	 + android.os.Build.VERSION.SDK_INT  	 + '\n' +
    			"Android version: " 	 + android.os.Build.VERSION.RELEASE;
    	
		result += "\nKernel: "   + getInfo("/proc/version");
    	
    	// http://developer.android.com/reference/java/lang/System.html#getProperty%28java.lang.String%29
    	result += "\nOS: " + System.getProperty("os.name"   ) + " "
    					   + System.getProperty("os.version") + " "
    					   + System.getProperty("os.arch"   ) + '\n' + 
    	"VM implementation: " + System.getProperty("java.vm.name") + " "
    											   + System.getProperty("java.vm.version") + '\n' +
    	"VM specification: " + System.getProperty("java.vm.specification.name") + " "
    											  + System.getProperty("java.vm.specification.version") + '\n' +
        "OpenGL ES version: " + GlVersionToString(GetSupportedGLVersion());
    	
    	// --- Display ---
    	DisplayMetrics metrics = activity.getResources().getDisplayMetrics();
    	result += "\nResolution: " + metrics.heightPixels + "x" + metrics.widthPixels;
    	result += "\nDPI: " + metrics.densityDpi;
    	
        double x = Math.pow(metrics.widthPixels  / metrics.xdpi, 2);  // TODO glcontext? smaller on tegra - bar?
        double y = Math.pow(metrics.heightPixels / metrics.ydpi, 2);
        result += "\nSize: " + Math.round(Math.sqrt(x + y)) + "inch";
    	
    	// --- CPU ---
        // TODO Arch (Krait), Model (Qualcomm Snapdragon APQ8064T), bogoMIPS? (/proc/cpuinfo), 
    	result += "\nCPU cores: " + Runtime.getRuntime().availableProcessors();
    	int cpuMinFreqHz = Integer.parseInt(getInfo("/sys/devices/system/cpu/cpu0/cpufreq/cpuinfo_min_freq")); // scaling_cur_freq
    	int cpuMaxFreqHz = Integer.parseInt(getInfo("/sys/devices/system/cpu/cpu0/cpufreq/cpuinfo_max_freq"));
    	result += "\nCPU freq: " + cpuMinFreqHz / 1000 + " ~ "  
    			                 + cpuMaxFreqHz / 1000 + " MHz";

    	// --- MEM ---
    	// TODO /proc/meminfo - free etc?
    	ActivityManager activityManager = (ActivityManager)activity.getApplicationContext().getSystemService(Context.ACTIVITY_SERVICE);
    	MemoryInfo memoryInfo = new ActivityManager.MemoryInfo();
    	activityManager.getMemoryInfo(memoryInfo);
    	result += "\nTotal memory: " + (float)memoryInfo.totalMem / (1 << 20) + " MB";
    	
    	// --- Telephony ---
    	TelephonyManager telephonyManager = (TelephonyManager)activity.getSystemService(Context.TELEPHONY_SERVICE);
    	result += "\nIMEI: " + telephonyManager.getDeviceId();
    	result += "\nIMSI: " + telephonyManager.getSubscriberId();
    	
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
