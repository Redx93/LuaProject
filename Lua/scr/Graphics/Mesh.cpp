#include "Mesh.h"

#include<vector> // for vector 
#include<algorithm> // for copy() and assign() 
#include<iterator> // for back_inserter 
#include <iostream>
using namespace std;

RenderbleGameObject::RenderbleGameObject()
{
	device = nullptr;
	deviceContext = nullptr;
}

RenderbleGameObject::~RenderbleGameObject()
{	

}

bool RenderbleGameObject::Initialize(ID3D11Device * device, ID3D11DeviceContext * deviceContext, 
	Color color)
{
	this->device = device;
	this->deviceContext = deviceContext;

	try
	{
		//Textured Square
		vertices.resize(36);
		vertices =
		{
			Vertex(-0.5f,  -0.5f, -0.5f), //FRONT Bottom Left   - [0]
			Vertex(-0.5f,   0.5f, -0.5f), //FRONT Top Left      - [1]
			Vertex(0.5f,   0.5f, -0.5f), //FRONT Top Right     - [2]

			Vertex(-0.5f,  -0.5f, -0.5f), //FRONT Bottom Left   - [0]
			Vertex(0.5f,   0.5f, -0.5f), //FRONT Top Right     - [2]
			Vertex(0.5f,  -0.5f, -0.5f), //FRONT Bottom Right   - [3]

			Vertex(-0.5f,  -0.5f, 0.5f), //BACK Bottom Left   - [4]
			Vertex(0.5f,  -0.5f, 0.5f), //BACK Bottom Right   - [7]
			Vertex(0.5f,   0.5f, 0.5f), //BACK Top Right     - [6]

			Vertex(-0.5f,  -0.5f, 0.5f), //BACK Bottom Left   - [4]
			Vertex(0.5f,   0.5f, 0.5f), //BACK Top Right     - [6]
			Vertex(-0.5f,   0.5f, 0.5f), //BACK Top Left      - [5]
		
			Vertex(0.5f,  -0.5f, -0.5f), //FRONT Bottom Right   - [3]
			Vertex(0.5f,   0.5f, -0.5f), //FRONT Top Right     - [2]
			Vertex(0.5f,   0.5f, 0.5f), //BACK Top Right     - [6]

			Vertex(0.5f,  -0.5f, -0.5f), //FRONT Bottom Right   - [3]
			Vertex(0.5f,   0.5f, 0.5f), //BACK Top Right     - [6]
			Vertex(0.5f,  -0.5f, 0.5f), //BACK Bottom Right   - [7]

			Vertex(-0.5f,  -0.5f, 0.5f), //BACK Bottom Left   - [4]
			Vertex(-0.5f,   0.5f, 0.5f), //BACK Top Left      - [5]
			Vertex(-0.5f,   0.5f, -0.5f), //FRONT Top Left      - [1]

			Vertex(-0.5f,  -0.5f, 0.5f), //BACK Bottom Left   - [4]
			Vertex(-0.5f,   0.5f, -0.5f), //FRONT Top Left      - [1]
			Vertex(-0.5f,  -0.5f, -0.5f), //FRONT Bottom Left   - [0]

			Vertex(-0.5f,   0.5f, -0.5f), //FRONT Top Left      - [1]
			Vertex(-0.5f,   0.5f, 0.5f), //BACK Top Left      - [5]
			Vertex(0.5f,   0.5f, 0.5f), //BACK Top Right     - [6]

			Vertex(-0.5f,   0.5f, -0.5f), //FRONT Top Left      - [1]
			Vertex(0.5f,   0.5f, 0.5f), //BACK Top Right     - [6]
			Vertex(0.5f,   0.5f, -0.5f), //FRONT Top Right     - [2]

			Vertex(-0.5f,  -0.5f, -0.5f), //FRONT Bottom Left   - [0]
			Vertex(0.5f,  -0.5f, -0.5f), //FRONT Bottom Right   - [3]
			Vertex(0.5f,  -0.5f, 0.5f), //BACK Bottom Right   - [7]

			Vertex(-0.5f,  -0.5f, -0.5f), //FRONT Bottom Left   - [0]
			Vertex(0.5f,  -0.5f, 0.5f), //BACK Bottom Right   - [7]
			Vertex(-0.5f,  -0.5f, 0.5f), //BACK Bottom Left   - [4]
		};

		//Load Vertex Data
		HRESULT hr = this->vertexBuffer.Initialize(this->device, vertices.data(), vertices.size());
		COM_ERROR_IF_FAILED(hr, "Failed to initialize vertex buffer.");

		//DWORD indices[] =
		//{
		//	0, 1, 2, //FRONT
		//	0, 2, 3, //FRONT
		//	4, 7, 6, //BACK 
		//	4, 6, 5, //BACK
		//	3, 2, 6, //RIGHT SIDE
		//	3, 6, 7, //RIGHT SIDE
		//	4, 5, 1, //LEFT SIDE
		//	4, 1, 0, //LEFT SIDE
		//	1, 5, 6, //TOP
		//	1, 6, 2, //TOP
		//	0, 3, 7, //BOTTOM
		//	0, 7, 4, //BOTTOM
		//};
		this->IndexCount =36;

		//Load Index Data
		/*hr = this->indexBuffer.Initialize(this->device, indices, IndexCount);
		COM_ERROR_IF_FAILED(hr, "Failed to initialize index buffer.");*/

		hr = cb_vs_Color.Initialize(device, deviceContext);
		COM_ERROR_IF_FAILED(hr, "Failed to initialize Model Color.");

		this->color = color;
		this->cb_vs_Color.data.color = color;
		this->cb_vs_Color.ApplyChanges();
	}
	catch (COMException & exception)
	{
		ErrorLogger::Log(exception);
		return false;
	}

	this->SetPosition(0.0f, 0.0f, 0.0f);
	this->SetRotation(0.0f, 0.0f, 0.0f);
	this->SetScale(1.0f, 1.0f, 1.0f);
	this->UpdateMatrix();
	return true;
}


