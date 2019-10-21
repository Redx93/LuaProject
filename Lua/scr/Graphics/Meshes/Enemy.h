#pragma once
#include "..//Mesh.h"
#include <d3d11.h>
#include <SimpleMath.h>
//#include <list> //use vector

class Enemy : public MeshOb
{
public:
	Enemy();
	~Enemy();
	std::string GetEnemyType() { return type; }

	void initWaypoints(std::vector<XMFLOAT3> newList);
	
	//XMFLOAT3 getMoveVector()const;

	bool update(float dt); //returns if waypoints is empty
private:
	std::string type;
	int hp; //hp based on that one enemy has 3 or 5 hits and tower has slow attackspeed
	float speed;

	std::vector<XMFLOAT3> waypoints;

	//XMFLOAT3 startWP;
	//XMFLOAT3 endWP;
	SimpleMath::Vector3 moveVec;

	void calcNewWP();
};
