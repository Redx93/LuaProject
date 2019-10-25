#include "ProjectileManager.h"

Projectile::Projectile()
{
}

Projectile::~Projectile()
{
}

Projectile::Projectile(const Projectile& otherObj) :MeshOb(otherObj)
{
	this->direction = otherObj.direction;
	
	//*this = otherObj;

}

void Projectile::setDirection(SimpleMath::Vector3 direction)
{
	this->direction = direction;
}

void Projectile::update(float deltaTime)
{
	this->AdjustPosition(direction);
}

ProjectileManager::ProjectileManager(ID3D11Device* dev, ID3D11DeviceContext* devCont)
{
	this->device = dev;
	this->deviceContext = devCont;

}

ProjectileManager::~ProjectileManager()
{
	/*for (std::vector<Projectile*>::iterator pObj = projectiles.begin();
		pObj != projectiles.end(); ++pObj) 
	{
		delete *pObj; // Note that this is deleting what pObj points to,
					  // which is a pointer
	}*/
}

void ProjectileManager::create(SimpleMath::Vector3 direction)
{
	float scale = 0.2f;
	Projectile newBullet; //= new Projectile;
	this->projectiles.push_back(newBullet);
	this->projectiles[this->projectiles.size()-1].setDirection(direction);
	this->projectiles[this->projectiles.size() - 1].Initialize(this->device, this->deviceContext);
	this->projectiles[this->projectiles.size() - 1].SetType("Projectile");


	
	this->projectiles[this->projectiles.size() - 1].SetPosition(this->projectiles[this->projectiles.size() - 1].GetPositionFloat3().x
		, this->projectiles[this->projectiles.size() - 1].GetPositionFloat3().y, -3);
	this->projectiles[this->projectiles.size() - 1].SetScale(scale, scale, scale);
		///.push_back(newBullet);
	
}

void ProjectileManager::update(float deltaTime)
{
	for (int i = 0; i < projectiles.size(); i++)
	{
		//check collision
		projectiles[i].update(deltaTime);
		projectiles[i].Draw();
	}
}
