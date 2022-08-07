#pragma once
#ifndef EBO_H
#define EBO_H 1

#include "stdafx.hpp"

class EBO {
private:
	bool initialized;

public:
	GLuint id;

	EBO();
	EBO(GLuint* indices, GLsizeiptr size);

	void init(GLuint* indices, GLsizeiptr size);
	void bind();
	void unbind();
	void destroy();
	bool isInitialized();
};

#endif