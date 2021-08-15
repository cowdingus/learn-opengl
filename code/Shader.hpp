#pragma once

#include <string>

#include <glm/matrix.hpp>

class Shader
{
public:
	Shader(const char* vertexPath, const char* fragmentPath);

	void use();
	unsigned int getId();

	void setBool(const std::string& name, bool value) const;
	void setInt(const std::string& name, int value) const;
	void setFloat(const std::string& name, float value) const;
	void setMatrix4(const std::string& name, const glm::mat4& matrix) const;
private:
	unsigned int m_id;

	std::string loadCodeFromFile(const char* filepath);
};

