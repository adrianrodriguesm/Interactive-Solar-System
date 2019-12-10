#include <Shader.h>

#include <GL/glew.h>
#include <iostream>
#include <fstream>

Shader::Shader() {
	mProgramId = 0;
	PositionUniformId = 0;
	ColorUniformId = 0;
	ProjectionMatrix_UId = 0;
	ModelMatrix_UId = 0;
	ViewMatrix_UId = 0;
	NormalsLoaded = TexcoordsLoaded = false;
}

unsigned int Shader::GetCompiledShader(unsigned int shader_type, const std::string& shader_source)
{
	unsigned int shader_id = glCreateShader(shader_type);

	const char* c_source = shader_source.c_str();
	glShaderSource(shader_id, 1, &c_source, nullptr);
	glCompileShader(shader_id);

	GLint result;
	glGetShaderiv(shader_id, GL_COMPILE_STATUS, &result);

	if (result == GL_FALSE)
	{
		int length;
		glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &length);

		GLchar* strInfoLog = new GLchar[length + 1];
		glGetShaderInfoLog(shader_id, length, &length, strInfoLog);

		fprintf(stderr, "Compilation error in shader: %s\n", strInfoLog);
		delete[] strInfoLog;
	}

	return shader_id;
}

bool Shader::Load(const std::string& vertexShaderFile, const std::string& fragmentShaderFile)
{
	std::string f_fs;
	std::string f_vs;
	std::string line;

	std::ifstream is_vs(".\\..\\..\\Shaders\\" + vertexShaderFile);
	if (is_vs.is_open()) {
		while (std::getline(is_vs, line)) {
			f_vs += line + "\n";
		}
	}
	else std::cout << "Error: Shader file '" << vertexShaderFile << "'could not be found." << std::endl;

	std::ifstream is_fs(".\\..\\..\\Shaders\\" + fragmentShaderFile);
	if (is_fs.is_open()) {
		while (std::getline(is_fs,line)) {
			f_fs += line + "\n";
		}
	}
	else std::cout << "Error: Shader file '" << fragmentShaderFile << "'could not be found." << std::endl;
	
	mProgramId = glCreateProgram();

	unsigned int vs = GetCompiledShader(GL_VERTEX_SHADER, f_vs);
	unsigned int fs = GetCompiledShader(GL_FRAGMENT_SHADER, f_fs);

	glAttachShader(mProgramId, vs);
	glAttachShader(mProgramId, fs);

	glBindAttribLocation(mProgramId, 0, "inPosition");
	if (TexcoordsLoaded)
		glBindAttribLocation(mProgramId, 1, "inTexcoord");
	if (NormalsLoaded)
		glBindAttribLocation(mProgramId, 2, "inNormal");
	
	glLinkProgram(mProgramId);

	ColorUniformId = glGetUniformLocation(mProgramId, "colorOverride");
	ModelMatrix_UId = glGetUniformLocation(mProgramId, "ModelMatrix");
	ViewMatrix_UId = glGetUniformLocation(mProgramId, "ViewMatrix");
	ProjectionMatrix_UId = glGetUniformLocation(mProgramId, "ProjectionMatrix");

	glValidateProgram(mProgramId);

	glDetachShader(mProgramId, vs);
	glDetachShader(mProgramId, fs);

	glDeleteShader(vs);
	glDeleteShader(fs);

	return true;
}

void Shader::Use()
{
	glUseProgram(mProgramId);
}

void Shader::Delete()
{
	glDeleteProgram(mProgramId);
}

