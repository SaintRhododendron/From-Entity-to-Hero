#include "Sprite.h"
#include "Texture.h"
#include "ShaderProgram.h"
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#define CMD(TEXT) std::cout << TEXT << std::endl
namespace Renderer
{
	Sprite::Sprite(std::shared_ptr<Texture> pTexture,
				   std::shared_ptr<ShaderProgram> pShaderProgram,
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
		const GLfloat textureCoords[] = {
			0.f, 0.f,
			0.f, 1.f,
			1.f, 1.f,

			1.f, 1.f,
			1.f, 0.f,
			0.f, 0.f
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
		CMD("[SPRITE] Enabling ShaderProgram.");
		_pShaderProgram->use();
		CMD("[SPRITE] Creating ModelMatrix.");
		glm::mat4 modelMatrix(1.0f);
		CMD("[SPRITE] Calculating ModelMatrix.");
		modelMatrix = glm::translate(modelMatrix, glm::vec3(_position, 0.f));
		modelMatrix = glm::translate(modelMatrix, glm::vec3(0.5f * _size.x, 0.5f * _size.y, 0.0f));
		modelMatrix = glm::rotate(modelMatrix, glm::radians(_rotation), glm::vec3(0.f, 0.f, 1.f));
		modelMatrix = glm::translate(modelMatrix, glm::vec3(-0.5f * _size.x, -0.5f * _size.y, 0.0f));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(_size, 1.0f));
		CMD("[SPRITE] Binding VAO.");
		glBindVertexArray(_VAO);
		CMD("[SPRITE] Sending ModelMatrix to the ShaderProgram.");
		_pShaderProgram->setMatrix4("modelMatrix", modelMatrix);
		CMD("[SPRITE] Activating texture.");
		glActiveTexture(GL_TEXTURE0);
		CMD("[SPRITE] Binding Texture.");
		_pTexture->bind();
		CMD("[SPRITE] Drawing Triangles.");
		glDrawArrays(GL_TRIANGLES, 0, 6);
		CMD("[SPRITE] Unbinding VAO.");
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