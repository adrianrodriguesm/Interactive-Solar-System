#version 330

//IN VARIABLES
in vec4 inPosition;
in vec2 inTexcoord;
in vec3 inNormals;

//OUT VARIABLES
out vec2 exTexcoord;
out vec3 exNormals;
out vec4 exFragmentPosition;
out vec4 exLightPosition;
out vec4 exCameraPosition;

//UNIFORM VARIABLES
uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform	mat4 ProjectionMatrix;
uniform vec4 lightPosition;
uniform float cameraValue;

void main(void)
{
	gl_Position =  ProjectionMatrix*ViewMatrix * ModelMatrix * inPosition;
	exTexcoord = inTexcoord;
    
    //We calculate exNormals using the Normal matrix, in order to avoid the deformation of Normals if a scale matrix is applied previously
    exNormals = mat3(transpose(inverse(ViewMatrix * ModelMatrix))) * inNormals;
    //We calculate each frament position in View Space
    exFragmentPosition = ViewMatrix * ModelMatrix * inPosition;
    //Light position is calculated in View Space
    exLightPosition = ViewMatrix * lightPosition;
    //We need the camera position for lighting calculations
    exCameraPosition = vec4(0.0f,0.0f,-cameraValue,1.0f); 
}