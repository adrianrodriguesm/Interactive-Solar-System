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
	Shader* shader = this->root->getShader();
	shader->Use();
	glUniformMatrix4fv(shader->Uniforms["ViewMatrix"], 1, GL_FALSE, main->viewMatrix.elements);
	glUniformMatrix4fv(shader->Uniforms["ProjectionMatrix"], 1, GL_FALSE, main->projMatrix.elements);
	this->root->draw();
		
}


