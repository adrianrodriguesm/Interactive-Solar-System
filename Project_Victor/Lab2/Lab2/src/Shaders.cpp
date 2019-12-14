#include "../include/Shaders.h"

namespace Shaders {

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
	}

	
	
}