#version 330
out vec4 FragColor;

in vec3 exTexcoord;

uniform samplerCube SkyBox;

void main()
{	
	FragColor = texture(SkyBox, exTexcoord);
}
