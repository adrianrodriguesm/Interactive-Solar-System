#include "Shader.h"

#include <GL/glew.h>
#include <iostream>
#include <fstream>

Shader::Shader() {
	vs = fs = 0;
	ProgramId = 0;
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

void Shader::Load(const std::string& vertexShaderFile, const std::string& fragmentShaderFile)
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
	
	ProgramId = glCreateProgram();

	vs = GetCompiledShader(GL_VERTEX_SHADER, f_vs);
	fs = GetCompiledShader(GL_FRAGMENT_SHADER, f_fs);

	glAttachShader(ProgramId, vs);
	glAttachShader(ProgramId, fs);

	//glBindAttribLocation(ProgramId, 0, "inPosition");
	//if (TexcoordsLoaded)
	//	glBindAttribLocation(ProgramId, 1, "inTexcoord");
	//if (NormalsLoaded)
	//	glBindAttribLocation(ProgramId, 2, "inNormal");
	//
	//glLinkProgram(ProgramId);

	//ColorUniformId = glGetUniformLocation(ProgramId, "colorOverride");
	//ModelMatrix_UId = glGetUniformLocation(ProgramId, "ModelMatrix");
	//ViewMatrix_UId = glGetUniformLocation(ProgramId, "ViewMatrix");
	//ProjectionMatrix_UId = glGetUniformLocation(ProgramId, "ProjectionMatrix");

	//glValidateProgram(ProgramId);

	//glDetachShader(ProgramId, vs);
	//glDetachShader(ProgramId, fs);

	//glDeleteShader(vs);
	//glDeleteShader(fs);

	//return true;
}

void Shader::AddAttribute(const GLuint index, const GLchar* name) {
	glBindAttribLocation(this->ProgramId, index, name);
}

void Shader::AddUniform(const GLchar* name) 
{
	GLint linkStatus;
	glGetProgramiv(ProgramId, GL_LINK_STATUS, &linkStatus);
	if (linkStatus == GL_FALSE) glLinkProgram(ProgramId);

	this->Uniforms[name] = glGetUniformLocation(this->ProgramId, name);
}

bool Shader::Create() {
	glValidateProgram(ProgramId);

	glDetachShader(ProgramId, vs);
	glDetachShader(ProgramId, fs);

	glDeleteShader(vs);
	glDeleteShader(fs);

	return true;
}

void Shader::Use()
{
	glUseProgram(ProgramId);
}

void Shader::Delete()
{
	glDeleteProgram(ProgramId);
}

