#pragma once
#include "Scenegraph.h"

struct Loader {


	fstream file;
	std::map<std::string, mat4> Matrices;

	Loader();
	float camDist;
	qtrn rot;

	void updateState(SceneGraph* scene, float camDistance, qtrn rot);
	void readState();
	bool is_empty();


private:
	float* insertMatrixValues(std::string text, float data[16], int& index);

	void loadMatrices(std::string tex);

};