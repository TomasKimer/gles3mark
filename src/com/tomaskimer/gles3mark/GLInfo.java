package com.tomaskimer.gles3mark;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

/**
 * Formats OpenGL and EGL info from JSON to readable text.
 *  
 * @author Tomas Kimer
 */
public class GLInfo extends BaseInfo {
	
	JSONObject json, jsonEGL;
	
	public GLInfo(JSONObject json, JSONObject jsonEGL) {
		this.json = json;
		this.jsonEGL = jsonEGL;		
	}
	
	public String toString() {
		String result = new String();
		
		try {
			result += "--- GL info ---\n";
			
			result += formatLine(json, "Vendor"      );
			result += formatLine(json, "Renderer"    );
			result += formatLine(json, "Version"     );
			result += formatLine(json, "GLSL version");
			
			result += "\n--- EGL context info ---\n";
			
			result += formatLine(jsonEGL, "Version"   );
			result += formatLine(jsonEGL, "Surface size"   );
			result += formatLine(jsonEGL, "Min swap interval");
			result += formatLine(jsonEGL, "Max swap interval");					
			
		} catch (JSONException e) {
			e.printStackTrace();
		}
		
		return result;
	}
	
	public String Limits() {
		String ret = "--- GL implementation-dependent limits ---\n";		
		try {
			ret += jsonToString(json.getJSONObject("Limits"));
		} catch (JSONException e) {
			e.printStackTrace();
		}
		
		return ret;
	}
	
	public String Extensions() {
		String ret = "--- GL extensions ---\n";
		try {
			JSONArray exts = json.getJSONArray("Extensions");
			for (int i = 0; i < exts.length(); ++i) {
				ret += exts.getString(i) + "\n";
			}
			//return json.getString("Extensions").replaceAll(" ", "\n");
		} catch (JSONException e) {
			e.printStackTrace();
		}
		return ret;
	}

}
