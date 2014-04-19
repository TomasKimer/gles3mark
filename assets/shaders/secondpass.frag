
precision mediump float;

in vec2 coord;
in vec3 normal;

uniform sampler2D tex;
uniform vec4 diffuseColor;
uniform bool hasTexture;

layout (location=0) out vec4 fragColor;

void main() {
	if (hasTexture)
		fragColor = texture(tex, coord) * diffuseColor; // vec4(1,0,0,1);   vec4(normal, 1) * 
	else
		fragColor = diffuseColor;
}