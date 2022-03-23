#include "Tile.h"

namespace Renderer
{
	Tile::Tile(const std::shared_ptr<Texture> pTexture,
			   const std::string initialSubTextureName,
			   const std::shared_ptr<ShaderProgram> pShaderProgram,
			   const glm::vec2& position) : Sprite(pTexture, initialSubTextureName, pShaderProgram, position)
	{

	}

	Tile::~Tile()
	{

	}
}