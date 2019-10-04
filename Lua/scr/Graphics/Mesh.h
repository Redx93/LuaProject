#pragma once
#include "Vertex.h"
#include "VertexBuffer.h"
#include "ConstantBuffer.h"
#include "GameObject.h"
#include <vector>
using namespace DirectX;

enum MeshType
{
	Environment,
	Enemy,
	Player,
	Teleport
};
class RenderbleGameObject : public GameObject
{
public:
	/*Mesh(MeshType type = MeshType::Environment):type(type) { }
	Mesh(const Mesh& other);
	Mesh& operator=(const Mesh other)*/
	RenderbleGameObject();
	 ~RenderbleGameObject();
	bool Initialize(ID3D11Device * device, ID3D11DeviceContext * deviceContext,
		Color color = Colours::White);
	void Draw();
	void SetColor(Color color);
	void SetColor(MeshType type);
	void SetType(MeshType type);
	Color GetColor();
	MeshType GetMeshType(const unsigned char keycode);
	MeshType GetMeshType(std::string keycode);
	std::string GetMeshType(MeshType type);
	MeshType GetType() { return this->type; }


	XMMATRIX worldMatrix = XMMatrixIdentity();
	std::vector<Vertex> vertices;
	VertexBuffer<Vertex> vertexBuffer;
	UINT IndexCount = 0;
private:
	void UpdateMatrix();

	MeshType type;
	Color color;
	ID3D11Device * device = nullptr;
	ID3D11DeviceContext * deviceContext = nullptr;

	ConstantBuffer<CB_PS_Color> cb_vs_Color;
	//IndexBuffer indexBuffer;
};