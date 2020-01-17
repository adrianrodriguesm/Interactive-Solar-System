#version 330 core

//IN VARIABLES
in vec4 in_Position;
in vec2 texCoord;
in vec3 normals;

//OUT VARIABLES
out vec2 v_TexCoord;
out vec3 v_Normals;
out vec4 v_FragPos;
out vec4 v_lightPosition;
out vec4 v_CameraPos;

//UNIFORM VARIABLES
uniform mat4 ModelMatrix;
uniform	mat4 ViewMatrix;
uniform	mat4 ProjectionMatrix;
uniform vec4 lightPosition;
uniform float cameraValue;

void main(void)
{
	gl_Position =  ProjectionMatrix*ViewMatrix * ModelMatrix * in_Position;
	v_TexCoord = texCoord;
    //Normals are calculated using the Normal matrix, in order to avoid the deformation of Normals if a scale matrix is applied previously
    v_Normals = mat3(transpose(inverse(ViewMatrix * ModelMatrix))) * normals;
    //We calculate each frament position in View Space
    v_FragPos = ViewMatrix * ModelMatrix * in_Position;
    //Light position is calculated in View Space
    v_lightPosition = ViewMatrix * lightPosition;
     //We need the camera position for lighting calculations
    v_CameraPos = vec4(0.0f,0.0f,-cameraValue,1.0f);
}