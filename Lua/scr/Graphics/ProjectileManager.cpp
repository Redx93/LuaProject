#include "ProjectileManager.h"
#include <iostream>
Projectile::Projectile()
{
	bool alive = false;
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
	newBullet->SetAlive(true);
	this->projectiles.push_back(newBullet);
	
}

void ProjectileManager::update(float deltaTime)
{
	srand(time(NULL));
	for (int j = 0; j < enemies.size(); j++)
	{
		//check collision
		//check collision
		//check collision
		//check collision
		for (int i = 0; i < projectiles.size(); i++)
		{
			if (projectiles[i]->GetAlive() == true)
			{
				SimpleMath::Vector2 enemyPos = SimpleMath::Vector2(enemies[j]->GetPositionFloat3().x, enemies[j]->GetPositionFloat3().y);
				SimpleMath::Vector2 projectile = SimpleMath::Vector2(projectiles[i]->GetPositionFloat3().x, projectiles[i]->GetPositionFloat3().y);
				float value = SimpleMath::Vector2::Distance(enemyPos, projectile);
				if (std::abs(value) <= 0.5f)
				{
					enemies[j]->RemoveHP(5);
					projectiles[i]->SetAlive(false);

					int rSpeed = rand() % 9 + 1;
					int unitHealth = 25;
					if (rSpeed < 3)
						unitHealth = 50;
					if (fmodf(enemies[j]->GetHealth(), unitHealth) <= 3) //restart
					{
						
						int rIncHealth = rand() % 30;


						float rSpeedF = (float)rSpeed / 60;
						enemies[j]->SetSpeed(rSpeedF);
						enemies[j]->AddHealth(rIncHealth);
						enemies[j]->ResetWayPoint();
						this->score += 100;
					}
				}

				if (enemies[j]->GetHealth() <= 0)
				{
					std::cout << "enemy is dead" << std::endl;
					
					this->endGame = true;
					this->score += 1000;
				}
				if (enemies[j]->CheckEnemyInGoal()==true)
				{

				}
				
				projectiles[i]->update(deltaTime);
				projectiles[i]->Draw();
			}

		}
	}
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
