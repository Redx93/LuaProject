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
	for(int i = projectiles.size()-1; i>=0; i--)
		delete projectiles[i];
}

void ProjectileManager::create(SimpleMath::Vector3 direction, SimpleMath::Vector3 origin)
{
	float scale = 1.f;
	Projectile* newBullet = new Projectile;
	this->projectiles.push_back(newBullet);
	this->projectiles[this->projectiles.size() - 1]->setDirection(direction);
	this->projectiles[this->projectiles.size() - 1]->Initialize(this->device, this->deviceContext);
	this->projectiles[this->projectiles.size() - 1]->SetType("Projectile");

	this->projectiles[this->projectiles.size() - 1]->SetScale(scale, scale, scale);
	this->projectiles[this->projectiles.size() - 1]->SetPosition(origin);
	this->projectiles[this->projectiles.size() - 1]->AdjustPosition(0.f, 0.f, -2.f); //-8.f, 2.f
	
		///.push_back(newBullet);
	
}

void ProjectileManager::update(float deltaTime)
{
	for (int i = 0; i < projectiles.size(); i++)
	{
		//check collision
		projectiles[i]->update(deltaTime);
		projectiles[i]->Draw();
	}
}
