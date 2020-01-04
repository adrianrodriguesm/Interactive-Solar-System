#include "Shader.h"

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
private:

	///SCREEN
	const unsigned int SCR_WIDTH = 1920/2;
	const unsigned int SCR_HEIGHT = 1920 / 2;

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
