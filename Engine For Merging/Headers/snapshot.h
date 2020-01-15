#pragma once
#include <stdlib.h>
#include <iostream>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "stb_image_write.h"

struct Snapshot
{
	Snapshot();

	void flipVertically(int width, int height, char* data);

	int saveSnapshot(const char* filename);

	const char* createSnapshotBasename();

	int captureSnapshot();
};
