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

class Mesh
{
public:
	Mesh(const std::string& filePath, const MeshUsage usage = MeshUsage::Static)
	{
		PrepData(filePath);
		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);
		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * m_Position.size(), m_Position.data(), static_cast<unsigned int>(usage));

		glGenBuffers(1, &EBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * m_Indices.size(), m_Indices.data(), static_cast<unsigned int>(usage));

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
		glEnableVertexAttribArray(0);
	}

	// Draws the mesh using the shader
	void Draw(const Shader& shader)
	{
		shader.Use();
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, m_FaceCount, GL_UNSIGNED_INT, 0);
	}

private:
	// Reads the mesh data from the filePath to put it into the appropriate vector
	// Throws an exception if the file can't be opened
	void PrepData(const std::string& filePath)
	{
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
						m_Position.push_back(std::stof(str));
					}
				}
				else if (line[0] == 'f')
				{
					while (ss.good())
					{
						std::string str;
						ss >> str;
						m_Indices.push_back(std::stoul(str) - 1);
					}
					m_FaceCount++;
				}
			}
			std::cout << "Face count of this mesh = " << m_FaceCount << std::endl;
		}
		else
		{
			throw std::exception(std::string("Unable to open file at '" + filePath + "'").c_str());
		}
	}

private:
	std::vector<float> m_Position;
	std::vector<unsigned long> m_Indices;
	unsigned int VAO;
	unsigned int VBO;
	unsigned int EBO;
	unsigned int m_FaceCount = 0;
};

#endif // MESH_H