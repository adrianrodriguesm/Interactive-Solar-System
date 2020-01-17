#include "SceneNode.h"

SceneNode::SceneNode() {
	parent = nullptr;
	children = std::vector<SceneNode*>();
	modelMatrix= mat4(1);
	scaleMatrix = mat4(1);
	worldPosition = vec4(0);
}

SceneNode* SceneNode::createNode() {
	SceneNode* node = new SceneNode();
	node->setParent(this);
	this->children.push_back(node);
	return node;
}

void SceneNode::draw(Camera* cam) {

	if (this->mesh != nullptr) {

		this->mesh->bindVaoId();

		while (shader == nullptr) {
			if (parent != nullptr) shader = parent->shader;
			else {
				cerr << "No shader was found for the scene node" << endl;
				return;
			}
		}
		shader->Use();

		//if (this->texture != nullptr) this->texture->Bind();

		mat4 totalMatrix = parent->modelMatrix * this->modelMatrix * this->scaleMatrix;
		this->worldPosition = totalMatrix * vec4(0,0,0,1);
		glUniformMatrix4fv(shader->Uniforms["ModelMatrix"], 1, GL_TRUE, totalMatrix.data);
		glUniformMatrix4fv(shader->Uniforms["ViewMatrix"], 1, GL_TRUE, cam->ViewMatrix.data);
		glUniformMatrix4fv(shader->Uniforms["ProjectionMatrix"], 1, GL_TRUE, cam->ProjectionMatrix.data);
		
		this->mesh->draw();

		glUseProgram(0);
		glBindVertexArray(0);
	}

	for (SceneNode* child : children) {
		child->draw(cam);
	}
}

void SceneNode::setMesh(Mesh* m) {
	this->mesh = m;
}

void SceneNode::setMatrix(mat4 M) {
	this->modelMatrix = M;
}

mat4 SceneNode::getMatrix() {
	return this->modelMatrix;
}

void SceneNode::setScaleMatrix(mat4 M) {
	this->scaleMatrix = M;
}

void SceneNode::setTexture(Texture* t) {
	this->texture= t;
}

void SceneNode::setShader(Shader* s) {
	this->shader = s;
}

void SceneNode::setName(std::string name)
{
	this->name = name;
}

void SceneNode::setParent(SceneNode* parent) {
	this->parent = parent;
}

void SceneNode::addChild(SceneNode* child) {
	this->children.push_back(child);
}

std::vector<SceneNode*> SceneNode::getChildren() {
	return this->children;
}