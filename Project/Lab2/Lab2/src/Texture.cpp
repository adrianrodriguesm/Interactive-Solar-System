#include "../include/Texture.h"
#include "Vendor/stb_image.h"

namespace EngineTexture {

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

}