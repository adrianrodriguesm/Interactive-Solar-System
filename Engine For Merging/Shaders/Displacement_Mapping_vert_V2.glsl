#version 330

//IN VARIABLES
in vec3 inPosition;
in vec2 inTexcoord;
in vec3 inNormals;

//OUT VARIABLES
out vec2 exTexcoord;
out vec4 exPosition;
out vec3 exNormals;
out vec4 exFragmentPosition;
out vec4 exLightPosition;
out vec4 exCameraPosition;
out vec4 E;

//UNIFORM VARIABLES
uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform	mat4 ProjectionMatrix;
uniform vec4 lightPosition;
uniform float cameraValue;
uniform vec3 SunPosition;

void main()
{
	exPosition = ViewMatrix * ModelMatrix * vec4(inPosition,1);
	exTexcoord = inTexcoord;
	E = normalize(-exPosition);
	gl_Position = ProjectionMatrix * exPosition;
    
    //We calculate exNormals using the Normal matrix, in order to avoid the deformation of Normals if a scale matrix is applied previously
    exNormals = mat3(transpose(inverse(ViewMatrix * ModelMatrix))) * inNormals;
    //We calculate each frament position in View Space
    exFragmentPosition = ViewMatrix * ModelMatrix * vec4(inPosition,1.0f);
    //Light position is calculated in View Space
    exLightPosition = ViewMatrix * lightPosition;
    //We need the camera position for lighting calculations
    exCameraPosition = vec4(0.0f,0.0f,-cameraValue,1.0f); 
}