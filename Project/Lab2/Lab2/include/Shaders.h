#pragma once
#include "../include/file.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <iostream>
#include "../include/vectors.h"
#include "../include/mat4.h"
using  namespace mathsMat4;

namespace Shaders {
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

	};
}
