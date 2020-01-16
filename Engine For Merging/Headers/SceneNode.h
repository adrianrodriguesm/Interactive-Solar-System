#pragma once
#include "Mesh.h"
#include "Matrices.h"
#include "Shader.h"
#include "Vectors.h"
#include "Camera.h"
#include "quaternions.h"
#include "Texture.h"

class SceneNode
{
private:
	SceneNode* parent;
	std::vector<SceneNode*> children;
	Mesh* mesh;
	Texture* texture;
	Shader* shader;
	vec4 color;
	std::string name;

	//The Scene Node has a seperate Scale and model matrix!!!!!
	mat4 scaleMatrix;
	mat4 modelMatrix;
	//


public:

	SceneNode();
	SceneNode* createNode();
	void draw(Camera* cam);

	void setMesh(Mesh* m);
	void setTexture(Texture* t);
	void setShader(Shader* s);
	void setName(std::string name);

	void setScaleMatrix(mat4 M);	//Set the scale matrix
	void setMatrix(mat4 m);			//Set the model matrix
	mat4 getMatrix();

	void setParent(SceneNode* parent);
	void addChild(SceneNode* child);

	std::vector<SceneNode*> getChildren();
};

