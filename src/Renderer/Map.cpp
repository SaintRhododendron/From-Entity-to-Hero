#include"Map.h"

#include"glm/vec2.hpp"

#include"Texture.h"
#include"ShaderProgram.h"
#include"Tile.h"

namespace Renderer
{
	Map::Map(const std::shared_ptr<Texture> pTexture, const std::shared_ptr<ShaderProgram> pShaderProgram, const std::vector<std::string> mapData)
	{
		_drawer = new Tile(pTexture, "default", pShaderProgram, glm::vec2(0.f));
		_mapData = mapData;
	}


	Map::~Map()
	{
		if (_drawer)
		{
			delete _drawer;
		}
	}


	void Map::draw()
	{
		_drawer->setSize(glm::vec2(200));
		float xPos = 0.f;
		float yPos = _mapData.size() * _drawer->getSize().y;
		std::string currentTile;
		for (auto& it : _mapData)
		{
			yPos -= _drawer->getSize().y;
			xPos = 0.f;
			for (int i=0; i < it.size(); i++)
			{
				_drawer->setPosition(glm::vec2(xPos, yPos));
				currentTile = it[i];
				_drawer->setSubTexture(currentTile);
				_drawer->render();
				xPos += _drawer->getSize().x;
			}
		}
	}
}