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
	
	newBullet->setDirection(direction);
	newBullet->Initialize(this->device, this->deviceContext);
	newBullet->SetType("Projectile");

	
	newBullet->SetPosition(origin);

	newBullet->AdjustPosition(0.f, 0.f, -2.f);
	newBullet->SetScale(scale, scale, scale);
	
	this->projectiles.push_back(newBullet);
	
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

void ProjectileManager::GetEnemies(std::vector<MeshOb*> enemies)
{
	this->enemies.clear();
	this->enemies = enemies;
}
