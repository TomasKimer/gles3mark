

in vec3 position;
in vec2 tc;
uniform mat4 mvp;
out vec2 coord;

void main() {
	gl_Position = mvp*vec4(position,1);
	coord = tc;
}