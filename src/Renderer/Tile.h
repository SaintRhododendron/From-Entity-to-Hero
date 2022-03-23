#pragma once

#include "Sprite.h"

namespace Renderer
{
	class Tile : public Sprite
	{
	public:
		Tile(const std::shared_ptr<Texture> pTexture,
			 const std::string initialSubTextureName,
			 const std::shared_ptr<ShaderProgram> pShaderProgram,
			 const glm::vec2& position);
		~Tile();

		Tile(const Tile&) = delete;
		Tile& operator=(const Tile&) = delete;

	private:

	};
}