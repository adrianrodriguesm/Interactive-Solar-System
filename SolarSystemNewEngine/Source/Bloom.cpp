#include <GL/glew.h>
#include <string>
#include <iostream>

#include "../Include/Bloom.h"
#include "../Include/matFactory.h"


Bloom::Bloom()
	:hdrFBO(0), colorBuffers(), pingpongFBO(), pingpongColorbuffers(), horizontal(true), 
	first_iteration(true), bloom(false), quadVAO(0), quadVBO(0)
{
	
}


void Bloom::bindHDRBuffer()
{
	glBindFramebuffer(GL_FRAMEBUFFER, hdrFBO);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Bloom::createBrightFilterBuffer()
{
	// set up floating point framebuffer to render scene to
	
	glGenFramebuffers(1, &hdrFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, hdrFBO);
	
	glGenTextures(2, colorBuffers);
	for (unsigned int i = 0; i < 2; i++)
	{
		glBindTexture(GL_TEXTURE_2D, colorBuffers[i]);
		glTexImage2D(
			GL_TEXTURE_2D, 0, GL_RGB16F, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGB, GL_FLOAT, NULL
		);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		// attach texture to framebuffer
		glFramebufferTexture2D(
			GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, colorBuffers[i], 0
		);
	}
	
}


void Bloom::createAttachBuffer()
{
	unsigned int rboDepth;
	glGenRenderbuffers(1, &rboDepth);
	glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, SCR_WIDTH, SCR_HEIGHT);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth);
	// tell OpenGL which color attachments we'll use (of this framebuffer) for rendering 
	unsigned int attachments[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
	glDrawBuffers(2, attachments);
	// finally check if framebuffer is complete
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "Framebuffer not complete!" << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Bloom::createBlurBuffer()
{

	// ping-pong-framebuffer for blurring
	glGenFramebuffers(2, pingpongFBO);
	glGenTextures(2, pingpongColorbuffers);
	for (unsigned int i = 0; i < 2; i++)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, pingpongFBO[i]);
		glBindTexture(GL_TEXTURE_2D, pingpongColorbuffers[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGB, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // we clamp to the edge as the blur filter would otherwise sample repeated texture values!
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, pingpongColorbuffers[i], 0);
		// also check if framebuffers are complete (no need for depth buffer)
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			std::cout << "Framebuffer not complete!" << std::endl;
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

}

void Bloom::renderWithBlurr(Shader* shaderBlur)
{
	
	// 2. blur bright fragments with two-pass Gaussian Blur 
	// --------------------------------------------------
	horizontal = true;
	first_iteration = true;
	unsigned int amount = 10;
	shaderBlur->Use();
	
	
	glUniform1i(shaderBlur->Uniforms["image"], 0);
	for (unsigned int i = 0; i < amount; i++)
	{
	
		glUniform1i(shaderBlur->Uniforms["horizontal"], horizontal);
		glBindFramebuffer(GL_FRAMEBUFFER, pingpongFBO[horizontal]);
		
		
		glBindTexture(GL_TEXTURE_2D, first_iteration ? colorBuffers[1] : pingpongColorbuffers[!horizontal]);  // bind texture of other framebuffer (or scene if first iteration)
		renderQuad();
		horizontal = !horizontal;
		if (first_iteration)
			first_iteration = false;
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, SCR_WIDTH,SCR_HEIGHT);
}

void Bloom::combineProcess(Shader* shaderBloomFinal)
{	
	// 3. now render floating point color buffer to 2D quad and tonemap HDR colors to default framebuffer's (clamped) color range
	//this->bloom = true;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	shaderBloomFinal->Use();
	
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, colorBuffers[0]);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, pingpongColorbuffers[!horizontal]);

	glUniform1i(shaderBloomFinal->Uniforms["scene"], 0);
	glUniform1i(shaderBloomFinal->Uniforms["bloomBlur"], 1);
	glUniform1i(shaderBloomFinal->Uniforms["bloom"], bloom);
	glUniform1f(shaderBloomFinal->Uniforms["exposure"], exposure);
	renderQuad();
	
	//std::cout << "bloom: " << (bloom ? "on" : "off") << "| exposure: " << exposure << std::endl;
	glUseProgram(0);
	
}


void Bloom::renderQuad()
{
	if (quadVAO == 0)
	{
		float quadVertices[] = {
			// positions        // texture Coords
			-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
			-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
			 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
			 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		};
		// setup plane VAO
		glGenVertexArrays(1, &quadVAO);
		glGenBuffers(1, &quadVBO);
		glBindVertexArray(quadVAO);
		glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	}
	glBindVertexArray(quadVAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
}

void Bloom::activateBloom(bool value)
{
	this->exposure = 1.0f;
	this->bloom = value;
}

void Bloom::increaseExpresure()
{
	exposure += 0.01f;
}

void Bloom::decreaseExpresure()
{
	if (exposure > 0) {
		exposure -= 0.01f;
	}
	
}

void Bloom::setScreenSize(const unsigned int width, const unsigned int height) {
	this->SCR_WIDTH = width;
	this->SCR_HEIGHT = height;
}





