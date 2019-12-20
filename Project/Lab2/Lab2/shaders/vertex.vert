#version 330 core

layout (location = 0) in vec4 in_Position;
layout (location = 1) in vec2 texCoord;

uniform mat4 Matrix;


uniform	mat4 ViewMatrix;
uniform	mat4 ProjectionMatrix;

out vec2 v_TexCoord;


void main(void)
{
	gl_Position =   ProjectionMatrix*ViewMatrix * Matrix * in_Position;
	v_TexCoord = texCoord;
	
}