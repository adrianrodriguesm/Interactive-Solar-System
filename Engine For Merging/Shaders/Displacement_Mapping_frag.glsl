#version 330
out vec4 FragColor;

in vec2 exTexcoord;
in vec4 E;

uniform sampler2D ColorMap;
uniform sampler2D HeightMap;

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
		h += h==0? 3*dh *E.z : dh * E.z;
		UV_temp += dw * E.xy;
		th = texture(HeightMap, UV_temp).r;

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
	FragColor = texture(ColorMap, texCoord);
}
