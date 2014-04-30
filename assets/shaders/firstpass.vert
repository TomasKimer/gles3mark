

layout (location=0) in vec3 vertexPosition;
layout (location=1) in vec3 vertexNormal;
layout (location=2) in vec2 vertexTextureCoord;

layout (location=3) in vec4 instanceModelMat0;  // layout (location=3) in mat4 instanceModelMat; // does not link on adreno - "multiple attribute attempt to bind at same location"
layout (location=4) in vec4 instanceModelMat1;
layout (location=5) in vec4 instanceModelMat2;
layout (location=6) in vec4 instanceModelMat3;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform bool isInstanced;

out vec2 coord;
out vec3 normal;
out vec3 position;

void main() {
	mat4 inverseProjection = inverse(projection);	
	
	if (isInstanced) {
        mat4 instanceModelMat = mat4(instanceModelMat0, 
                                     instanceModelMat1,
                                     instanceModelMat2,
                                     instanceModelMat3);

		gl_Position = projection * view * instanceModelMat * model * vec4(vertexPosition, 1);
    }
	else
		gl_Position = projection * view                    * model * vec4(vertexPosition, 1);

	coord = vertexTextureCoord;
	normal = vertexNormal;
	position = vertexPosition;
}