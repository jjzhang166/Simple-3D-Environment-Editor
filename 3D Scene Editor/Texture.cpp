#include "Texture.h"

#ifndef STB_IMAGE_IMPLEMENTATION
	#define STB_IMAGE_IMPLEMENTATION
	#include <stb/stb_image.h>
#endif

Texture::Texture() {
	initialized = false;
}

Texture::Texture(std::string sourceFile, GLenum type) {
	initialized = false;
	init(sourceFile, type);
}

void Texture::init(std::string sourceFile, GLenum target) {
	if (!isInitialized()) {
		initialized = true;
		this->target = target;
		glGenTextures(1, &id);
		bind();
		unsigned char* data = readPictureData(sourceFile);
		if (data) {
			glTexImage2D(target, 0, GL_RGBA, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(target);
		} else {
			LOG("ERROR: UNABLE TO READ FILE '" << sourceFile << "'");
			throw(errno);
		}
		stbi_image_free(data);
		unbind();
	}
}

void Texture::bind() {
	if (isInitialized()) {
		glBindTexture(target, id);
	}
}

void Texture::unbind() {
	if (isInitialized()) {
		glBindTexture(target, 0);
	}
}

void Texture::destroy() {
	if (isInitialized()) {
		glDeleteTextures(1, &id);
		initialized = false;
	}
}

bool Texture::isInitialized() {
	return initialized;
}

unsigned char* Texture::readPictureData(std::string sourceFile) {
	stbi_set_flip_vertically_on_load(true);
	int nrChannel;
	return stbi_load(sourceFile.c_str(), &width, &height, &nrChannel, 0);
}