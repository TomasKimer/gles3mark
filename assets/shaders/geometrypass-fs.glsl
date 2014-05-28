/**
  *  gles3mark v1.0
  *
  *  \date   2014-05-28
  *  \author Tomas Kimer <xkimer00@stud.fit.vutbr.cz>
  */

precision highp float;  // mediump

in vec2 coord;
in vec3 normalVS;

uniform sampler2D tex;
uniform vec4 diffuseColor;
uniform bool hasTexture;

//const float ambientContribution = 0.1;  //0.2

layout (location=0) out vec4 fragColor;
layout (location=1) out vec3 fragNormal;
//layout (location=2) out vec4 fragAmbientContrib;

void main() {
	vec3 albedo = diffuseColor.rgb;    
    
    if (hasTexture)
		albedo = texture(tex, coord).rgb;
	
	fragNormal = normalize(normalVS);
    fragColor = vec4(albedo, 1.0);
//    fragAmbientContrib = vec4(albedo * ambientContribution, 1.0);
}