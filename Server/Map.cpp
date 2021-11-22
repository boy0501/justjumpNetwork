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
			//0. 로비화면 배경 1.바닥 2.기본 발판 3.작은발판 4.컨베이어 벨트오른쪽방향 5.투명바닥 6. 컨베이어벨트 왼쪽방향 
			//7. 발판 101.나사못 102.깨진파이프 103.증기 106,107.톱니바퀴 201.포탈 301.로프
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