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

	//mat4 translationMatrix;
	mat4 scaleMatrix;
	//qtrn quaternion;
	mat4 modelMatrix;

	Texture* texture;
	Shader* shader;
	vec4 color;

public:

	SceneNode();
	SceneNode* createNode();
	void draw(Camera* cam);

	void setMesh(Mesh* m);

	//void setTranslationMatrix(mat4 M);
	void setScaleMatrix(mat4 M);
	//void setQuaternion(qtrn q);
	void setMatrix(mat4 m);

	void setTexture(Texture* t);
	void setShader(Shader* s);
	void setColor(vec4 c);
	void setParent(SceneNode* parent);
	void addChild(SceneNode* child);

	std::vector<SceneNode*> getChildren();
};

