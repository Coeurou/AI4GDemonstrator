#pragma once

#include <GL/glew.h>
#include <glm.hpp>
#include <string>

class Texture
{
public:
				Texture(GLenum target);
				Texture(const Texture& t);
				Texture(Texture&& t) noexcept;
				~Texture();
	Texture&	operator=(const Texture& t);
	Texture&	operator=(Texture&& t) noexcept;

	void		SetParameter(GLenum paramName, GLint value);
	void		SetParameter(GLenum paramName, GLfloat value);
	void		SetActive(GLenum textureUnit) const;
	void		BindTexture() const;
	void		LoadImage(const char* filePath, bool generateMipmap);

private:
	GLuint		textureID;
	GLenum		target;
	std::string imageFilePath;
	bool		hasMipmaps;
};