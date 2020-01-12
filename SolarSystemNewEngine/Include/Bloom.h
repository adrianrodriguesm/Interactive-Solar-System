//#include "Shaders.h"
//#include "Texture.h"
#include "SceneGraph.h"


struct Bloom {
	Bloom();
	void bindHDRBuffer();
	void createBrightFilterBuffer();

	void createAttachBuffer();
	void createBlurBuffer();
	void renderWithBlurr(Shader* shaderBlur);
	void combineProcess(Shader* shaderBloomFinal);
	void renderQuad();
	void activateBloom(bool value);
	void increaseExpresure();
	void decreaseExpresure();
	void setScreenSize(const unsigned int width, const unsigned int height);
	void setTex(Texture* tex);
private:
	Texture* tex;
	///SCREEN
	unsigned int SCR_WIDTH;
	unsigned int SCR_HEIGHT;

	///BUFFERS
	unsigned int hdrFBO;
	unsigned int colorBuffers[2];
	unsigned int pingpongFBO[2];
	unsigned int pingpongColorbuffers[2];

	bool horizontal, first_iteration;
	bool bloom;
	float exposure = 1.0f;

	///RENDER QUAD
	unsigned int quadVAO;
	unsigned int quadVBO;

};
