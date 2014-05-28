/**
  *  gles3mark v1.0
  *
  *  \date   2014-05-28
  *  \author Tomas Kimer <xkimer00@stud.fit.vutbr.cz>
  */

precision highp float;  // mediump

in vec2 fragTexCoord;

uniform sampler2D tex;

layout (location=0) out vec4 fragColor;

void main()
{
    fragColor = texture(tex, fragTexCoord);
}