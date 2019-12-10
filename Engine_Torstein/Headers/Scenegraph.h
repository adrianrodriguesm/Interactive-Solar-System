#pragma once
#include "SceneNode.h"
#include "Matrices.h"
#include "Shader.h"

class SceneGraph
{
private:
	SceneNode* root;
	Camera* camera;

public:
	SceneGraph();
	SceneGraph(Camera& cam);
	SceneNode* createNode();
	SceneNode* getRoot();
	Camera* getCamera();
	void setCamera(Camera& cam);
	void draw();
};

