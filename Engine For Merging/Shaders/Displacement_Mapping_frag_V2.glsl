#version 330

struct Attenuation 
{
    float constant;
    float linear;
    float quadratic;
};

//IN VARIABLES
in vec2 exTexcoord;
in vec4 exPosition;
in vec3 exNormals;
in vec4 exFragmentPosition;
in vec4 exLightPosition;
in vec4 exCameraPosition;
in vec4 E;

//OUT VARIABLES
out vec4 FragColor;

//UNIFORM VARIABLES
uniform sampler2D ColorMap;
uniform sampler2D HeightMap;
uniform sampler2D SpecularMap;
uniform vec4 lightColor;
uniform Attenuation att;

bool inside = true;
float h = 0;
int max_iter = 1000;

void main()
{	
	vec2 UV_temp = exTexcoord;
	vec2 texCoord = exTexcoord;
	float th;
	float dh = E.z/max_iter;
	vec2 dw = vec2(length(vec2(E.x, E.y))/max_iter);
	for (int i=0; i<max_iter; i++)
	{
		h += dh * E.z;
		UV_temp += dw * E.xy;
		//The height from the heightmap is normalized to 0.1 and then scaled to 0.3 because i thought it looked better:
		th = min(texture(HeightMap, UV_temp).r, 0.1) * 3; 

		if (inside && h>th)
		{
			texCoord = UV_temp;
			inside = false;
		}
		else if (!inside && th>h)
		{
			inside = true;
		}
	}
    
    
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
	float water = texture(SpecularMap, texCoord).x; //I added the texture line that returns 0 if land and 1 if water
    float specularStrength = 0.3 * water;
    vec3 halfwayVector = normalize(lightDirection + viewDir).xyz;
    float spec =  pow(max(dot(normal, halfwayVector), 0.0), shininess);
    vec4 specularLight = lightColor * specularStrength * spec * attenuationValue;
    
                
	FragColor = texture(ColorMap, texCoord) * (ambientLight + diffuseLight +  specularLight);
}
