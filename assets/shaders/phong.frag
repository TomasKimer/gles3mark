
// This demo builds on demo3 but performs the lighting in eye coordinate space, not world coordinates.  In eye coordinate
// space the camera is at the origin, so we don't need to provide the cameraPos variable to tell the shader where the camera is
// located in world coordinates.  However, we do need to decide if we should model the light direction (and position for positional
// lights) in world coordinates or eye coordinates.  Typically, we model the light position in world coordinates but if we do
// we must apply the view transform matrix to transform the light direction / position into eye coordinate space.  So both world
// and eye coordinate lighting calculations have tradeoffs.


#version 330

//
// basic directional light model
//
uniform vec4		lightDirection; // direction light comes FROM (specified in World Coordinates)
uniform vec4		lightDiffuseColour;
uniform vec4        lightSpecularColour;
uniform float       lightSpecularExponent;

uniform mat4        viewMatrix; // world to eye coord transform (for lightDirection vector)


//
// input fragment packet (contains interpolated values for the fragment calculated by the rasteriser)
//
in vec4 posEyeCoord;
in vec3 normalEyeCoord;
in vec4 colour;



// output fragment colour
layout (location = 0) out vec4 fragColour;


void main(void) {

	// make sure light direction vector is unit length (store in L) and in eye coordinate space
    // We're modelling a directional light but we transform this direction with the view matrix
	vec4 L = normalize(viewMatrix * lightDirection);
    
	// important to normalise length of normal otherwise shading artefacts occur
	vec3 N = normalize(normalEyeCoord);
	
    // calculate lambertian term
    float lambertian = clamp(dot(L.xyz, N), 0.0, 1.0);

    //
	// calculate diffuse light colour
    vec3 diffuseColour = colour.rgb * lightDiffuseColour.rgb * lambertian; // input colour actually diffuse colour
    //

    //
    // calculate specular light colour
    //

    // vectors needed for specular light calculation...
    vec3 E = -posEyeCoord.xyz; // vector from point on object surface in eye coords to camera (ie. the origin for eye coord space)
    E = normalize(E);
    vec3 R = reflect(-L.xyz, N); // reflected light vector about normal N

    float specularIntensity = pow(max(dot(R, E), 0.0), lightSpecularExponent);
    vec3 specularColour = lightSpecularColour.rgb * specularIntensity * lambertian;


    //
    // combine colour components to get final pixel / fragment colour
    //
    vec3 rgbColour = diffuseColour + specularColour;


    // output final colour to framebuffer
	fragColour = vec4(rgbColour, 1.0);
}
