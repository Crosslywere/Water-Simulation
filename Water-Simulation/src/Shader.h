#pragma once
#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <vector>
#include <sstream>
#include <fstream>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

// Enumeration for the 3 types of shaders
enum class ShaderType
{
	None = 0,
	Vertex = GL_VERTEX_SHADER,
	Geometry = GL_GEOMETRY_SHADER,
	Fragment = GL_FRAGMENT_SHADER
};

// Structure that points to a shader file via filePath and holds what type of shader the file is
struct ShaderFile
{
	std::string filePath = "";
	ShaderType type = ShaderType::None;
};

// Class used to interact with shaders
class Shader
{
public:
	Shader(const ShaderFile& shaderFile1 = {}, const ShaderFile& shaderFile2 = {}, const ShaderFile& shaderFile3 = {})
	{
		std::vector<unsigned int> shaders;
		if (shaderFile1.type != ShaderType::None)
		{
			try
			{
				shaders.push_back(CompileShader(shaderFile1.filePath, static_cast<unsigned int>(shaderFile1.type)));
			}
			catch (std::exception e)
			{
				std::cout << e.what() << std::endl;
			}
			if (shaderFile2.type != ShaderType::None)
			{
				try
				{
					shaders.push_back(CompileShader(shaderFile2.filePath, static_cast<unsigned int>(shaderFile2.type)));
				}
				catch (std::exception e)
				{
					std::cout << e.what() << std::endl;
				}
				if (shaderFile3.type != ShaderType::None)
				{
					try
					{
						shaders.push_back(CompileShader(shaderFile3.filePath, static_cast<unsigned int>(shaderFile3.type)));
					}
					catch (std::exception e)
					{
						std::cout << e.what() << std::endl;
					}
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

	// Switch shader to shader instance.
	void Use()
	{
		glUseProgram(m_ProgramID);
	}
	
	// Disables all shaders until Use is called from a shader instance
	static void Disable()
	{
		glUseProgram(0);
	}

	// Sets a float uniform with a matching name in the file to be the value of f
	void SetFloat(const std::string& name, float f)
	{
		glUniform1f(glGetUniformLocation(m_ProgramID, name.c_str()), f);
	}

	// Sets a vec3 uniform with a matching name in the file to be the value of v
	void SetVec3(const std::string& name, glm::vec3 v)
	{
		glUniform3fv(glGetUniformLocation(m_ProgramID, name.c_str()), GL_FALSE, glm::value_ptr(v));
	}

	// Sets a mat4 uniform with a matching name to the file to be the value of m
	void SetMat4(const std::string& name, glm::mat4 m)
	{
		glUniformMatrix4fv(glGetUniformLocation(m_ProgramID, name.c_str()), 1, GL_FALSE, glm::value_ptr(m));
	}

private:
	// Reads in the shader from the file path compiles the shader
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
			// std::cerr << "\033[31mUnable to open shader file " << shaderPath << "\033[0m" << std::endl;
			sourceFile.close();
			throw std::exception(std::string("Unable to open shader file in path '" + shaderPath).c_str());
			return 0;
		}
	}
	
	// Reports on the compilation errors if any
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