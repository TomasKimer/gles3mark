
precision mediump float;

in vec2 coord;
in vec3 normalVS;

uniform sampler2D tex;
uniform vec4 diffuseColor;
uniform bool hasTexture;

layout (location=0) out vec4 fragColor;
layout (location=1) out vec3 fragNormal;

void main() {
	vec3 albedo = diffuseColor.rgb;    
    
    if (hasTexture)
		albedo = texture(tex, coord).rgb;
	
	fragNormal = normalize(normalVS);
    fragColor = vec4(albedo, 1.0);
}