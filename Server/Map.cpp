#include <iostream>
#include <fstream>
#include <sstream>
#include "Map.h"
#include "Object.h"
#include "OBSTACLE/Obstacle.h"
#include "OBSTACLE/MoveObstacle.h"
#include "OBSTACLE/AttackObstacle.h"
#include "OBSTACLE/PlatformObstacle.h"
#include "INTERACT/Interact.h"

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
			Object* obj = nullptr;
			//0. �κ�ȭ�� ��� 1.�ٴ� 2.�⺻ ���� 3.�������� 4.�����̾� ��Ʈ�����ʹ��� 5.����ٴ� 6. �����̾Ʈ ���ʹ��� 
			//7. ���� 101.����� 102.���������� 103.���� 106,107.��Ϲ��� 201.��Ż 301.����
			switch (type)
			{
			case 0: obj = new Object(); break;
			case 1: obj = new PlatformObstacle(); break;
			case 2: obj = new PlatformObstacle(); break;
			case 3: obj = new PlatformObstacle(); break;
			case 4: obj = new PlatformObstacle(); break;
			case 5: obj = new  PlatformObstacle(); break;
			case 6: obj = new PlatformObstacle(); break;
			case 7: obj = new PlatformObstacle(); break;
			case 101: obj = new PlatformObstacle();  break;
			case 102: obj = new PlatformObstacle(); break;
			case 103: obj = new AttackObstacle(1.f, 3); break;
			case 106: obj = new MoveObstacle(MoveObstacle::MoveType::Horizontal); break;
			case 107: obj = new MoveObstacle(MoveObstacle::MoveType::Vertical); break;
			case 201: obj = new Interact(); break;
			case 301: obj = new Interact(); break;
			default: std::cerr << "Bad Object Read" << std::endl; return;
			}
			obj->initalize(x, y, w, h, type);
			mObjects[i].push_back(obj);
		}
		std::cout << mapName << " Load Complete" << std::endl;
	}
}