
precision highp float;  // mediump

in vec2 texCoord;

// gbuffer
uniform sampler2D albedoTex;
uniform sampler2D normalVStex;
uniform sampler2D depthTex;

uniform mat4      invProj;
//uniform mat4      projection;

const float ambientContribution = 0.2;  //0.2
//const int KERNEL_SIZE = 32;  // max 64
//const float totalStrength = 1.0;
//const float radius = 4.0;
//uniform vec3 kernel[KERNEL_SIZE];
//uniform sampler2D noiseTex;

//in vec3 viewRay;

layout (location=2) out vec4 fragColor;

/*
float linearizeDepth(in float depth) {
	return projection[3][2] / (depth - projection[2][2]);

    //float zn = 1.0;
    //float zf = 200.0;    
    //return ((2.0*zf*zn)/(zf-zn)) / (depth - ((zf+zn)/(zf-zn)));
    //return (2.0*zn) / (zf + zn - depth * (zf - zn)); 
}

//	construct kernel basis matrix:
mat3 constructKernelBasis(in vec3 rvec, in vec3 normal) {
	vec3 tangent = normalize(rvec - normal * dot(rvec, normal));
	vec3 bitangent = cross(tangent, normal);
	return mat3(tangent, bitangent, normal);
}

float ssao(in mat3 kernelBasis, in vec3 originPos, in float radius) {
	float occlusion = 0.0;
	
    for (int i = 0; i < KERNEL_SIZE; ++i) {
	    //	get sample position:
		vec3 samplePos = kernelBasis * kernel[i];
		samplePos = samplePos * radius + originPos;
		
	    //	project sample position:
		vec4 offset = projection * vec4(samplePos, 1.0);
		offset.xy /= offset.w; // only need xy
		offset.xy = offset.xy * 0.5 + 0.5; // scale/bias to texcoords
		
	    //	get sample depth:
		float sampleDepth = texture(depthTex, offset.xy).r;
		sampleDepth = linearizeDepth(sampleDepth);
		
        float rangeCheck = smoothstep(0.0, 1.0, radius / abs(originPos.z - sampleDepth));  // abs(viewPos.z - linearDepth) < radius ? 1.0 : 0.0;
        occlusion += rangeCheck * step(sampleDepth, samplePos.z);                          // rangeCheck * (linearDepth <= sample.z ? 1.0 : 0.0);
        //if ( (abs(samplePos.z - sampleDepth) < radius) && (sampleDepth <= samplePos.z) ) occlusion += 1.0;
	}
	
	occlusion = 1.0 - (occlusion / float(KERNEL_SIZE));
	return pow(occlusion, 2.0);  // uPower
}*/

const int sample_count = 16;
const vec2 poisson16[] = vec2[](    // These are the Poisson Disk Samples
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
const float filterRadius = 5.0;  // 10 / screenWidth, 10 / screenHeight
const float distanceThreshold = 5.0;

vec3 calculatePosition(in vec2 coord, in float depth)
{
    //return viewRay * linearizeDepth(depth); //    
    
    vec4 viewPos = invProj * vec4(coord * 2.0 - 1.0, depth * 2.0 - 1.0, 1.0);  // [-1..1]
    viewPos /= viewPos.w;

    // restore position
    return viewPos.xyz; //restoredDepth;
}

float ssao2(in vec3 viewPos, in vec3 viewNormal, in vec2 radius) {
    float ambientOcclusion = 0.0;
    // perform AO
    for (int i = 0; i < sample_count; ++i) {
        
        // sample at an offset specified by the current Poisson-Disk sample and scale it by a radius (has to be in Texture-Space)
        vec2 sampleTexCoord = texCoord + (poisson16[i] * radius);
        float sampleDepth = texture(depthTex, sampleTexCoord).r;
        vec3 samplePos = calculatePosition(sampleTexCoord, sampleDepth);
        vec3 sampleDir = normalize(samplePos - viewPos);
 
        // angle between SURFACE-NORMAL and SAMPLE-DIRECTION (vector from SURFACE-POSITION to SAMPLE-POSITION)
        float NdotS = max(dot(viewNormal, sampleDir), 0.0);
        // distance between SURFACE-POSITION and SAMPLE-POSITION
        float VPdistSP = distance(viewPos, samplePos);
 
        // a = distance function
        float a = 1.0 - smoothstep(distanceThreshold, distanceThreshold * 2.0, VPdistSP);
        // b = dot-Product
        float b = NdotS;
 
        ambientOcclusion += (a * b);
    }
 
    return 1.0 - (ambientOcclusion / float(sample_count));
}


void main() {
    // Load texture values
    vec3 albedo = texture(albedoTex  , texCoord).rgb;
    vec3 normal = texture(normalVStex, texCoord).rgb;    //	get view space normal: decodeNormal(texture(uGBufferGeometricTex, vertexIn.texcoord).rgb);
    float depth = texture(depthTex, texCoord).r;

    //	sample noise texture
	//vec2 noiseTexCoords = texCoord * vec2(textureSize(depthTex, 0)) / vec2(textureSize(noiseTex, 0));
	//vec3 rvec = texture(noiseTex, noiseTexCoords).rgb; // * 2.0 - 1.0;

	//	get view space origin:
	//vec3 originPos = viewRay * linearizeDepth(depth, projection);

    // Calculate the pixel's position in view space
    //vec4 viewPos = invProj * vec4(texCoord * 2.0 - 1.0, depth * 2.0 - 1.0, 1.0);  // [-1..1]
    //viewPos /= viewPos.w;
    
    //originPos = viewPos.xyz;

    //vec3 ray = normalize(viewRay);
    //ray = ray / ray.z;
    //originPos = linearizeDepth(depth, projection) * ray;


    //mat3 kernelBasis = constructKernelBasis(rvec, normal); //	construct kernel basis matrix
    //float ssaoTerm = ssao(kernelBasis, originPos, radius);

    vec3 viewPos = calculatePosition(texCoord, depth);
    vec2 radius = filterRadius / vec2(1280.0, 720.0);   // vec2(textureSize(depthTex, 0));  // - app die on Adreno
    float ssaoTerm2 = ssao2(viewPos, normal, radius);


    // Final
    //fragColor = vec4(vec3(ssaoTerm2), 1.0);
    fragColor = vec4(albedo * ambientContribution * ssaoTerm2, 1.0);
}