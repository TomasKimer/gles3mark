/**
  *  gles3mark v1.0
  *
  *  \date   2014-05-28
  *  \author Tomas Kimer <xkimer00@stud.fit.vutbr.cz>
  *  http://blog.evoserv.at/index.php/2012/12/hemispherical-screen-space-ambient-occlusion-ssao-for-deferred-renderers-using-openglglsl
  */

precision highp float;  // mediump

in vec2 texCoord;

// gbuffer
uniform sampler2D albedoTex;
uniform sampler2D normalVStex;
uniform sampler2D depthTex;

uniform mat4      invProj;
uniform vec2      radius;

layout (location=2) out vec4 ambientColor;
layout (location=3) out float ssaoColor;

const float ambientContribution = 0.1;  //0.2
const float distanceThreshold   = 5.0;
const float strength            = 1.0;
const int sample_count          = 16;

// Poisson disk samples
const vec2 poissonSamples[] = vec2[](
    vec2( -0.94201624,  -0.39906216 ),
    vec2(  0.94558609,  -0.76890725 ),
    vec2( -0.094184101, -0.92938870 ),
    vec2(  0.34495938,   0.29387760 ),
    vec2( -0.91588581,   0.45771432 ),
    vec2( -0.81544232,  -0.87912464 ),
    vec2( -0.38277543,   0.27676845 ),
    vec2(  0.97484398,   0.75648379 ),
    vec2(  0.44323325,  -0.97511554 ),
    vec2(  0.53742981,  -0.47373420 ),
    vec2( -0.26496911,  -0.41893023 ),
    vec2(  0.79197514,   0.19090188 ),
    vec2( -0.24188840,   0.99706507 ),
    vec2( -0.81409955,   0.91437590 ),
    vec2(  0.19984126,   0.78641367 ),
    vec2(  0.14383161,  -0.14100790 )
);

vec3 reconstructPosition(in vec2 coord, in float depth) {    
    vec4 viewPos = invProj * vec4(coord * 2.0 - 1.0, depth * 2.0 - 1.0, 1.0);  // [-1..1]
    viewPos /= viewPos.w;

    return viewPos.xyz;
}

float computeSSAO(in vec3 viewPos, in vec3 viewNormal) {
    float occlusion = 0.0;
    
    for (int i = 0; i < sample_count; ++i) {
        // sample at an offset specified by the current Poisson disk sample and scale it by a radius (has to be in texture space)
        vec2  sampleTexCoord = texCoord + (poissonSamples[i] * radius);
        float sampleDepth    = texture(depthTex, sampleTexCoord).r;        
        vec3  samplePos      = reconstructPosition(sampleTexCoord, sampleDepth);
        vec3  sampleDir      = normalize(samplePos - viewPos);
 
        // angle between surface normal and sample direction (vector from surface position to sample position)
        float angle = max(dot(viewNormal, sampleDir), 0.0);
        
        // distance between surface position and sample position
        float dist = distance(viewPos, samplePos);
 
        // distance function
        float distFunc = 1.0 - smoothstep(distanceThreshold, distanceThreshold * 2.0, dist);
 
        occlusion += distFunc * angle;
    }
     
    return 1.0 - (occlusion / float(sample_count)) * strength;
}


void main() {
    // load texture values
    vec3 albedo = texture(albedoTex  , texCoord).rgb;
    vec3 normal = texture(normalVStex, texCoord).rgb;
    float depth = texture(depthTex, texCoord).r;

    // calculate the pixel's position in view space
    vec3 viewPos = reconstructPosition(texCoord, depth);
    //vec2 radius = filterRadius / vec2(1280.0, 720.0);   // vec2(textureSize(depthTex, 0));  // - app will die on Adreno
    float ssaoTerm = computeSSAO(viewPos, normal);

    // final
    ssaoColor = ssaoTerm; //vec4(vec3(ssaoTerm), 1.0);
    ambientColor = vec4(albedo * ambientContribution * ssaoTerm, 1.0);
}