/*#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <iostream>
#include "file.h"
#include "vectors.h"
#include "mat4.h"


struct Shader {
	GLuint shader_id;
	const char* pathVertex;
	const char* pathFragment;

	Shader(const char* vert, const char* frag);
	Shader();
	~Shader();

	GLint getUniformLocation(const GLchar* name);
	void addAttribute(const float value, const GLchar* name);
	void setUniform1f(const GLchar* name, const float value);
	void setUniform1i(const GLchar* name, const int value);
	void setUniform2f(const GLchar* name, const vec2& vec);
	void setUniform3f(const GLchar* name, const vec3& vec);
	void setUniform4f(const GLchar* name, const vec4& vec);
	void setUniformMat4(const GLchar* name, const mat4& mat);

	GLuint getProgram();
	void enable() const;
	void disable() const;
	
	GLuint load();

};*/
#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <map>
#include <GL/glew.h>
#include <GLFW/glfw3.h>


class Shader {

private:
	GLuint vs;
	GLuint fs;

	GLuint ProgramId;

	unsigned int GetCompiledShader(unsigned int shader_type, const std::string& shader_source);

public:
	std::map<const GLchar*, GLuint> Uniforms;

	bool TexcoordsLoaded, NormalsLoaded;

	Shader();

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

#endif

