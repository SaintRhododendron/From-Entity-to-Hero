#pragma once

#include "glm/vec2.hpp"
#include <memory>
#include<vector>
#include<string>

namespace Renderer
{
	class Tile;
	class Texture;
	class ShaderProgram;

	class Map
	{
	public:
		Map(const std::shared_ptr<Texture> pTexture, const std::shared_ptr<ShaderProgram> pShaderProgram, const std::vector<std::string> mapData);
		~Map();
		void draw();
	private:
		std::vector<std::string> _mapData;
		Tile* _drawer;
	};
}
