
in vec2 coord;

uniform sampler2D tex;

layout (location=0) out vec4 fragColor;

void main() {
	fragColor = texture(tex, coord); //vec4(1,0,0,1); //
}