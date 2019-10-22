//#include "Enemy.h"
//
//Enemy::Enemy()
//{
//	this->hp = 3;
//	this->speed = 1.f;
//}
//
//Enemy::~Enemy()
//{
//}
//
//void Enemy::initWaypoints(std::vector<XMFLOAT3> newList)
//{
//	this->waypoints = newList; //merge list into this->list
//	//does this work or is a for-loop needed?
//}
//
//void Enemy::calcNewWP() //move this to update?
//{
//	XMFLOAT3 from;
//	XMFLOAT3 to;
//	from = waypoints.front();
//	waypoints.erase(waypoints.begin());
//
//	if (waypoints.empty() != false)
//		to = waypoints.front();
//	//else
//	//	to = { 0.f,0.f,0.f }; //debug waypoint object goes to 0
//	//don't delete front of list (used for next round of start/end)
//
//	SimpleMath::Vector3 start = from;
//	SimpleMath::Vector3 end = to;
//	moveVec = end - start;
//	moveVec.Normalize();
//	moveVec = moveVec * speed;
//	
//
//}
//
////XMFLOAT3 Enemy::getMoveVector() const
////{
////	return this->moveVec;
////}
//
//bool Enemy::update(float deltaTime)
//{
//	bool result = false;
//	SimpleMath::Vector3 enemyPos = this->GetPositionFloat3();
//	SimpleMath::Vector3 currentTargetWP = waypoints.front(); //confirm if this is target wp
//	//sqrtf(x*x+y*y+z*z);
//	float length = SimpleMath::Vector3::Distance(enemyPos, currentTargetWP);
//	
//	if(length<0.5)
//		calcNewWP();
//
//	SimpleMath::Vector3 move = moveVec *deltaTime;
//	this->AdjustPosition(move);
//	return this->waypoints.empty();
//}
//
//
