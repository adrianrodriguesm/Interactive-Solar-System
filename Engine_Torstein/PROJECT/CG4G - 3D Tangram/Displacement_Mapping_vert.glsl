#version 330
in vec3 inPosition;
in vec2 inTexcoord;

out vec2 exTexcoord;
out vec4 exPosition;

uniform mat4 ModelMatrix;
uniform sampler2D heightMap;

uniform Camera
{
	mat4 ViewMatrix;
	mat4 Projection;
	vec3 CamPosition;
};


bool inside = true;
float h = 0;
int max_iter = 1000;

void main()
{
	vec4 worldPosition = ModelMatrix * vec4(inPosition,1);

	vec2 texCoord = inTexcoord;
	vec2 UV_temp = inTexcoord;
	vec3 E = normalize(CamPosition - vec3(worldPosition));

	float th;
	float dh = E.z/max_iter;
	vec2 dw = vec2(length(vec2(E.x, E.y))/max_iter);

	for (int i=0; i<max_iter; i++)
	{
		h += dh * E.z;
		UV_temp += dw * E.xy;
		th = texture(heightMap, UV_temp).r;

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

	exTexcoord = texCoord;
	gl_Position = Projection * ViewMatrix * worldPosition;
}