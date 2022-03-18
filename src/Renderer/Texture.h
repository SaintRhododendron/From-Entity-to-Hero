#pragma once

#include <glad/glad.h>

#include<string>
#include<iostream>

namespace Renderer
{
	class Texture
	{
	public:
		Texture(GLuint width, GLuint height, 
				const unsigned char* textureData, 
				const unsigned int chanelsAmount = 4, 
				const GLenum filter = GL_LINEAR, 
				const GLenum wrapMode = GL_CLAMP_TO_EDGE);
		~Texture();

		Texture() = delete;
		Texture(const Texture&) = delete;
		Texture& operator=(const Texture&) = delete;
		Texture& operator=(Texture&& texture);
		Texture(Texture&& texture);

		void bind();

	private:
		GLenum _format;
		GLuint _ID;
		unsigned int _width;
		unsigned int _height;
	};
}