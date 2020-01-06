#version 330 core
layout (location = 0) in vec4 aPos;
layout (location = 1) in vec2 aTexCoords;

out vec2 TexCoords;

uniform mat4 ModelMatrix;
uniform	mat4 ViewMatrix;
uniform	mat4 ProjectionMatrix;

void main()
{
    
    TexCoords = aTexCoords;
    
    gl_Position = ProjectionMatrix*ViewMatrix * ModelMatrix*aPos;
}