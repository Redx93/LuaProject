#pragma once
#include "Vertex.h"
#include "VertexBuffer.h"
#include "ConstantBuffer.h"
#include "GameObject.h"
#include <vector>
using namespace DirectX;

class RenderbleGameObject : public GameObject
{
public:
	/*Mesh(MeshType type = MeshType::Environment):type(type) { }
	Mesh(const Mesh& other);
	Mesh& operator=(const Mesh other)*/
	RenderbleGameObject();
	 ~RenderbleGameObject();
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
private:

	void UpdateMatrix();
	std::string type;
	Color color;
	ID3D11Device * device = nullptr;
	ID3D11DeviceContext * deviceContext = nullptr;

	ConstantBuffer<CB_PS_Color> cb_vs_Color;
	ConstantBuffer<CB_VS_vertexshader> cb_vs_vertexshader;
};