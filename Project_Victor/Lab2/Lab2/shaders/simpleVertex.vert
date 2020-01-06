#version 330 core

//IN VARIABLES
in vec4 in_Position;
in vec2 texCoord;
in vec3 normals;

//UNIFORM VARIABLES
uniform mat4 Matrix;
uniform	mat4 ViewMatrix;
uniform	mat4 ProjectionMatrix;


//OUT VARIABLES
out vec2 v_TexCoord;



void main(void)
{
	gl_Position =  ProjectionMatrix*ViewMatrix * Matrix * in_Position;
	v_TexCoord = texCoord;

}