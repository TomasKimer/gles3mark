
in vec2 coord;
uniform sampler2D tex;
out vec4 fragColor;

void main() {
	fragColor = texture(tex, coord);
}