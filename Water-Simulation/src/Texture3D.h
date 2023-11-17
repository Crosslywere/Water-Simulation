#pragma once

#include <glad/glad.h>
#include <stb_image.h>
#include <iostream>
#include <array>

enum Texture3DSide
{
	Right = 0, Left,
	Top, Bottom,
	Front, Back
};

struct TextureFile
{
	std::string filePath = "";
	Texture3DSide side = Texture3DSide::Right;
};

class Texture3D
{
public:
	Texture3D(const TextureFile& side1, const TextureFile& side2, const TextureFile& side3,const TextureFile& side4,const TextureFile& side5,const TextureFile& side6)
		: m_TextureID(0), m_Width(0), m_Height(0)
	{
		glGenTextures(1, &m_TextureID);
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_TextureID);
		std::array<TextureFile, 6> files = { side1, side2, side3, side4, side5, side6 };
		int bpc;
		for (const auto& file : files)
		{
			unsigned char* data = stbi_load(file.filePath.c_str(), &m_Width, &m_Height, &bpc, 0);
			if (data)
			{
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + file.side, 0, GL_RGB8, m_Width, m_Height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			}
			else
			{
				std::cout << "Failed to load image resource '" << file.filePath << '\'' << std::endl;
			}
			stbi_image_free(data);
		}
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	}

	~Texture3D()
	{
		glDeleteTextures(1, &m_TextureID);
	}

	void Bind(unsigned int slot = 0)
	{
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_TextureID);
	}

	unsigned int GetID() const
	{
		return m_TextureID;
	}

private:
	unsigned int m_TextureID;
	int m_Width, m_Height;
};