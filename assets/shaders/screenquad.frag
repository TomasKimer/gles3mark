
precision mediump float;

in vec2 fragTexCoord;

uniform sampler2D tex;

layout (location=0) out vec4 fragColor;

void main()
{
    fragColor = texture(tex, fragTexCoord);
}