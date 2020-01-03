#version 330

out vec4 out_Color;

uniform vec4 colorOverride;

void main()
{
	out_Color = colorOverride;
}