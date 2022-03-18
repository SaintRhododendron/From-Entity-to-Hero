#include "Texture.h"

#include"glm/vec2.hpp"
#include<map>

namespace Renderer
{

	Texture::Texture(GLuint width, GLuint height,
					 const unsigned char* textureData,
					 const unsigned int chanelsAmount,
					 const GLenum filter,
					 const GLenum wrapMode)
					: _width(width)
					, _height(height)
	{
		switch (chanelsAmount)
		{
		case(4):
			_format = GL_RGBA;
			break;
		case(3):
			_format = GL_RGB;
			break;
		default:
			_format = GL_RGBA;
			break;
		}
	
		glGenTextures(1, &_ID);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, _ID);
		glTexImage2D(GL_TEXTURE_2D, 0, _format, _width, _height, 0, _format, GL_UNSIGNED_BYTE, textureData);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapMode);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapMode);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);
		glGenerateMipmap(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	Texture& Texture::operator=(Texture&& texture) noexcept
	{
		glDeleteTextures(1, &_ID);
		_ID = texture._ID;
		texture._ID = 0;
		_width = texture._width;
		_height = texture._height;
		_format = texture._format;
		return *this;
	}

	Texture::Texture(Texture&& texture) noexcept
	{
		_ID = texture._ID;
		texture._ID = 0;
		_width = texture._width;
		_height = texture._height;
		_format = texture._format;
	}

	Texture::~Texture()
	{
		glDeleteTextures(1, &_ID);
	}

	void Texture::bind()
	{
		glBindTexture(GL_TEXTURE_2D, _ID);
	}

	void Texture::addSubTexture(std::string name, const glm::vec2& leftBottomUV, const glm::vec2& rightTopUV)
	{
		_subTextures.emplace(std::move(name), SubTexture(leftBottomUV, rightTopUV));
	}

	const Texture::SubTexture& Texture::getSubTexture(const std::string& name) const
	{
		auto it = _subTextures.find(name);
		if (it != _subTextures.end())
		{
			return it->second;
		}
		const static SubTexture defaultSubTexture;
		return defaultSubTexture;
	}

}