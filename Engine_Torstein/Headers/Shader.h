#ifndef SHADERUTIL_H
#define SHADERUTIL_H

#include <string>

class Shader {

private:
	unsigned int mProgramId;

	unsigned int GetCompiledShader(unsigned int shader_type, const std::string& shader_source);

public:
	Shader();
	~Shader() {}

	unsigned int PositionUniformId;
	unsigned int ColorUniformId;
	unsigned int ModelMatrix_UId;
	unsigned int ViewMatrix_UId;
	unsigned int ProjectionMatrix_UId;

	bool TexcoordsLoaded, NormalsLoaded;

	// Load a vertex and a fragment shader from file
	bool Load(const std::string& vertexShaderFile, const std::string& fragmentShaderFile);

	// Use the program
	void Use();

	// Delete the program
	void Delete();

};


#endif