void RenderbleGameObject::Draw()
{	
	//Update Color
	this->deviceContext->PSSetConstantBuffers(0, 1, this->cb_vs_Color.GetAddressOf());

	UINT offset = 0;
	this->deviceContext->IASetVertexBuffers(0, 1, this->vertexBuffer.GetAddressOf(), this->vertexBuffer.Stride(), &offset);
	this->deviceContext->Draw(36, 0);
}

void RenderbleGameObject::SetColor(Color color)
{
	cb_vs_Color.data.color = color;
	this->cb_vs_Color.ApplyChanges();
}

void RenderbleGameObject::SetColor(MeshType type)
{
	switch (type)
	{
	case MeshType::Environment :
		this->SetColor(Colours::White);
		color = Colours::White;
		break;
	case MeshType::Enemy:
		this->SetColor(Colours::Red);
		color = Colours::Red;
		break;
	case MeshType::Player :
		this->SetColor(Colours::Blue);
		color = Colours::Blue;
		break;
	case MeshType::Teleport :
		this->SetColor(Colours::Green);
		color = Colours::Green;
		break;

	default:
		this->SetColor(Colours::White);
		color = Colours::White;
		break;
	}
}

void RenderbleGameObject::SetType(MeshType type)
{
	this->type = type;
}

Color RenderbleGameObject::GetColor()
{
	return color;
}

MeshType RenderbleGameObject::GetMeshType(const unsigned char keycode)
{
	MeshType type;
	switch (keycode)
	{
	case '1':
		type = MeshType::Environment;
		break;
	case '2':
		type = MeshType::Enemy;
		break;
	case '3':
		type = MeshType::Player;
		break;
	case '4':
		type = MeshType::Teleport;
		break;

	default:
		type = MeshType::Environment;
		break;
	}
	return type;
}

MeshType RenderbleGameObject::GetMeshType(std::string keycode)
{
	MeshType type = MeshType::Environment;
	if (keycode == "Environment")
	{
		type = MeshType::Environment;
	}
	else if (keycode == "Enemy")
	{
		type = MeshType::Enemy;
	}
	else if (keycode == "Player")
	{
		type = MeshType::Player;
	}
	else if (keycode == "Teleport")
	{
		type = MeshType::Teleport;
	}
	return type;
}

std::string RenderbleGameObject::GetMeshType(MeshType type)
{
	std::string mesh = "Environment";
	if (type == MeshType::Environment)
	{
		mesh = "Environment";
	}
	else if (type == MeshType::Enemy)
	{
		mesh = "Enemy";
	}
	else if (type == MeshType::Player)
	{
		mesh = "Player";
	}
	else if (type == MeshType::Teleport)
	{
		mesh = "Teleport";
	}
	return mesh;
}


void RenderbleGameObject::UpdateMatrix()
{
	this->worldMatrix = XMMatrixRotationRollPitchYaw(this->rot.x, this->rot.y, this->rot.z)
		* XMMatrixTranslation(this->pos.x, this->pos.y, this->pos.z)
		* XMMatrixScaling(this->scale.x, this->scale.y, this->scale.z);
}
