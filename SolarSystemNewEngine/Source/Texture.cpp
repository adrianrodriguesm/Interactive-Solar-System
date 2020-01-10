#include "../Include/Texture.h"
#include "../Include/stb_image.h"
#include <iostream>

Texture::Texture(const std::string& m_FilePath)
	:id(0), width(0), height(0), m_BPP(0), m_FilePath(m_FilePath), m_LocalBuffer(nullptr)
{
	stbi_set_flip_vertically_on_load(1);//Flip the texture make upsidedown
	m_LocalBuffer = stbi_load(m_FilePath.c_str(), &width, &height, &m_BPP, 4);//4 because we use RGB
		

	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_LocalBuffer);

	glBindTexture(GL_TEXTURE_2D, 0);

	if (m_LocalBuffer) {
		stbi_image_free(m_LocalBuffer);
	}

}

Texture::Texture(std::vector<std::string> faces)
	:id(0), width(0), height(0), m_BPP(0), m_FilePath(m_FilePath), m_LocalBuffer(nullptr)
{
	//stbi_set_flip_vertically_on_load(1);//Flip the texture make upsidedown

	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);

	for (unsigned int i = 0; i < faces.size(); i++) 
	{
		m_LocalBuffer = stbi_load(faces[i].c_str(), &width, &height, &m_BPP, 0);//4 because we use RGB
		if (m_LocalBuffer) {
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, m_LocalBuffer);
			stbi_image_free(m_LocalBuffer);
		}
		else {
			std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
			stbi_image_free(m_LocalBuffer);
		}

	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}

Texture::~Texture() {
	glDeleteTextures(1, &id);
}

void Texture::Bind(unsigned int slot ) const {
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, id);
}
void Texture::Unbind() const {
	glBindTexture(GL_TEXTURE_2D, 0);
}

