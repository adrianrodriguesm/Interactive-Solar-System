#version 330 core

in vec2 v_TexCoord; 
out vec4 out_Color;

uniform vec4 u_Color;
uniform sampler2D u_Texture;

void main(void)
{
	vec2 p = vec2(v_TexCoord.x*0.1f,v_TexCoord.y);
	vec4 texColor = texture(u_Texture,p);
	out_Color = mix(texColor,u_Color, 0.2f);
}