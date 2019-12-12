#version 330
out vec4 FragColor;

in vec2 ex_Texcoord;

uniform sampler2D colorMap;

void main()
{	
	FragColor = texture(colorMap, ex_Texcoord);
}
