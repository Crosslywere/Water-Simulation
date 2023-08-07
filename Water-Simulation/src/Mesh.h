#pragma once
#ifndef MESH_H
#define MESH_H

#include <glad/glad.h>
#include <vector>
#include <fstream>
#include <sstream>
#include <glm/glm.hpp>
#include "Shader.h"

enum class MeshUsage
{
	Stream = GL_STREAM_DRAW,
	Static = GL_STATIC_DRAW,
	Dynameic = GL_DYNAMIC_DRAW,
};

// Works only with Wavefront(.obj) files
class Mesh
{
public:
	using MeshData = std::pair<std::vector<float>, std::vector<unsigned int>>;
public:
	Mesh(const std::string& filePath, const MeshUsage usage = MeshUsage::Static)
	{
		try
		{
			auto data = PrepData(filePath);
			glGenVertexArrays(1, &m_VAO);
			glBindVertexArray(m_VAO);
			glGenBuffers(1, &m_VBO);
			glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float) * data.first.size(), data.first.data(), static_cast<unsigned int>(usage));

			glGenBuffers(1, &m_EBO);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * data.second.size(), data.second.data(), static_cast<unsigned int>(usage));

			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
			glEnableVertexAttribArray(0);
		}
		catch (std::exception e)
		{
			std::cout << e.what() << std::endl;
		}
	}

	// Draws the mesh using the shader
	void Draw(const Shader& shader)
	{
		shader.Use();
		glBindVertexArray(m_VAO);
		glDrawElements(GL_TRIANGLES, m_VertCount, GL_UNSIGNED_INT, 0);
	}

private:
	// Reads the mesh data from the filePath to put it into the appropriate vector
	// Throws an exception if the file can't be opened
	MeshData PrepData(const std::string& filePath)
	{
		std::vector<float> posData;
		std::vector<unsigned int> indexData;
		std::ifstream fs(filePath);
		if (fs.is_open())
		{
			std::string line;
			while (std::getline(fs, line))
			{
				std::string lineValue= line.substr(line.find_first_of(' '));
				std::stringstream ss(lineValue);
				if (line[0] == 'v')
				{
					while (ss.good())
					{
						std::string str;
						ss >> str;
						posData.push_back(std::stof(str));
					}
				}
				else if (line[0] == 'f')
				{
					while (ss.good())
					{
						std::string str;
						ss >> str;
						indexData.push_back(std::stoul(str) - 1);
						m_VertCount++;
					}
					m_FaceCount++;
				}
			}
			std::cout << "Face count of this mesh = " << m_FaceCount << std::endl;
			return { posData, indexData };
		}
		else
		{
			throw std::exception(std::string("Unable to open file at '" + filePath + "'").c_str());
		}
	}

private:
	unsigned int m_VAO = 0;
	unsigned int m_VBO = 0;
	unsigned int m_EBO = 0;
	unsigned int m_FaceCount = 0;
	unsigned int m_VertCount = 0;
};

#endif // MESH_H