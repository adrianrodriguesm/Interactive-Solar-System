#include "FlareManager.h"

FlareManager::FlareManager(FlareTexture* flareTextures[6], float spacing)
{
	
	for (int i = 0; i < 6; i++)
	{
		this->flareTextures[i] = flareTextures[i];
	}
	this->spacing = spacing;
}

void FlareManager::render(Camera* cam, const vec4& sunPos)
{
	//convert sun pos to coords on the screen
	vec2 sunScreenPos = convertToScreenSpace(cam, sunPos);
	//calculate line from the center
	vec2 sunToCenter = CENTER - sunScreenPos;
	//calculate brightness from distance
	float brightness = 1 - (sunToCenter.Magnitude() / 0.6f);
	//calculate flarePosition
	if (brightness > 0) {
		flarePos(sunToCenter, sunScreenPos, brightness);
	}

}



vec2 FlareManager::convertToScreenSpace(Camera* cam, const vec4& sunPos)
{
	vec4 pos = cam->ViewMatrix * sunPos;
	pos = cam->ProjectionMatrix * pos;
	float x = pos.x / pos.w;
	float y = pos.y / pos.w;
	vec2 result = vec2(x,y);
	
	return result;
}

void FlareManager::flarePos(vec2& sunToCenter, vec2& sunCoords, float& brightness)
{
	vec2 flarePos;
	for (int i = 0; i < 6; i++) {
		vec2 dir = vec2(sunToCenter);
		dir = dir * spacing;
		flarePos += dir + sunCoords;
		
		flareTextures[i]->pos = flarePos;
		flareTextures[i]->brightness = brightness;
	}
}

