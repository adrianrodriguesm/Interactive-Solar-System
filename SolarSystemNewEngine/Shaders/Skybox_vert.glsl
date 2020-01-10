#version 330
in vec3 inPosition;

out vec3 exTexcoord;

uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform	mat4 ProjectionMatrix;

void main()
{
	exTexcoord = vec3(ModelMatrix * vec4(inPosition,1));
	vec4 pos = ProjectionMatrix * ViewMatrix * ModelMatrix * vec4(inPosition,1);
	gl_Position = pos.xyww; //We set z to 1 in order for the skybox to be drawn behind everything else
}