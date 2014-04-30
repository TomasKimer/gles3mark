

layout (location=0) in vec3 vertexPosition;
layout (location=1) in vec3 vertexNormal;
layout (location=2) in vec2 vertexTextureCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec2 coord;
out vec3 normal;
out vec3 position;

void main() {
	mat4 inverseProjection = inverse(projection);
	
	gl_Position = projection * view * model * vec4(vertexPosition, 1);
	coord = vertexTextureCoord;
	normal = vertexNormal;
	position = vertexPosition;
}