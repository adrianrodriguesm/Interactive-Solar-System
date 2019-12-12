#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <GLFW/glfw3.h>

#define VERTICES 0
#define TEXCOORDS 1
#define NORMALS 2

class Texture
{
private:
	GLuint id;
	int weight;
	int height;

public:

	Texture();
};

