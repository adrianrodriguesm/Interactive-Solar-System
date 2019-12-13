#version 330
in vec3 inPosition;
in vec2 inTexcoord;

out vec2 exTexcoord;
out vec4 exPosition;
out vec4 E;

uniform mat4 ModelMatrix;

uniform mat4 ViewMatrix;
uniform	mat4 ProjectionMatrix;


void main()
{
	exPosition = ViewMatrix * ModelMatrix * vec4(inPosition,1);
	exTexcoord = inTexcoord;
	E = normalize(-exPosition);
	gl_Position = ProjectionMatrix * exPosition;
}