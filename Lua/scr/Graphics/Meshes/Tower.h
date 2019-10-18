#pragma once
#include "..//Mesh.h"
class Tower : public MeshOb
{
public:
	Tower();
	~Tower();
	std::string GetTowerType() { return typeof; }
private:
	std::string typeof;
};
