#include "SceneGraph.h"

SceneGraph::SceneGraph() {
	this->root = new SceneNode();
	this->camera = new Camera();
}

SceneGraph::SceneGraph(Camera& cam) {
	this->root = new SceneNode();
	this->camera = &cam;
}

SceneNode* SceneGraph::createNode() {
	SceneNode* node = root->createNode();
	return node;
}

SceneNode* SceneGraph::getRoot() {
	return this->root;
}

Camera* SceneGraph::getCamera() {
	return this->camera;
}

void SceneGraph::setCamera(Camera& cam) {
	this->camera = &cam;
}

void SceneGraph::draw() {
	for (SceneNode* child : this->root->getChildren()) {
		child->draw(this->camera);
	}
}