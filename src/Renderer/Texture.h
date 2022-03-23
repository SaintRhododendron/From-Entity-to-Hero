#pragma once

#include <glad/glad.h>

#include<string>
#include<iostream>
#include<map>
#include<glm/vec2.hpp>
#define TX_BASIC_LEFT_BOTTOM_UV 0.f
#define TX_BASIC_RIGHT_TOP_UV 1.f
#define TX_BASIC_CHANELS_AMOUNT 4
#define TX_BASIC_FILTER GL_LINEAR
#define TX_BASIC_WRAP_MODE GL_CLAMP_TO_EDGE

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
			SubTexture(): leftBottomUV(TX_BASIC_LEFT_BOTTOM_UV), rightTopUV(TX_BASIC_RIGHT_TOP_UV) {}
		};

		Texture(GLuint width, GLuint height, 
				const unsigned char* textureData, 
				const unsigned int chanelsAmount = TX_BASIC_CHANELS_AMOUNT,
				const GLenum filter = TX_BASIC_FILTER,
				const GLenum wrapMode = TX_BASIC_WRAP_MODE);
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

		std::map<std::string, SubTexture> _subTextures;
	};
}