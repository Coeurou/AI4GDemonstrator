#include "Texture.h"
#include <iostream>
#include <SOIL.h>

Texture::Texture(GLenum target) : target(target)
{
	glGenTextures(1, &textureID);
}

Texture::Texture(const Texture& t) : target(t.target)
{
	glGenTextures(1, &textureID);
	LoadImage(t.imageFilePath.c_str(), t.hasMipmaps);
}

Texture::Texture(Texture&& t) noexcept 
	: target(t.target), textureID(t.textureID),
	  imageFilePath(t.imageFilePath), hasMipmaps(t.hasMipmaps)
{
	t.textureID = 0;
	t.hasMipmaps = false;
	t.imageFilePath = "";
}

Texture::~Texture()
{
	glDeleteTextures(1, &textureID);
}

Texture& Texture::operator=(const Texture& t)
{
	glGenTextures(1, &textureID);
	LoadImage(t.imageFilePath.c_str(), t.hasMipmaps);

	return *this;
}

Texture& Texture::operator=(Texture&& t) noexcept
{
	textureID = t.textureID;
	hasMipmaps = t.hasMipmaps;
	imageFilePath = t.imageFilePath;

	t.textureID = 0;
	t.hasMipmaps = false;
	t.imageFilePath = "";

	return *this;
}

void Texture::SetParameter(GLenum paramName, GLint value)
{
	BindTexture();
	glTexParameteri(target, paramName, value);
	glBindTexture(target, 0);
}

void Texture::SetParameter(GLenum paramName, GLfloat value)
{
	BindTexture();
	glTexParameterf(target, paramName, value);
	glBindTexture(target, 0);
}

void Texture::SetActive(GLenum textureUnit) const
{
	glActiveTexture(textureUnit);
	BindTexture();
}

void Texture::BindTexture() const
{
	glBindTexture(target, textureID);
}

void Texture::LoadImage(const char* filePath, bool generateMipmap)
{
	if (target != GL_TEXTURE_2D) {
		std::cerr << "[Texture::LoadImage] Only 2D texture are compatible." << std::endl;
		return;
	}

	int width = 0;
	int height = 0;
	unsigned char* imageData = SOIL_load_image(filePath, &width, &height, 0, SOIL_LOAD_RGB);
	if (imageData == nullptr) {
		std::cerr << "[Texture::LoadImage] Failed to load file: " << filePath << std::endl;
		return;
	}

	BindTexture();
	glTexImage2D(target, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData);

	hasMipmaps = generateMipmap;
	if (generateMipmap) {
		glGenerateMipmap(target);
	}

	imageFilePath = filePath;
	SOIL_free_image_data(imageData);
	glBindTexture(target, 0);
}
