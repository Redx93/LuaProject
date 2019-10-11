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
		cb_vs_vertexshader.Initialize(device, deviceContext);
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
	
	cb_vs_vertexshader.data.mat = XMMatrixTranspose(worldMatrix);
	cb_vs_vertexshader.ApplyChanges();
	this->deviceContext->VSSetConstantBuffers(1, 1, this->cb_vs_vertexshader.GetAddressOf());

	UINT offset = 0;
	this->deviceContext->IASetVertexBuffers(0, 1, this->vertexBuffer.GetAddressOf(), this->vertexBuffer.Stride(), &offset);
	this->deviceContext->Draw(36, 0);
}

void RenderbleGameObject::SetColor(Color color)
{
	cb_vs_Color.data.color = color;
	this->cb_vs_Color.ApplyChanges();
}


void RenderbleGameObject::SetType(std::string type)
{
	if (type == "Environment")
	{
		SetColor(Colours::White);
	}
	else if (type == "Enemy")
	{
		SetColor(Colours::Red);
	}
	else if (type == "Player")
	{
		SetColor(Colours::Blue);
	}
	else if (type == "Teleport")
	{
		SetColor(Colours::Grey);
	}
	this->type = type;
}

std::string RenderbleGameObject::GetType()
{
	return this->type;
}
void RenderbleGameObject::UpdateMatrix()
{
	this->worldMatrix = XMMatrixRotationRollPitchYaw(this->rot.x, this->rot.y, this->rot.z)
		* XMMatrixTranslation(this->pos.x, this->pos.y, this->pos.z)
		* XMMatrixScaling(this->scale.x, this->scale.y, this->scale.z);
}
