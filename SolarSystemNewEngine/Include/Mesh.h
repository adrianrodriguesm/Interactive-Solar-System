
#include "Shaders.h"
#include "Texture.h"
#include "mat4.h"
#include "Camera.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <GL\glew.h>


#define VERTICES 0
#define TEXCOORDS 1
#define NORMALS 2

class Mesh
{
private:
	void parseVertex(std::stringstream& sin);

	void parseTexcoord(std::stringstream& sin);

	void parseNormal(std::stringstream& sin);

	void parseFace(std::stringstream& sin);

	void parseLine(std::stringstream& sin);

	void loadMeshData(std::string& filename);

	void processMeshData();

	void freeMeshData();

	void createBufferObjects(Mesh& mesh);

public:

	bool NormalsLoaded, TexcoordsLoaded;

	Mesh();

	Mesh(std::string& filename);

	GLuint VaoId, verticesCount;

	Texture* tex;

	Mesh meshFromObj(std::string& filename);

	void destroy();

	const void createMesh(std::string& filename);

	void bindVaoId();

	void draw();

	void draw(Shader* shader, Camera* cam, vec4& color, mat4& m);

	void setTexture(Texture* tex);
};



