
in vec2 coord;
in vec3 normal;

uniform sampler2D tex;

layout (location=0) out vec4 fragColor;

void main() {
	fragColor = vec4(normal, 1) * texture(tex, coord); // vec4(1,0,0,1);
}