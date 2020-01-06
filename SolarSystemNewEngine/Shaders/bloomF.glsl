#version 330 core
layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;


 in vec2 TexCoords;

out vec4 out_Colour;
uniform vec4 u_Color;
uniform sampler2D u_Texture;

void main(void){

	FragColor = texture(u_Texture, TexCoords);
	float brightness = dot(FragColor.rgb,vec3(0.2126, 0.7152, 0.0722));
	if(brightness > 0.7){
		BrightColor = vec4(FragColor.rgb, 1.0);
	}else{
		BrightColor = vec4(0.0, 0.0, 0.0, 1.0);
	}
		
}