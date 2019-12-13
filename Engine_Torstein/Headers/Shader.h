#include <string>
#include <map>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Shader {

private:
	GLuint vs, fs;

	GLuint ProgramId;

	unsigned int GetCompiledShader(unsigned int shader_type, const std::string& shader_source);

public:
	Shader();
	~Shader() {}

	std::map<const GLchar*, GLuint> Uniforms;

	unsigned int PositionUniformId;
	unsigned int ColorUniformId;
	unsigned int ModelMatrix_UId;
	unsigned int ViewMatrix_UId;
	unsigned int ProjectionMatrix_UId;

	bool TexcoordsLoaded, NormalsLoaded;

	// Load a vertex and a fragment shader from file
	void Load(const std::string& vertexShaderFile, const std::string& fragmentShaderFile);

	void AddAttribute(const GLuint index, const GLchar* name);
	void AddUniform(const GLchar* name);

	bool Create();

	// Use the program
	void Use();

	// Delete the program
	void Delete();

};
