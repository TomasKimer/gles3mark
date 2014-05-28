
precision highp float;  // mediump

// gbuffer
uniform sampler2D albedoTex;
uniform sampler2D normalVStex;
uniform sampler2D depthTex;
uniform sampler2D ssaoTex;

uniform mat4      invProj;
uniform vec2      viewport;

uniform vec3  lightColor;
uniform vec3  lightPos;
uniform float lightSize;

layout (location=2) out vec4 fragColor;

vec3 reconstructPosition(in vec2 coord, in float depth) {    
    vec4 viewPos = invProj * vec4(coord * 2.0 - 1.0, depth * 2.0 - 1.0, 1.0);  // [-1..1]
    viewPos /= viewPos.w;

    return viewPos.xyz;
}

void main() {
	vec2 texCoord = gl_FragCoord.xy / viewport;  // map to [0..1]
    
    // load texture values (g-buffer)
    vec3 albedo = texture(albedoTex  , texCoord).rgb;
    vec3 normal = texture(normalVStex, texCoord).rgb;
    float depth = texture(depthTex   , texCoord).r;
    float ssao  = texture(ssaoTex    , texCoord).r;

    // calculate the pixel's position in view space
    vec3 viewPos = reconstructPosition(texCoord, depth);

    // calculate light attenuation
    vec3 lightDir = lightPos - viewPos;
    float dist = length(lightDir);
    float attenuation = 1.0 - pow(clamp(dist / lightSize, 0.0, 1.0), 2.0);
    
    // calculate diffuse lighting
    lightDir = normalize(lightDir);
    vec3 diffuse = lightColor * clamp(dot(lightDir, normal), 0.0, 1.0);

    // final combination
    fragColor = vec4(attenuation * diffuse * albedo * ssao, 1.0);
}