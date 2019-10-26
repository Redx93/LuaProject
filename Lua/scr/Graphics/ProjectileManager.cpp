#include "ProjectileManager.h"
#include <iostream>
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
	this->score = 0;

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
	//for (int i = projectiles.size() - 1; i >= 0; i--)
	for (int i = 0; i < projectiles.size(); i++)
	{
		projectiles[i]->Draw();
		projectiles[i]->update(deltaTime);
		
		//check collision
		//check collision
		for (int j = 0; j < enemies.size(); j++)
		{
			SimpleMath::Vector3 enemyPos = enemies[j]->GetPositionFloat3();
			SimpleMath::Vector2 enemy2D = { enemyPos.x,enemyPos.y };
			SimpleMath::Vector3 projectile = projectiles[i]->GetPositionFloat3();
			SimpleMath::Vector2 projectile2D = { projectile.x,projectile.y };
			float dist = SimpleMath::Vector2::Distance(enemy2D, projectile2D);
			
			if (dist <= 0.5f)
			{
				projectiles[i]->SetPosition(-1000000, -1000000, 0);
				enemies[j]->GetEnemy()->hp -= 1;
		
				//int temp = i;
				//Projectile* test = projectiles[i];
				delete projectiles[i];
				if (projectiles.size() != 1)
				{
					projectiles[i] = projectiles[projectiles.size() - 1];
				}
				projectiles.pop_back();
				break;
		
			}
			if (enemies[j]->GetEnemy()->hp == 0)
			{
				std::cout << "enemy is dead" << std::endl;
				enemies[j]->SetPosition(1000000, 1000000, 0);
				//projectiles[i]->SetPosition(-1000000, -1000000, 0);

				//save score
				this->score += 100;
				enemies[j]->GetEnemy()->hp = 3;

				//delete projectiles
				//delete projectiles[i];
				//projectiles[i] = projectiles[projectiles.size() - 1];
				//delete projectiles[projectiles.size() - 1];
				//projectiles.pop_back();
			}
		}
	
	}
	/*for (int i = 0; i < projectiles.size(); i++)
	{
		projectiles[i]->update(deltaTime);
		projectiles[i]->Draw();
	}*/
}

void ProjectileManager::GetEnemies(std::vector<MeshOb*> enemies)
{
	this->enemies.clear();
	this->enemies = enemies;
}

int ProjectileManager::GetScore() const
{
	return this->score;
}
