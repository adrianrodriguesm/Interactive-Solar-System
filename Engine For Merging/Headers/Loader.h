#pragma once
#include "Scenegraph.h"

struct Loader {
	SceneGraph* scene;
	Loader(SceneGraph* scene);
	fstream file;

	void updateState();
	void readState();
};