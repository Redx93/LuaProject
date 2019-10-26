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

	void setDirection(SimpleMath::Vector3 direction);
	void update(float deltaTime);
private:
	SimpleMath::Vector3 direction;
};

class ProjectileManager
{
private:
	std::vector<Projectile*> projectiles;
	std::vector<MeshOb*> enemies;
	ID3D11Device* device;
	ID3D11DeviceContext* deviceContext;
	int score;

public:
	ProjectileManager(ID3D11Device* dev, ID3D11DeviceContext* devCont);
	~ProjectileManager();

	void create(SimpleMath::Vector3 direction, SimpleMath::Vector3);
	void update(float deltaTime);

	void GetEnemies(std::vector<MeshOb*> enemies);
	int GetScore()const;
};