#pragma once
#include "Mesh.h"
#include "SimpleMath.h"
#include <vector>
class Projectile:	public MeshOb
{
public:
	Projectile();
	~Projectile();
	Projectile(const Projectile& otherObj);
	void SetAlive(bool Alive = false) { this->alive = Alive; }
	bool GetAlive() { return alive; }
	void setDirection(SimpleMath::Vector3 direction);
	void update(float deltaTime);
private:
	SimpleMath::Vector3 direction;
	bool alive;
};

class ProjectileManager
{
private:
	std::vector<Projectile*> projectiles;
	std::vector<Projectile*> projectilesDead;
	std::vector<MeshOb*> enemies;
	ID3D11Device* device;
	ID3D11DeviceContext* deviceContext;

	bool endGame = false;
public:
	ProjectileManager(ID3D11Device* dev, ID3D11DeviceContext* devCont);
	~ProjectileManager();

	void create(SimpleMath::Vector3 direction, SimpleMath::Vector3);
	void update(float deltaTime);
	bool EndGame() { return endGame; }
	void GetEnemies(std::vector<MeshOb*> enemies);
	//void 
};