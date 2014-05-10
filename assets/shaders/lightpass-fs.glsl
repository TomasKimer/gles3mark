
precision highp float;  // mediump

// gbuffer
uniform sampler2D albedoTex;
uniform sampler2D normalVStex;
uniform sampler2D depthTex;

uniform mat4      invProj;
uniform vec2      viewport;

uniform vec3  lightColor;
uniform vec3  lightPos;
uniform float lightSize;

layout (location=2) out vec4 fragColor;

void main() {
	vec2 texCoord = gl_FragCoord.xy / viewport;  // map to [0..1]
    
    // Load texture values
    vec3 albedo = texture(albedoTex  , texCoord).rgb;
    vec3 normal = texture(normalVStex, texCoord).rgb;
    float depth = texture(depthTex   , texCoord).r;

    // Calculate the pixel's position in view space
    vec4 viewPos = invProj * vec4(texCoord * 2.0 - 1.0, depth * 2.0 - 1.0, 1.0);  // [-1..1]
    viewPos /= viewPos.w;

    // Calculate light attenuation
    vec3 lightDir = lightPos - viewPos.xyz;
    float dist = length(lightDir);
    float attenuation = 1.0 - pow(clamp(dist / lightSize, 0.0, 1.0), 2.0);
    
    // Calculate diffuse lighting
    lightDir = normalize(lightDir);
    vec3 diffuse = lightColor * clamp(dot(lightDir, normal), 0.0, 1.0);

    // Final
    fragColor = vec4(attenuation * diffuse * albedo, 1.0);
}