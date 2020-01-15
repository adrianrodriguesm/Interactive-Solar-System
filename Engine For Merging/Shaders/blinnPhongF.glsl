#version 330

struct Attenuation 
{
    float constant;
    float linear;
    float quadratic;
};

//IN VARIABLES
in vec2 exTexcoord;
in vec3 exNormals;
in vec4 exFragmentPosition;
in vec4 exLightPosition;
in vec4 exCameraPosition;

//OUT VARIABLES
out vec4 FragColor;

//UNIFORM VARIABLES
uniform vec4 lightColor;
uniform Attenuation att;
uniform sampler2D Texture;

void main()
{	 
    //LIGHTING CALCULATIONS
    
    float shininess = 32.0f;
    vec3 normal = normalize(exNormals);
    vec4 lightDirection = normalize(exLightPosition - exFragmentPosition);
    vec4 viewDir = normalize(exFragmentPosition - exCameraPosition);
    float distance = length(exLightPosition - exFragmentPosition);
    float attenuationValue = 1.0f / (att.constant + att.linear * distance + 
    att.quadratic * (distance * distance)); 
                
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
    
                
	FragColor = texture(Texture, exTexcoord) * (ambientLight + diffuseLight +  specularLight);
}