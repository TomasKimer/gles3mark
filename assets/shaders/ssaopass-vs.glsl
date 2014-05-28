/**
  *  gles3mark v1.0
  *
  *  \date   2014-05-28
  *  \author Tomas Kimer <xkimer00@stud.fit.vutbr.cz>
  */

layout(location = 0) in vec2 in_ScreenCoord;
layout(location = 1) in vec2 in_TexCoord;

out vec2 texCoord;

void main()
{
	gl_Position = vec4(in_ScreenCoord, 0.0, 1.0);
	texCoord = in_TexCoord;   // or in_ScreenCoord * 0.5 + 0.5;
}