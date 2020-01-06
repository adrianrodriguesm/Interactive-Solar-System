#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <sstream>
#include <fstream>
#include "Shaders.h"
#include "Texture.h"
#include "mat4.h"

#define VERTICES 0
#define TEXCOORDS 1
#define NORMALS 2



struct Mesh {
	Mesh() = default;
	Texture* tex;
	typedef struct {
		GLfloat x, y, z;
	} Vertex;

	typedef struct {
		GLfloat u, v;
	} Texcoord;

	typedef struct {
		GLfloat nx, ny, nz;
	} Normal;

	bool TexcoordsLoaded = false, NormalsLoaded = false;
	GLuint VaoId;
	std::vector <Vertex> Vertices, vertexData;
	std::vector <Texcoord> Texcoords, texcoordData;
	std::vector <Normal> Normals, normalData;
	std::vector <unsigned int> vertexIdx, texcoordIdx, normalIdx;
		

	const void createMesh(std::string& filename);
	void parseVertex(std::stringstream& sin);
	void parseTexcoord(std::stringstream& sin);
	void parseNormal(std::stringstream& sin);
	void parseFace(std::stringstream& sin);
	void parseLine(std::stringstream& sin);
	void loadMeshData(std::string& filename);
	void processMeshData();
	void freeMeshData();
	void draw(Shader* shader,  vec4& color, mat4& m);

	void setTexture(Texture* tex);

	void createBufferObjects();
	void destroyBufferObjects();

	bool getTexLoaded();
	bool getNormalLoaded();
		

};

