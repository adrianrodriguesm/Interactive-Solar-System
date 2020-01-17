#pragma once
#include "Scenegraph.h"

struct Loader {


	fstream file;
	std::map<std::string, mat4> Matrices;

	Loader();
	float camDist;
	qtrn rot;
	mat4 camTargetRot; mat4 camTargetTrans; int stencilId;

	void updateState(SceneGraph* scene, float camDistance, qtrn rot, mat4 camTargetRot, mat4 camTargetTrans, int stencilId);
	void readState();
	bool is_empty();


private:
	float* insertMatrixValues(std::string text, float data[16], int& index);

	void loadMatrices(std::string tex);

};