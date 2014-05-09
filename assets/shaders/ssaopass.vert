

layout(location = 0) in vec2 in_ScreenCoord;
layout(location = 1) in vec2 in_TexCoord;

out vec2 texCoord;


//uniform mat4      invProj;
//out     vec3      viewRay;


void main()
{
	gl_Position = vec4(in_ScreenCoord, 0.0, 1.0);
	texCoord = in_TexCoord;   // or in_ScreenCoord * 0.5 + 0.5;


//    vec4 pos = invProj * gl_Position;
//    viewRay = vec3(pos.xy / pos.z, 1.0f);
}