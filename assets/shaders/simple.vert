

layout (location=0) in vec3 vertexPosition;
layout (location=1) in vec3 vertexNormal;
layout (location=2) in vec2 vertexTextureCoord;

uniform mat4 mvp;

out vec2 coord;
out vec3 normal;

void main() {
	gl_Position = mvp*vec4(vertexPosition,1);
	coord = vertexTextureCoord;
	normal = vertexNormal;
}