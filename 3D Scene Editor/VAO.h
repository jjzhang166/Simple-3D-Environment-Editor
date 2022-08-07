#pragma once
#ifndef VAO_H
#define VAO_H 1

#include "stdafx.hpp"
#include "VBO.h"

class VAO {
public:
	GLuint id;

	VAO();

	void linkVBO(VBO& buffer, GLuint layout, int size, GLenum type, GLboolean normalized, int stride, const void *offset);
	void bind();
	void unbind();
	void destroy();
};

#endif