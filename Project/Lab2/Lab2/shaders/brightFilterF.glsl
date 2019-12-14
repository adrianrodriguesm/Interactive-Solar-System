#version 330 core
layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;

in vec2 v_TexCoord;

out vec4 out_Colour;

uniform sampler2D u_Texture;

void main(void){

	FragColor = texture(u_Texture, v_TexCoord);
	float brightness = (FragColor.r * 0.2126) + (FragColor.g * 0.7152) + (FragColor.b * 0.0722);
	 if(brightness > 0.7)
        BrightColor = vec4(FragColor.rgb, 1.0);
    else
        BrightColor = vec4(0.0, 0.0, 0.0, 1.0);
	

}