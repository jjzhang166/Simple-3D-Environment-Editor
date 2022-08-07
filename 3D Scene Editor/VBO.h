#pragma once
#ifndef VBO_H
#define VBO_H 1

#include "stdafx.hpp"

class VBO {
private:
	bool initialized;

public:
	GLuint id;

	VBO();
	VBO(GLfloat* vertices, GLsizeiptr size);

	void bind();
	void unbind();
	void destroy();
	void init(GLfloat* vertices, GLsizeiptr size);
	bool isInitialized();
};

#endif