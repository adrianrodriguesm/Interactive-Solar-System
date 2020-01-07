#include "../Include/SceneNode.h"


SceneNode::SceneNode() {
	localMatrix = matFactory::createIdentityMat4();
	worldMatrix = matFactory::createIdentityMat4();
		
	parent = nullptr;
	shader = nullptr;
	mesh = nullptr;
	childNodes;

	scale = matFactory::createIdentityMat4();
	state;
	stateI;
	stateF;

		
		
}

SceneNode* SceneNode::createNode() {
	SceneNode* child = new SceneNode();
	child->setParent(this);
	this->childNodes.push_back(child);
	return child;
}

////Setters
void SceneNode::setMesh(Mesh* mesh) {
	this->mesh = mesh;
}
void SceneNode::setParent(SceneNode* parent) {
	this->parent = parent;
}
void SceneNode::setLocalMatrix(const mat4& m) {
	this->localMatrix = m;
}
	
void SceneNode::setShaderProgram(Shader* s) {
	this->shader = s;
}
void SceneNode::setColor(const vec4& color) {
	this->color = color;
}

void SceneNode::setTrans(const mat4& m) {
	this->state.pos = m;
	this->state.pos =  m;
		
}
void SceneNode::updateTrans(const mat4& m) {

	this->state.pos = this->state.pos+ m;

}
void SceneNode::setRot(const qtrn& q) {
	this->state.rot = q;
}

void SceneNode::setScale(const mat4& s) {
	this->scale = s;
}

void  SceneNode::draw(Camera* cam) {

	SceneNode* currParent = this->parent;
	mat4 rot;
	if (currParent != nullptr) {
		qGLMatrix(state.rot, rot);
		localMatrix = state.pos * rot * scale;
		worldMatrix =  parent->worldMatrix * localMatrix;
			
	}
	else {
		qGLMatrix(state.rot, rot);
			
		localMatrix = state.pos*  rot * scale;
		worldMatrix = localMatrix;
			
	}
	Shader* currShader = this->getShader();
	if (mesh != nullptr && currShader != nullptr) {
		mesh->draw(currShader,cam ,color, worldMatrix);
	}
	for (SceneNode* node : childNodes) {
		
		node->draw(cam);
	}
	
}
void SceneNode::setInitialState(State& state) {
		
	this->stateI = state;
}

void SceneNode::setFinalState(State& state) {
	this->stateF = state;
}

Shader* SceneNode::getShader() {
	SceneNode* currParent = this->parent;
	Shader* currShader = this->shader;
	while (currShader == nullptr) {
		currShader = currParent->shader;
		currParent = currParent->parent;
	}
	return currShader;
}
void SceneNode::playAnimation(float duration,  float curr_time) {
			
		mat4 pos = (curr_time / duration) * (stateF.pos - state.pos);
		updateTrans(pos);
		qtrn rot = qLerp(state.rot, stateF.rot, (curr_time/ duration));
		setRot(rot);
			
		

}

void SceneNode::switchState() {
	State i = this->stateI;
	State f = this->stateF;
	this->stateF = i;
	this->stateI = f;
	for (SceneNode* node : childNodes) {
		node->switchState();
	}
}


