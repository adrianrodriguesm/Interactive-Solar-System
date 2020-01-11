#include "../Include/Mesh.h"

typedef struct {
	float x, y, z;
} Vertex;

typedef struct {
	float u, v;
} Texcoord;

typedef struct {
	float nx, ny, nz;
} Normal;

std::vector <Vertex> Vertices, vertexData;
std::vector <Texcoord> Texcoords, texcoordData;
std::vector <Normal> Normals, normalData;

std::vector <unsigned int> vertexIdx, texcoordIdx, normalIdx;

Mesh::Mesh() {
	TexcoordsLoaded = false;
	NormalsLoaded = false;
	VaoId = 0;
	verticesCount = 0;
}

Mesh::Mesh(std::string& filename) {
	this->createMesh(filename);
	createBufferObjects(*this);
	freeMeshData();
}

void Mesh::parseVertex(std::stringstream& sin)
{
	Vertex v;
	sin >> v.x >> v.y >> v.z;
	vertexData.push_back(v);
}

void Mesh::parseTexcoord(std::stringstream& sin)
{
	Texcoord t;
	sin >> t.u >> t.v;
	texcoordData.push_back(t);
}

void Mesh::parseNormal(std::stringstream& sin)
{
	Normal n;
	sin >> n.nx >> n.ny >> n.nz;
	normalData.push_back(n);
}

void Mesh::parseFace(std::stringstream& sin)
{
	std::string token;
	if (normalData.empty() && texcoordData.empty())
	{
		for (int i = 0; i < 3; i++)
		{
			sin >> token;
			vertexIdx.push_back(std::stoi(token));
		}
	}
	else
	{
		for (int i = 0; i < 3; i++)
		{
			std::getline(sin, token, '/');
			if (token.size() > 0) vertexIdx.push_back(std::stoi(token));
			std::getline(sin, token, '/');
			if (token.size() > 0) texcoordIdx.push_back(std::stoi(token));
			std::getline(sin, token, ' ');
			if (token.size() > 0) normalIdx.push_back(std::stoi(token));
		}
	}
}

void Mesh::parseLine(std::stringstream& sin)
{
	std::string s;
	sin >> s;
	if (s.compare("v") == 0) parseVertex(sin);
	else if (s.compare("vt") == 0) parseTexcoord(sin);
	else if (s.compare("vn") == 0) parseNormal(sin);
	else if (s.compare("f") == 0) parseFace(sin);
}

void Mesh::loadMeshData(std::string& filename)
{
	std::ifstream ifile(filename);
	std::string line;
	while (std::getline(ifile, line)) {
		std::stringstream sline(line);
		parseLine(sline);
	}
	TexcoordsLoaded = (texcoordIdx.size() > 0);
	NormalsLoaded = (normalIdx.size() > 0);
}

void Mesh::processMeshData()
{
	for (unsigned int i = 0; i < vertexIdx.size(); i++) {
		unsigned int vi = vertexIdx[i];
		Vertex v = vertexData[vi - 1];
		Vertices.push_back(v);
		if (TexcoordsLoaded) {
			unsigned int ti = texcoordIdx[i];
			Texcoord t = texcoordData[ti - 1];
			Texcoords.push_back(t);
		}
		if (NormalsLoaded) {
			unsigned int ni = normalIdx[i];
			Normal n = normalData[ni - 1];
			Normals.push_back(n);
		}
	}
}

void Mesh::freeMeshData()
{
	vertexData.clear();
	texcoordData.clear();
	normalData.clear();
	Vertices.clear();
	Normals.clear();
	Texcoords.clear();
	vertexIdx.clear();
	texcoordIdx.clear();
	normalIdx.clear();
}

const void Mesh::createMesh(std::string& filename)
{
	loadMeshData(filename);
	processMeshData();
	this->verticesCount = Vertices.size();
}

Mesh Mesh::meshFromObj(std::string& filename) {
	Mesh newMesh = Mesh();
	newMesh.createMesh(filename);
	createBufferObjects(newMesh);
	freeMeshData();
	return newMesh;
}

void Mesh::createBufferObjects(Mesh& mesh) {
	GLuint VboVertices, VboTexcoords, VboNormals;

	glGenVertexArrays(1, &mesh.VaoId);
	glBindVertexArray(mesh.VaoId);
	{
		glGenBuffers(1, &VboVertices);
		glBindBuffer(GL_ARRAY_BUFFER, VboVertices);
		glBufferData(GL_ARRAY_BUFFER, Vertices.size() * sizeof(Vertex), &Vertices[0], GL_STATIC_DRAW);
		glEnableVertexAttribArray(VERTICES);
		glVertexAttribPointer(VERTICES, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);

		if (TexcoordsLoaded)
		{
			glGenBuffers(1, &VboTexcoords);
			glBindBuffer(GL_ARRAY_BUFFER, VboTexcoords);
			glBufferData(GL_ARRAY_BUFFER, Texcoords.size() * sizeof(Texcoord), &Texcoords[0], GL_STATIC_DRAW);
			glEnableVertexAttribArray(TEXCOORDS);
			glVertexAttribPointer(TEXCOORDS, 2, GL_FLOAT, GL_FALSE, sizeof(Texcoord), 0);
		}
		if (NormalsLoaded)
		{
			glGenBuffers(1, &VboNormals);
			glBindBuffer(GL_ARRAY_BUFFER, VboNormals);
			glBufferData(GL_ARRAY_BUFFER, Normals.size() * sizeof(Normal), &Normals[0], GL_STATIC_DRAW);
			glEnableVertexAttribArray(NORMALS);
			glVertexAttribPointer(NORMALS, 3, GL_FLOAT, GL_FALSE, sizeof(Normal), 0);
		}
	}
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDeleteBuffers(1, &VboVertices);
	glDeleteBuffers(1, &VboTexcoords);
	glDeleteBuffers(1, &VboNormals);
}

void destroyBufferObjects(GLuint& vaoid)
{
	glBindVertexArray(vaoid);
	glDisableVertexAttribArray(VERTICES);
	glDisableVertexAttribArray(TEXCOORDS);
	glDisableVertexAttribArray(NORMALS);
	glDeleteVertexArrays(1, &vaoid);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Mesh::destroy() {
	freeMeshData();
	destroyBufferObjects(this->VaoId);
}

void Mesh::bindVaoId() {
	glBindVertexArray(this->VaoId);
}

/*void Mesh::draw() {
	glDrawArrays(GL_TRIANGLES, 0, (GLsizei)this->verticesCount);
}*/

void Mesh::draw(Shader* shader, Camera* main, vec4& color, mat4& m) {
	glBindVertexArray(this->VaoId);

	shader->Use();
	glUniformMatrix4fv(shader->Uniforms["ViewMatrix"], 1, GL_FALSE, main->viewMatrix.elements);
	glUniformMatrix4fv(shader->Uniforms["ProjectionMatrix"], 1, GL_FALSE, main->projMatrix.elements);
	glUniformMatrix4fv(shader->Uniforms["ModelMatrix"], 1, GL_FALSE, m.elements);
	if (tex != nullptr) {
		/*tex->Bind(0);
		glUniform1i(shader->Uniforms["u_Texture"], 0);*/

	}
	glDrawArrays(GL_TRIANGLES, 0, (GLsizei)this->verticesCount);
	glBindVertexArray(0);
	glUseProgram(0);
}

void Mesh::setTexture(Texture* texture) {
	this->tex = texture;
}

