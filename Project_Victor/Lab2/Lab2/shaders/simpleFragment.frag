#version 330 core

in vec2 v_TexCoord; 
out vec4 out_Color;

uniform vec4 u_Color;
uniform sampler2D u_Texture;

void main(void)
{
	vec2 p = vec2(v_TexCoord.x,v_TexCoord.y);
	vec4 texColor = texture(u_Texture,p);
    out_Color = texColor;
     
    //out_Color = texture(u_Texture, vec2( 1.0 - v_TexCoord.x, v_TexCoord.y +sin(v_TexCoord.x * 60.0 + 1.0 * 2.0) / 30.0)) * vec4(0.7, 0.7, 1.0, 1.0);

}