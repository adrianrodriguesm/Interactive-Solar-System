#include "../Include/SceneGraph.h"

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
	this->root->draw(main);		
}


