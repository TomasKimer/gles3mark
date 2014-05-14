package com.tomaskimer.gles3mark;

import java.util.Iterator;

import org.json.JSONException;
import org.json.JSONObject;

public abstract class BaseInfo {
	
	protected JSONObject json;
	
    public JSONObject GetJSON() {
    	return json;
    }
    
    protected static String formatLine(JSONObject j, String key) throws JSONException {
    	return key + ": " +	j.getString(key) + '\n';    	
    }
    
    protected static String jsonToString(JSONObject json) {
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
}
