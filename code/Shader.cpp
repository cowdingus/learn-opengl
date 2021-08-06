#include "Shader.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <fstream>
#include <iostream>
#include <iterator>

Shader::Shader(const char* vertexShaderPath, const char* fragmentShaderPath)
{
	std::string vertexShaderCode = loadCodeFromFile(vertexShaderPath);
	std::string fragmentShaderCode = loadCodeFromFile(fragmentShaderPath);

	const char* vertexShaderCodeStr = vertexShaderCode.c_str();
	const char* fragmentShaderCodeStr = fragmentShaderCode.c_str();

	unsigned int vertexShader, fragmentShader;
	int success;
	char infoLog[512];

	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderCodeStr, NULL);
	glCompileShader(vertexShader);

	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cerr << "Shader path: " << vertexShaderPath << '\n' << infoLog << std::endl;
		glDeleteShader(vertexShader);
		throw std::runtime_error("Failed to compile vertex shader");
	}

	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderCodeStr, NULL);
	glCompileShader(fragmentShader);

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cerr << "Shader path: " << fragmentShaderPath << '\n' << infoLog << std::endl;
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
		throw std::runtime_error("Failed to compile fragment shader");
	}

	m_id = glCreateProgram();
	glAttachShader(m_id, vertexShader);
	glAttachShader(m_id, fragmentShader);
	glLinkProgram(m_id);

	glGetProgramiv(m_id, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(m_id, 512, NULL, infoLog);
		std::cerr << "Vertex shader path: " << vertexShaderPath << '\n'
			<< "Fragment shader path: " << fragmentShaderPath << '\n'
			<< infoLog << std::endl;
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
		glDeleteProgram(m_id);
		throw std::runtime_error("Failed to link shader program");
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

std::string Shader::loadCodeFromFile(const char* filepath)
{
	std::ifstream codeFile(filepath);

	return {std::istreambuf_iterator<char>(codeFile), std::istreambuf_iterator<char>()};
}

void Shader::use()
{
	glUseProgram(m_id);
}

unsigned int Shader::getId()
{
	return m_id;
}

void Shader::setBool(const std::string &name, bool value) const
{
	glUniform1i(glGetUniformLocation(m_id, name.c_str()), (int)value);
}

void Shader::setInt(const std::string &name, int value) const
{
	glUniform1i(glGetUniformLocation(m_id, name.c_str()), value);
}

void Shader::setFloat(const std::string &name, float value) const
{
	glUniform1f(glGetUniformLocation(m_id, name.c_str()), value);
}
