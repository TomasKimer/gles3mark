/**
  *  gles3mark v1.0
  *
  *  \date   2014-05-28
  *  \author Tomas Kimer <xkimer00@stud.fit.vutbr.cz>
  */

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

out vec3 normalVS;
out vec2 coord;

void main() {
	mat4 modelMat = model;
    
    if (isInstanced) {
        modelMat = mat4(instanceModelMat0, 
                        instanceModelMat1,
                        instanceModelMat2,
                        instanceModelMat3) * model;
    }
    	
	normalVS = mat3(view) * mat3(modelMat) * vertexNormal;  // view * model * vec4(vertexNormal, 0.0)).xyz
    coord = vertexTextureCoord;
    
    gl_Position = projection * view * modelMat * vec4(vertexPosition, 1);
}