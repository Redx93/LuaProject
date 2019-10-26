#ifndef MESHOB_H
#define MESHOB_H
#include "Vertex.h"
#include "VertexBuffer.h"
#include "ConstantBuffer.h"
#include "GameObject.h"
#include <vector>
#include <d3d11.h>
#include <SimpleMath.h>
#include "../Timer.h"
using namespace DirectX;
class MeshOb;


class Enemy
{
public:
	int hp = 10;
	float speed = 0.18f;;
	std::vector<XMFLOAT3> waypoints;
	std::vector<XMFLOAT3> waypointsStored;
	SimpleMath::Vector3 moveVec;
};

class Tower
{
public:
	float dmg  = 1;
	float radius = 5;
	float projectileSpeed = 0.05f;
};
class MeshOb : public GameObject
{
public:
	MeshOb();
	~MeshOb();

	MeshOb(const MeshOb& otherObj);
	bool Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext,
		Color color = Colours::Red);
	void Draw();
	void SetColor(Color color);
	void SetType(std::string type);
	std::string GetType();
	XMMATRIX worldMatrix = XMMatrixIdentity();
	std::vector<Vertex> vertices;
	VertexBuffer<Vertex> vertexBuffer;
	UINT IndexCount = 0;

	//enemy
	void InitEnemy();
	bool Update(float dt); //returns if waypoints is empty
	void CalcNewWP();
	void initWaypoints(std::vector<XMFLOAT3> newList);
	void ResetWayPoint();
	//tower
	void InitTower();
	bool InRange(MeshOb* incomingOb);
	void Shoot(MeshOb* incomingOb);
	void RemoveHP(float value) { this->enemy->hp -= value; }
	float GetHealth() { return this->enemy->hp; }
private:
	void UpdateMatrix();
	std::string type;
	Color color;
	/* constant buffers*/
	ConstantBuffer<CB_PS_Color> cb_vs_Color;
	ConstantBuffer<CB_VS_vertexshader> cb_vs_vertexshader;

	ID3D11Device* device = nullptr;
	ID3D11DeviceContext* deviceContext = nullptr;

	Timer projectileTimer;
	Enemy* enemy;
	Tower* tower;

	
};
#endif // !