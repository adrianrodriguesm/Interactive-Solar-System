#include "../include/Texture.h"
#include "Vendor/stb_image.h"
#include <iostream>

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

		glGenerateMipmap(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, 0);

		if (m_LocalBuffer) {
			stbi_image_free(m_LocalBuffer);
		}

	}

	Texture::Texture()
		:id(0), width(512), height(512), m_BPP(0), m_FilePath(""), m_LocalBuffer(nullptr)
	{
		
		float* data = new float[width * height];

		m_LocalBuffer = new unsigned char[width * height];
		PerlinNoise pNoise;
		int count = 0;


		// Visit every pixel of the image and assign a color generated with Perlin noise
		for (int i = 0; i < height; ++i) {     
			for (int j = 0; j < width; ++j) { 
				double x = (double)j / ((double)width);
				double y = (double)i / ((double)height);

				// Typical Perlin noise
				
				 unsigned char n = floor(pNoise.noise(x*8, y*10, 0.8) * sin(pNoise.noise(x * 8, y * 10, 0.8)*2) * 255);

				//std::cout << n << std::endl;
				m_LocalBuffer[count] = (n);
				count++;
			}
		}

		glGenTextures(1, &id);
		glBindTexture(GL_TEXTURE_2D, id);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_LocalBuffer);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width/2,height/2, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_LocalBuffer);

		glGenerateMipmap(GL_TEXTURE_2D);


		glBindTexture(GL_TEXTURE_2D, 0);

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