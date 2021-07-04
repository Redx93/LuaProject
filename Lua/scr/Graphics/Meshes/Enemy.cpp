#include "Enemy.h"

Enemy::Enemy()
{
	this->type = "Enemy type";
	this->hp = 3;
	this->speed = 1.f;
}

Enemy::~Enemy()
{
}

void Enemy::initWaypoints(std::list<XMFLOAT3> newList)
{
	this->waypoints = newList; //merge list into this->list
	//does this work or is a for-loop needed?
}

void Enemy::calcNewWP() //handle stuff intern
{
	//get fron of list (1st object become start)
	XMFLOAT3 from;
	XMFLOAT3 to;
	from = waypoints.front();
	//delete front of list
	waypoints.pop_front();
	//get fron of list (2nd object becomes end) 
	//if list is empty or only first element exist a problem arises
	if (waypoints.empty() != false)
		to = waypoints.front();
	else
		to = { 0.f,0.f,0.f }; //debug waypoint object goes to 0
	//don't delete front of list (used for next round of start/end)

	float deltaX = to.x - from.x;
	float deltaY = to.y - from.y;
	float deltaZ = to.z - from.z;
	XMFLOAT3 rawWP = XMFLOAT3(deltaX, deltaY, deltaZ); //calc vector to WP
	float length = sqrtf(deltaX * deltaX + deltaY * deltaY + deltaZ * deltaZ); //calc length between points
	//normalized vector can be used to multiply with speed
	this->moveVec = XMFLOAT3(deltaX / length, deltaY / length, deltaZ / length);
	//multiply with speed to get moveVector
	this->moveVec = XMFLOAT3(this->moveVec.x * this->speed, this->moveVec.y * this->speed, this->moveVec.z * this->speed);
}

XMFLOAT3 Enemy::getMoveVector() const
{
	return this->moveVec;
}


