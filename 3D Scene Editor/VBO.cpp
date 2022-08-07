#include "VBO.h"

VBO::VBO() {
	initialized = false;
}

VBO::VBO(GLfloat* vertices, GLsizeiptr size) {
	initialized = false;
	init(vertices, size);
}

void VBO::bind() {
	if (isInitialized()) {
		glBindBuffer(GL_ARRAY_BUFFER, this->id);
	}
}

void VBO::unbind() {
	if (isInitialized()) {
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
}

void VBO::destroy() {
	if (isInitialized()) {
		glDeleteBuffers(1, &this->id);
	}
}

void VBO::init(GLfloat* vertices, GLsizeiptr size) {
	if (!initialized) {
		initialized = true;
		glGenBuffers(1, &this->id);
		bind();
		glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
		unbind();
	}
}

bool VBO::isInitialized() {
	return initialized;
}