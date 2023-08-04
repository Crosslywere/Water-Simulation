#pragma once
#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <vector>
#include <sstream>
#include <fstream>
#include <iostream>
#include <glm/glm.hpp>

enum class ShaderType
{
	None = 0,
	Vertex = GL_VERTEX_SHADER,
	Geometry = GL_GEOMETRY_SHADER,
	Fragment = GL_FRAGMENT_SHADER
};

struct ShaderFile
{
	std::string filePath = "";
	ShaderType type = ShaderType::None;
};

class Shader
{
public:
	Shader(const ShaderFile& shaderFile1 = {}, const ShaderFile& shaderFile2 = {}, const ShaderFile& shaderFile3 = {})
	{
		std::vector<unsigned int> shaders;
		if (shaderFile1.type != ShaderType::None)
		{
			shaders.push_back(CompileShader(shaderFile1.filePath, static_cast<unsigned int>(shaderFile1.type)));
			if (shaderFile2.type != ShaderType::None)
			{
				shaders.push_back(CompileShader(shaderFile2.filePath, static_cast<unsigned int>(shaderFile2.type)));
				if (shaderFile3.type != ShaderType::None)
				{
					shaders.push_back(CompileShader(shaderFile3.filePath, static_cast<unsigned int>(shaderFile3.type)));
				}
			}
			m_ProgramID = glCreateProgram();
			for (auto shader : shaders)
				glAttachShader(m_ProgramID, shader);

			glLinkProgram(m_ProgramID);
		}
		for (auto shader : shaders)
			glDeleteShader(shader);

	}
	~Shader()
	{
		if (m_ProgramID != 0)
			glDeleteProgram(m_ProgramID);
	}
	void Use()
	{
		glUseProgram(m_ProgramID);
	}
private:
	static unsigned int CompileShader(const std::string& shaderPath, unsigned int type)
	{
		std::ifstream sourceFile(shaderPath);
		if (sourceFile.is_open())
		{
			unsigned int shader = glCreateShader(type);
			std::stringstream ss;
			ss << sourceFile.rdbuf();
			std::string str = ss.str();
			const char* source = str.c_str();
			glShaderSource(shader, 1, &source, 0);
			glCompileShader(shader);
			sourceFile.close();
			ReportShaderStatus(shader, GL_COMPILE_STATUS);
			return shader;
		}
		else
		{
			std::cerr << "\033[31mUnable to open shader file " << shaderPath << "\033[0m" << std::endl;
			sourceFile.close();
			return 0;
		}
	}
	static void ReportShaderStatus(unsigned int shader, unsigned int pname)
	{
		static int success = 0;
		glGetShaderiv(shader, pname, &success);
		if (!success)
		{
			char info[512];
			glGetShaderInfoLog(shader, 512, 0, info);
			std::cout << "Error in shader\n" << info << std::endl;
		}
		else
		{
			std::cout << "Shader compiled successfully" << std::endl;
		}
	}
private:
	unsigned int m_ProgramID = 0;
};

#endif // SHADER_H