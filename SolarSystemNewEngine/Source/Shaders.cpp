#include "../Include/Shaders.h"
/*


Shader::Shader(const char* vert, const char* frag) {
	pathVertex = vert;
	pathFragment = frag;
	shader_id = load();
		
}

Shader::~Shader() {
	glDeleteProgram(shader_id);

}

Shader::Shader() {
	pathVertex = "";
	pathFragment = "";
	shader_id = 0;
}


GLuint Shader::load() {
	GLuint program = glCreateProgram();
	const GLchar* VertexShader = ParseShader(pathVertex);
	GLuint VertexShaderId = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(VertexShaderId, 1, &VertexShader, 0);
	glCompileShader(VertexShaderId);

	GLint result;//Int de glew
	glGetShaderiv(VertexShaderId, GL_COMPILE_STATUS, &result);//ver status para ver se existem erros
	if (result == GL_FALSE) {
		GLint length;
		glGetShaderiv(VertexShaderId, GL_INFO_LOG_LENGTH, &length);
		std::vector<char> error(length);
		glGetShaderInfoLog(VertexShaderId, length, &length, &error[0]);
		std::cout << "Failed Vertex shader " << std::endl << &error[0] << std::endl;
		glDeleteShader(VertexShaderId);
		return 0;
	}

	const GLchar* FragmentShader = ParseShader(pathFragment);
	GLuint FragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(FragmentShaderId, 1, &FragmentShader, 0);
	glCompileShader(FragmentShaderId);

		
	glGetShaderiv(FragmentShaderId, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE) {
		GLint length;
		glGetShaderiv(FragmentShaderId, GL_INFO_LOG_LENGTH, &length);
		std::vector<char> error(length);
		glGetShaderInfoLog(FragmentShaderId, length, &length, &error[0]);
		std::cout <<"Failed Fragment shader " <<  std::endl << &error[0] << std::endl;
		glDeleteShader(FragmentShaderId);
		return 0;
	}
		
	glAttachShader(program, VertexShaderId);
	glAttachShader(program, FragmentShaderId);

	glLinkProgram(program);
	glValidateProgram(program);


	glDeleteShader(VertexShaderId);

	glDeleteShader(FragmentShaderId);
	return program;

}

GLuint Shader::getProgram() {
	return shader_id;
}

void Shader::enable() const {
	glUseProgram(shader_id);
}

void Shader::disable() const {
	glUseProgram(0);
}

GLint Shader::getUniformLocation(const GLchar* name) {
	return glGetUniformLocation(shader_id, name);
}
void Shader::addAttribute(const float value, const GLchar* name) {
	glBindAttribLocation(shader_id, (GLint)value, name);
	glLinkProgram(shader_id);
}
void Shader::setUniform1f(const GLchar* name, const float value) {
	glUniform1f(getUniformLocation(name), value);
}


void Shader::setUniform1i(const GLchar* name,const int value) {
	glUniform1i(getUniformLocation(name), value);
}

void Shader::setUniform2f(const GLchar* name, const vec2& vec) {
	glUniform2f(getUniformLocation(name), vec.x, vec.y);
}

void Shader::setUniform3f(const GLchar* name, const vec3& vec) {
	glUniform3f(getUniformLocation(name), vec.x, vec.y, vec.z);
}

void Shader::setUniform4f(const GLchar* name, const vec4& vec) {
	glUniform4f(getUniformLocation(name), vec.x, vec.y, vec.z, vec.w);
}

void  Shader::setUniformMat4(const GLchar* name, const mat4& mat) {
	glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, mat.elements);
}*/

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
		while (std::getline(is_fs, line)) {
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


	
	
