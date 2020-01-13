#ifndef TEXTURE_H
#define TEXTURE_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include <vector>
#include "stb_image.h"

struct Texture {
	
	Texture();
	Texture(const std::string& m_FilePath);
	Texture(std::vector<std::string> faces); //For skybox / cubemap
	~Texture();		

	void Bind(unsigned int slot = 0) const;
	void Unbind() const;

	inline int GetId() const { return id; }
	inline int GetWidth() const { return width; }
	inline int GetHeight() const { return height; }

private:
	GLuint id;
	int width, height, m_BPP;
	std::string m_FilePath;
	unsigned char* m_LocalBuffer;

};

#endif