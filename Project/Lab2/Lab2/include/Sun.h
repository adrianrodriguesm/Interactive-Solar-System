#include "../include/Shaders.h"
#include "../include/SceneGraph.h"
using namespace EngineSceneGraph;
struct Sun {

	const unsigned int SCR_WIDTH = 1280;
	const unsigned int SCR_HEIGHT = 720;
	unsigned int colorBuffers[2];
	//Blurr
		
	unsigned int pingpongFBO[2];
	unsigned int pingpongColorbuffers[2];

	bool bloom = true;
	float exposure = 1.0f;

	Sun();
	void createBrightFilterBuffer();

	void createDepthBuffer();
	void createBlurBuffer();
	void renderWithBlurr(Shader* shaderBlur,  SceneGraph* scene);
	//void createBlurBuffer(Shader* shaderBlur, Shader* shaderBloomFinal, SceneGraph* scene);
	

};
