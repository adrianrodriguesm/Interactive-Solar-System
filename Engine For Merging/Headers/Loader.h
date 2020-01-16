#pragma once
#include "Scenegraph.h"

struct Loader {
	SceneGraph* scene;
	Loader(SceneGraph* scene);
	std::string file;

	void updateState();
};