
precision highp float;  // mediump

in vec2 texCoord;

// gbuffer
uniform sampler2D albedoTex;
uniform sampler2D normalVStex;
uniform sampler2D depthTex;

uniform mat4      invProj;
uniform mat4      projection;

const float ambientContribution = 0.2;  //0.2
const int KERNEL_SIZE = 32;  // max 64
const float totalStrength = 1.0;
const float radius = 4.0;
const float noiseSize = 4.0;
const vec2 noiseScale = vec2(noiseSize / 1280.0, noiseSize / 720.0);  // vec2(0.003125, 0.005556)

uniform vec3 kernel[KERNEL_SIZE];
uniform sampler2D noiseTex;

layout (location=2) out vec4 fragColor;

float getLinearDepth(float zn, float zf, float depth) {
    return ((2.0*zf*zn)/(zf-zn)) / (depth - ((zf+zn)/(zf-zn)));
    //return (2.0*zn) / (zf + zn - depth * (zf - zn)); 
}

mat3 getRotationMatrix(vec3 normal, vec3 noise) {
    vec3 tangent = normalize(noise - normal * dot(noise, normal));
    vec3 bitangent = cross(normal, tangent);

    return mat3(tangent, bitangent, normal);
}


void main() {
	//vec2 texCoord = gl_FragCoord.xy / viewport;  // map to [0..1]
    
    // Load texture values
    vec3 albedo = texture(albedoTex  , texCoord).rgb;
    vec3 normal = texture(normalVStex, texCoord).rgb;
    float depth = texture(depthTex   , texCoord).r;
    vec3 noise  = texture(noiseTex   , texCoord).xyz;  // tc * noiseScale

    // Calculate the pixel's position in view space
    vec4 viewPos = invProj * vec4(texCoord * 2.0 - 1.0, depth * 2.0 - 1.0, 1.0);  // [-1..1]
    viewPos /= viewPos.w;

    mat3 rotationMatrix = getRotationMatrix(normal, noise);    
    float occlusion = 0.0;

    for (int i = 0; i < KERNEL_SIZE; ++i) {
        vec3 sample = rotationMatrix * kernel[i];
        sample = sample * radius + viewPos.xyz;

        vec4 offset = projection * vec4(sample, 1.0);
        offset.xy /= offset.w;
        offset.xy = offset.xy * 0.5 + 0.5;

        float currentDepth = texture(depthTex, offset.xy).r;
        float linearDepth = getLinearDepth(1.0, 200.0, currentDepth);

        if (abs(viewPos.z - linearDepth) < radius) {
            occlusion += (linearDepth >= sample.z) ? 1.0 : 0.0;
        }
    }

    float ssaoTerm = 1.0 - totalStrength * (occlusion / float(KERNEL_SIZE));

    // Final
    fragColor = vec4(albedo * ambientContribution * ssaoTerm /*vec3(occlusion)*/, 1.0);   // vec3(getLinearDepth(1.0, 200.0, depth)  texture(noiseTex, texCoord * noiseScale).xyz
}