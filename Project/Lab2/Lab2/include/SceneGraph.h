#pragma once
#include "SceneNode.h"
#include "Camera.h"


using namespace EngineCamera;
using namespace EngineSceneNode;

namespace EngineSceneGraph {
	struct SceneGraph {
		Camera* main;
		SceneNode* root;


		SceneGraph();
		SceneGraph(Camera* main, SceneNode* root);

		void setCamera(Camera* main);
		void setRoot(SceneNode* root);
		void draw();
		
	};

}
