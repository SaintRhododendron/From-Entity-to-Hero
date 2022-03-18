#pragma once

#include <glad/glad.h>

#include<string>
#include<iostream>
#include <map>
#include<glm/vec2.hpp>

namespace Renderer
{
	class Texture
	{
	public:

		struct SubTexture
		{
			glm::vec2 leftBottomUV;
			glm::vec2 rightTopUV;

			SubTexture(const glm::vec2& _leftBottomUV, const glm::vec2& _rightTopUV) : leftBottomUV(_leftBottomUV), rightTopUV(_rightTopUV) {}
			SubTexture(): leftBottomUV(0.f), rightTopUV(1.f) {}
		};

		Texture(GLuint width, GLuint height, 
				const unsigned char* textureData, 
				const unsigned int chanelsAmount = 4, 
				const GLenum filter = GL_LINEAR, 
				const GLenum wrapMode = GL_CLAMP_TO_EDGE);
		~Texture();

		Texture() = delete;
		Texture(const Texture&) = delete;
		Texture& operator=(const Texture&) = delete;
		Texture& operator=(Texture&& texture) noexcept;
		Texture(Texture&& texture) noexcept; 

		void addSubTexture(std::string name, const glm::vec2& leftBottomUV, const glm::vec2& rightTopUV);
		const SubTexture& getSubTexture(const std::string& name) const;
		unsigned int getWidth() { return _width; }
		unsigned int getHeight() { return _height; }
		void bind();

	private:
		GLenum _format;
		GLuint _ID;
		unsigned int _width;
		unsigned int _height;

		std::map<std::string, SubTexture> _subTextures = {};
	};
}