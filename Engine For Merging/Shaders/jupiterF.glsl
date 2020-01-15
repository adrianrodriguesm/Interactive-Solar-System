#version 330 core

struct Attenuation 
{
    float constant;
    float linear;
    float quadratic;
};

in vec2 v_TexCoord; 
in vec3 v_Normals;
in vec4 v_FragPos;
in vec4 v_lightPosition;
in vec4 v_CameraPos;

out vec4 out_Color;

uniform sampler2D u_Texture;
uniform vec4 lightColor;
uniform Attenuation att;

//Rotation Matrix
mat2 rotate2d(float angle)
{
    return mat2(cos(angle),-sin(angle), sin(angle),cos(angle));
}

// Simple random function
float random (in vec2 st) 
{
    return fract(sin(dot(st.xy, vec2(12.9898,78.233))) * 43758.5453123);
}
void main(void)
{
    //COLORS
    vec4 cream = vec4(1.000f, 0.973f, 0.863f, 1.0f);
    vec4 orange = vec4(1.000f, 0.647f, 0.000f, 1.0f);
    vec4 darkOrange = vec4(0.957, 0.643, 0.376, 1.0f);
    vec4 chocolate = vec4(0.961, 0.871, 0.702, 1.0f);
    vec4 brown = vec4(0.741, 0.718, 0.420, 1.0f);
    
    float shininess = 32.0f;
    vec3 normal = normalize(v_Normals);
    vec4 lightDirection = normalize(v_lightPosition - v_FragPos);
    vec4 viewDir = normalize(v_FragPos - v_CameraPos);
    float distance = length(v_lightPosition - v_FragPos);
    float attenuationValue = 1.0f / (att.constant + att.linear * distance + 
    		    att.quadratic * (distance * distance)); ;
    
    float rand = random(v_TexCoord);
    mat2 rot = rotate2d(radians(90.0));
    
    //AMBIENT LIGHT
    float ambientStrength = 0.2f;
    vec4 ambientLight = lightColor * ambientStrength * attenuationValue;
    
    //DIFFUSE LIGHT
    float diff = max(dot(vec4(normal,1.0f),lightDirection), 0.0f);
    vec4 diffuseLight = lightColor * diff * attenuationValue;
    
    //SPECULAR LIGHT
    float specularStrength = 0.3;
    vec3 halfwayVector = normalize(lightDirection + viewDir).xyz;
    
    float spec =  pow(max(dot(normal, halfwayVector), 0.0), shininess);
    vec4 specularLight = lightColor * specularStrength * spec * attenuationValue;
    
    
    vec4 firstTexColor = texture(u_Texture, rot * vec2((1.0 - v_TexCoord.x), v_TexCoord.y + sin(rand) / 30.0));
    vec4 secondTexColor = texture(u_Texture,v_TexCoord);
    vec4 finalColor = mix(firstTexColor, secondTexColor, 0.4) * (ambientLight + diffuseLight +  specularLight);
    
    
    
    //COLOR SELECTION
    if(firstTexColor.r < 0.20f && firstTexColor.g < 0.20f && firstTexColor.b < 0.20f)
    {
        out_Color = mix(finalColor,darkOrange,0.5f); 
    }
    else if(firstTexColor.r < 0.35f && firstTexColor.g < 0.35f && firstTexColor.b < 0.35f)
    {
        out_Color = mix(mix(finalColor,orange,0.5f), cream, 0.2);
    }
    else if(firstTexColor.r < 0.50f && firstTexColor.g < 0.50f && firstTexColor.b < 0.50f)
    {
        out_Color = mix(mix(finalColor,chocolate,0.4f), orange, 0.05);
    }
    else if(firstTexColor.r < 0.70f && firstTexColor.g < 0.70f && firstTexColor.b < 0.70f)
    {
        out_Color = mix(finalColor,cream,0.4f);
    }
    else if(firstTexColor.r < 0.90f && firstTexColor.g < 0.90f && firstTexColor.b < 0.90f)
    {
        out_Color = mix(finalColor,brown,0.4f);
    }
    else
    {
        out_Color = mix(finalColor,darkOrange, 0.3f);
    }

}