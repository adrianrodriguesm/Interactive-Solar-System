#version 330

in vec2 pass_textureCoords;

out vec4 out_colour;

uniform sampler2D u_Texture;
uniform float brightness;

void main(void){

    out_colour = texture(u_Texture, pass_textureCoords);
    out_colour.rgb *= brightness;


}