#include "VAO.h"

VAO::VAO() {
	glGenVertexArrays(1, &this->id);
}

void VAO::linkVBO(VBO &buffer, GLuint layout, int size, GLenum type, GLboolean normalized, int stride, const void *offset) {
	this->bind();
	glVertexAttribPointer(layout, size, type, normalized, stride, offset);
	glEnableVertexAttribArray(layout);
	this->unbind();
}

void VAO::bind() {
	glBindVertexArray(this->id);
}

void VAO::unbind() {
	glBindVertexArray(0);
}

void VAO::destroy() {
	glDeleteVertexArrays(1, &this->id);
}