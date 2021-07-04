#pragma once
#include "..//Mesh.h"
#include <list>
class Enemy : public MeshOb
{
public:
	Enemy();
	~Enemy();
	std::string GetEnemyType() { return type; }

	void initWaypoints(std::list<XMFLOAT3> newList);
	void calcNewWP();
	XMFLOAT3 getMoveVector()const;
private:
	std::string type;
	int hp; //hp based on that one enemy has 3 or 5 hits and tower has slow attackspeed
	float speed;

	std::list<XMFLOAT3> waypoints;

	XMFLOAT3 startWP;
	XMFLOAT3 endWP;
	XMFLOAT3 moveVec; 


};
