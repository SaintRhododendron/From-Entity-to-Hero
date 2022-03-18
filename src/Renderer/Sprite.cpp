#include "Sprite.h"
#include "Texture.h"
#include "ShaderProgram.h"
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace Renderer
{
	Sprite::Sprite(const std::shared_ptr<Texture> pTexture,
				   const std::string initialSubTextureName,
				   const std::shared_ptr<ShaderProgram> pShaderProgram,
				   const glm::vec2& position,
				   const glm::vec2& size,
				   const float rotation)
					: _pTexture(std::move(pTexture))
					, _pShaderProgram(std::move(pShaderProgram))
					, _position (position)
					, _size (size)
					, _rotation (rotation)
		
	{
		const GLfloat vertexCoords[] = {
			0.f, 0.f,
			0.f, 1.f,
			1.f, 1.f,

			1.f, 1.f,
			1.f, 0.f,
			0.f, 0.f
		};

		auto pSubTexture = pTexture->getSubTexture(initialSubTextureName);

		const GLfloat textureCoords[] = {
			pSubTexture.leftBottomUV.x, pSubTexture.leftBottomUV.y,
			pSubTexture.leftBottomUV.x, pSubTexture.rightTopUV.y,
			pSubTexture.rightTopUV.x, pSubTexture.rightTopUV.y,

			pSubTexture.rightTopUV.x, pSubTexture.rightTopUV.y,
			pSubTexture.rightTopUV.x, pSubTexture.leftBottomUV.y,
			pSubTexture.leftBottomUV.x, pSubTexture.leftBottomUV.y
		};

		glGenVertexArrays(1, &_VAO);
		glBindVertexArray(_VAO);

		glGenBuffers(1, &_vertexCoordsVBO);
		glBindBuffer(GL_ARRAY_BUFFER, _vertexCoordsVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertexCoords), &vertexCoords, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

		glGenBuffers(1, &_textureCoordsVBO);
		glBindBuffer(GL_ARRAY_BUFFER, _textureCoordsVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(textureCoords), &textureCoords, GL_STATIC_DRAW);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

	}

	Sprite::~Sprite()
	{
		glDeleteBuffers(1, &_vertexCoordsVBO);
		glDeleteBuffers(1, &_textureCoordsVBO);
		glDeleteVertexArrays(1, &_VAO);
	}


	void Sprite::render()
	{
		
		_pShaderProgram->use();
		glm::mat4 modelMatrix(1.0f);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(_position, 0.f));
		modelMatrix = glm::translate(modelMatrix, glm::vec3(0.5f * _size.x, 0.5f * _size.y, 0.0f));
		modelMatrix = glm::rotate(modelMatrix, glm::radians(_rotation), glm::vec3(0.f, 0.f, 1.f));
		modelMatrix = glm::translate(modelMatrix, glm::vec3(-0.5f * _size.x, -0.5f * _size.y, 0.0f));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(_size, 1.0f));
		glBindVertexArray(_VAO);
		_pShaderProgram->setMatrix4("modelMatrix", modelMatrix);
		glActiveTexture(GL_TEXTURE0);
		_pTexture->bind();
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);
	}

	void Sprite::setSize(const glm::vec2& size)
	{
		_size = size;
	}

	void Sprite::setPosition(const glm::vec2& position)
	{
		_position = position;
	}

	void Sprite::setRotation(const float rotation)
	{
		_rotation = rotation;
	}

}