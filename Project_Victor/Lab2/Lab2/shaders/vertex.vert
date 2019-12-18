#version 330 core

in vec4 in_Position;
in vec2 texCoord;

uniform mat4 Matrix;


uniform	mat4 ViewMatrix;
uniform	mat4 ProjectionMatrix;
uniform float time;

out vec2 v_TexCoord;



void main(void)
{
	gl_Position =   ProjectionMatrix*ViewMatrix * Matrix * in_Position;
	v_TexCoord = texCoord;

}