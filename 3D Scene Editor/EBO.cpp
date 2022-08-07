#include "EBO.h"

EBO::EBO() {
	initialized = false;
}

EBO::EBO(GLuint* indices, GLsizeiptr size) {
	initialized = false;
	init(indices, size);
}

void EBO::init(GLuint* indices, GLsizeiptr size) {
	if (!isInitialized()) {
		initialized = true;
		glGenBuffers(1, &this->id);
		bind();
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, indices, GL_STATIC_DRAW);
		unbind();
	}
}

void EBO::bind() {
	if (isInitialized()) {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->id);
	}
}

void EBO::unbind() {
	if (isInitialized()) {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
}

void EBO::destroy() {
	if (isInitialized()) {
		glDeleteBuffers(1, &this->id);
	}
}

bool EBO::isInitialized() {
	return initialized;
}