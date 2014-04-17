
precision mediump float;

in vec2 coord;
in vec3 normal;

uniform sampler2D tex;
uniform vec4 diffuseColor;

layout (location=0) out vec4 fragColor;

void main() {
	fragColor = texture(tex, coord) * diffuseColor; // vec4(1,0,0,1);   vec4(normal, 1) * 
}