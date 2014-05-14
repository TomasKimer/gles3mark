package com.tomaskimer.gles3mark;

import org.json.JSONException;
import org.json.JSONObject;

public class GLInfo extends BaseInfo {
	
	JSONObject json;
	
	public GLInfo(JSONObject json) {
		this.json = json;
	}
	
	public String toString() {
		String result = new String();
		
		try {
			result += formatLine(json, "Vendor"      );
			result += formatLine(json, "Renderer"    );
			result += formatLine(json, "Version"     );
			result += formatLine(json, "GLSL version");
			
			result += formatLine(json, "Max render buffer size");
			result += formatLine(json, "Max texture size"      );
			result += formatLine(json, "Max viewport dims"     );
			result += formatLine(json, "Max samples"           );
			result += formatLine(json, "Max color attachments" );
			result += formatLine(json, "Max vertex attributes" );						
			
		} catch (JSONException e) {
			e.printStackTrace();
		}
		
		return result;
	}
	
	public String Extensions() {
		try {
			return json.getString("Extensions");
		} catch (JSONException e) {
			e.printStackTrace();
			return "";
		}
	}

}
