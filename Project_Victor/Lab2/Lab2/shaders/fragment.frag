#version 330 core


in vec2 v_TexCoord; 
out vec4 out_Color;

uniform vec4 u_Color;
uniform sampler2D u_Texture;
uniform float time;

//Rotation Matrix
mat2 rotate2d(float angle){
    return mat2(cos(angle),-sin(angle), sin(angle),cos(angle));
}

// Simple random function
float random (in vec2 st) {
    return fract(sin(dot(st.xy, vec2(12.9898,78.233))) * 43758.5453123);
}
void main(void)
{
    //COLORS
    vec4 cream = vec4(1.000f, 0.973f, 0.863f, 1.0f);
    vec4 orange = vec4(1.000f, 0.647f, 0.000f, 1.0f);
    vec4 khaki = vec4(0.941f, 0.902f, 0.549f, 1.0f);
    vec4 darkOrange = vec4(0.957, 0.643, 0.376, 1.0f);
    vec4 chocolate = vec4(0.961, 0.871, 0.702, 1.0f);
    vec4 brown = vec4(0.741, 0.718, 0.420, 1.0f);
    
    float rand = random(v_TexCoord);
    mat2 rot = rotate2d(radians(90.0));
    
    vec4 firstTexColor = texture(u_Texture, rot * vec2((1.0 - v_TexCoord.x), v_TexCoord.y + sin(rand) / 30.0));
    vec4 secondTexColor = texture(u_Texture,v_TexCoord);
    vec4 finalColor = mix(firstTexColor, secondTexColor, 0.5);
    
    if(firstTexColor.r < 0.20f || firstTexColor.g < 0.20f || firstTexColor.b < 0.20f)
    {
        out_Color = mix(finalColor,darkOrange,0.5f);  //FRANJA PRONUNCIADA
    }
    else if(firstTexColor.r < 0.35f || firstTexColor.g < 0.35f || firstTexColor.b < 0.35f)
    {
        out_Color = mix(mix(finalColor,orange,0.5f), cream, 0.2);
    }
    else if(firstTexColor.r < 0.50f || firstTexColor.g < 0.50f || firstTexColor.b < 0.50f)
    {
        out_Color = mix(mix(finalColor,chocolate,0.4f), orange, 0.05);
    }

    
    else if(firstTexColor.r < 0.70f || firstTexColor.g < 0.70f || firstTexColor.b < 0.70f)
    {
        out_Color = mix(finalColor,cream,0.4f);
    }
    else if(firstTexColor.r < 0.90f || firstTexColor.g < 0.90f || firstTexColor.b < 0.90f)
    {
        out_Color = mix(finalColor,brown,0.4f);
    }
    else
    {
        out_Color = mix(finalColor,darkOrange, 0.3f);
    }
    
}