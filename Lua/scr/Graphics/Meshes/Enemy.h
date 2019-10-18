#pragma once
#include "..//Mesh.h"
class Enemy : public MeshOb
{
public:
	Enemy();
	~Enemy();
	std::string GetEnemyType() { return type; }
private:
	std::string type;
};
