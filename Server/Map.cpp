#include <iostream>
#include <fstream>
#include <sstream>
#include "Map.h"
#include "Object.h"


Map::Map()
{

}

Map::~Map()
{
	for (int i = 0; i < 5; ++i)
	{
		for (auto& p : mObjects[i])
		{
			delete p;
		}
	}
}


void Map::LoadAllObjects()
{
	int x, y, w, h, type;
	int objcount = 0;

	for (int i = 0; i < 5; ++i)
	{
		std::string mapName = "map/map_";
		mapName += std::to_string(i) + ".txt";
		std::ifstream in(mapName);
		if (!in.is_open())
		{
			std::cerr << "file not found : " << mapName << std::endl;
			continue;
		}

		for (int j = 0; j < 150; ++j)
		{
			if (in.eof())
			{
				objcount = j;
				break;
			}
			in >> x >> y >> w >> h >> type;
			auto obj = new Object();
			obj->initalize(x, y, w, h, type);
			mObjects[i].push_back(obj);
		}
		std::cout << mapName << " Load Complete" << std::endl;
	}
}