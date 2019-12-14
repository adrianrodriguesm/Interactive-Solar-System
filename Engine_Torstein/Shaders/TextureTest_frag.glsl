#version 330
out vec4 FragColor;

in vec2 ex_Texcoord;
in vec4 E;

uniform sampler2D ColorMap;
uniform sampler2D HeightMap;

void main()
{	
	FragColor = texture(ColorMap, ex_Texcoord);
}
