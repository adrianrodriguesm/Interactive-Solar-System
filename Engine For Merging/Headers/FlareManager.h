#include "Vectors.h"
#include "Camera.h"
#include "FlareTexture.h"

struct  FlareManager
{
	const vec2 CENTER = vec2(0.0, 0.0);
	FlareTexture* flareTextures[6];
	float spacing;
	
	FlareManager(FlareTexture* flareTextures[6], float spacing);
	void render(Camera* cam, const vec4& sunPos);
	vec2 convertToScreenSpace(Camera* cam, const vec4& sunPos);
	void flarePos(vec2& sunToCenter, vec2& sunCoords, float& brightness);
};
