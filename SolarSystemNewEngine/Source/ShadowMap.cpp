#include "../Include/ShadowMap.h"
#include <GL/glew.h>
#include <string>
#include <iostream>
ShadowMap::ShadowMap()
{
}

void ShadowMap::createFBOShadowMap()
{
	////Generated depthMapFBO
	glGenFramebuffers(1, &depthMapFBO);

	///Generated depthMap texture
	glGenTextures(1, &depthMap);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
		SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);


	////attach the generated depth texture to the framebuffer's depth buffer
	
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
	//We only need the depth information when rendering 
	//the scene from the light's perspective so there is no need for a color buffer.
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);/////Tell OpenGL we're not going to render any color data
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void ShadowMap::renderDepthMap()
{
	glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glClear(GL_DEPTH_BUFFER_BIT);
		//ConfigureShaderAndMatrices();
		//RenderScene();
	//glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void ShadowMap::renderWithShadowMap()
{
	glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	///ConfigureShaderAndMatrices();
	
}
void ShadowMap::bindDepthMap()
{
	glBindTexture(GL_TEXTURE_2D, depthMap);
	//RenderScene();
}
void ShadowMap::setScreenSize(const unsigned int width, const unsigned int height)
{
	this->SCR_WIDTH = width;
	this->SCR_HEIGHT = height;
}