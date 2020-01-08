#pragma once

struct ShadowMap {
	ShadowMap();
	
	void createFBOShadowMap();
	void setScreenSize(const unsigned int width, const unsigned int height);
	void renderDepthMap();
	void renderWithShadowMap();
	void bindDepthMap();

private:
	unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
	unsigned int SCR_WIDTH, SCR_HEIGHT;
	unsigned int depthMapFBO;///FBO depthMap
	unsigned int depthMap;///Texture shadow
};