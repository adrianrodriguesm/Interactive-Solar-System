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
    exNormals = mat3(transpose(inverse(ViewMatrix * ModelMatrix))) * inNormals;
    exFragmentPosition = ViewMatrix * ModelMatrix * inPosition;
    exLightPosition = ViewMatrix * lightPosition;
    exCameraPosition = vec4(0.0f,0.0f,-cameraValue,1.0f); 
}