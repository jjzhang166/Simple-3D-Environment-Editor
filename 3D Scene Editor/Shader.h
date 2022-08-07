#pragma once

#ifndef SHADER_CLASS_H
#define SHADER_CLASS_H

#include "stdafx.hpp"

std::string getFileContents(std::string source);

class Shader {
public:
	GLuint id;
	Shader(std::string vertexFile, std::string fragFile);

	void setMat4(std::string name, const glm::mat4& value);
	void setMat3(std::string name, const glm::mat3& value);
	void setVec3(std::string name, const glm::vec3& value);
	void setFloat(std::string name, const float value);
	void setInt(std::string name, const int value);
	void activate();
	void destroy();
};

#endif