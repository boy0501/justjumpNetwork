#pragma once
#include <vector>
class Map
{
public:
	Map();
	~Map();

	std::vector<class Object*> mObjects[5];
	void LoadAllObjects();
};

