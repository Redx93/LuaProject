#ifndef MESHOB_H
#define MESHOB_H
#include "Vertex.h"
#include "VertexBuffer.h"
#include "ConstantBuffer.h"
#include "GameObject.h"
#include <vector>
#include <d3d11.h>
#include <SimpleMath.h>
using namespace DirectX;
class MeshOb;

class Enemy
{
public:
	int hp = 3;
	float speed = 0.008f;;
	std::vector<XMFLOAT3> waypoints;
	SimpleMath::Vector3 moveVec;
};

class Tower
{
public:
	float dmg  = 1;
	float radius = 3;
	float projectileSpeed = 0.2;
};
class MeshOb : public GameObject
{
public:
	MeshOb();
	 ~MeshOb();
	bool Initialize(ID3D11Device * device, ID3D11DeviceContext * deviceContext,
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
	//tower
	void InitTower();
	bool InRange(MeshOb* incomingOb);
	void Shoot(MeshOb* incomingOb);

private:
	void UpdateMatrix();
	std::string type;
	Color color;
	/* constant buffers*/
	ConstantBuffer<CB_PS_Color> cb_vs_Color;
	ConstantBuffer<CB_VS_vertexshader> cb_vs_vertexshader;

	ID3D11Device* device = nullptr;
	ID3D11DeviceContext* deviceContext = nullptr;

	//Enemy
	Enemy* enemy;
	Tower* tower;
};
#endif // !