#pragma once
#include "..//Mesh.h"
class Tower : public MeshOb
{
public:
	Tower();
	~Tower();
	std::string GetTowerType() { return typeof; }
	float getRadius()const { return radius; };
	float getDamage()const { return damage; };
private:
	std::string typeof;
	float radius;
	float damage;
};
