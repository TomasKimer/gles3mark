
// This demo builds on demo3 but performs the lighting in eye coordinate space, not world coordinates.  In eye coordinate
// space the camera is at the origin, so we don't need to provide the cameraPos variable to tell the shader where the camera is
// located in world coordinates.  However, we do need to decide if we should model the light direction (and position for positional
// lights) in world coordinates or eye coordinates.  Typically, we model the light position in world coordinates but if we do
// we must apply the view transform matrix to transform the light direction / position into eye coordinate space.  So both world
// and eye coordinate lighting calculations have tradeoffs.


#version 330

//
// model-view-projection matrices to transform vertex positions
//
uniform mat4 modelMatrix; // modelling to world coord transform
uniform mat4 viewMatrix; // world to eye coord transform
uniform mat4 projectionMatrix; // eye to clip coord transform

//
// inverse-transpose to transform normal vectors
//
uniform mat4 invTransposeModelViewMatrix; // modelling to eye coordinate transform FOR NORMAL VECTOR ONLY

// note: we're going to transform normals directly from modelling coords into eye coords using invTransposeModelViewMatrix.
// So why have seperate model, view and projection matrices for the actual vertex position?  First, we don't want all 3 combined as 
// this would take a vertex from modelling coords straight to clip coords, by-passing eye coords where we need vertex coordinates
// to be in order to do lighting calculations.  We could combine the model and view matrices (remember the ModelView matrix last year!)
// but in our shader we need the view transform on its own because we want to transform lighting directions / positions, which are
// already in world coordinates, into eye coordinates.  Because of these requirements, it's better to have the model, view and 
// projection transformations as seperate variables.



//
// input vertex packet
//
layout (location = 0) in vec4 vertexPos;
layout (location = 1) in vec4 vertexColour;
layout (location = 2) in vec4 vertexNormal;


//
// output vertex packet
//
out vec4 posEyeCoord;
out vec3 normalEyeCoord;
out vec4 colour;



void main(void) {
	
	// combine model and view matrix into local variable
	mat4 modelViewMatrix = viewMatrix * modelMatrix;

	// vertex position in eye coords - for fragment shader
	posEyeCoord = modelViewMatrix * vertexPos;
	
	// setup output packet (fragment shader gets packet with interpolated values)
	colour = vertexColour;

	normalEyeCoord = (invTransposeModelViewMatrix * vertexNormal).xyz; // normal transformed to eye coordinate space
	

	// vertex position in clip coords - necessary for pipeline
	gl_Position = projectionMatrix * modelViewMatrix * vertexPos;
}
