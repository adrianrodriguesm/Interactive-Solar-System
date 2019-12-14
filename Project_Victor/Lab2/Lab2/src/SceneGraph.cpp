#include "../include/SceneGraph.h"


namespace EngineSceneGraph {

	SceneGraph::SceneGraph() {
		this->main = nullptr;
		this->root = nullptr;
		

	}
	SceneGraph::SceneGraph(Camera* main, SceneNode* root) {
		this->main = main;
		this->root = root;
	}

	void SceneGraph::setCamera(Camera* main) {
		this->main = main;
	}
	void SceneGraph::setRoot(SceneNode* root) {

		this->root = root;
	}
	void SceneGraph::draw() {
		Shader* shader = this->root->getShader();
		shader->setUniformMat4("ProjectionMatrix", main->projMatrix);
		shader->setUniformMat4("ViewMatrix", main->viewMatrix);
		this->root->draw();
	}


}