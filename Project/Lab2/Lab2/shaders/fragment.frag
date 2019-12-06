#version 330 core


out vec4 out_Color;
uniform vec4 u_Color;

void main(void)
{
	out_Color = u_Color;
}