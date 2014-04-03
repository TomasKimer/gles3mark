

layout (location=0) in vec3 position;
layout (location=1) in vec2 tc;

uniform mat4 mvp;

out vec2 coord;

void main() {
	gl_Position = mvp*vec4(position,1);
	coord = tc;
}