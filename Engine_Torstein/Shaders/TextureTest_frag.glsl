#version 330
out vec4 FragColor;

in vec2 exTexcoord;
in vec4 E;

uniform sampler2D ColorMap;

void main()
{	
	FragColor = texture(ColorMap, exTexcoord);
}
