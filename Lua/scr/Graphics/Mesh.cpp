#include "Mesh.h"

#include<vector> // for vector 
#include<algorithm> // for copy() and assign() 
#include<iterator> // for back_inserter 
#include <iostream>
using namespace std;

MeshOb::MeshOb()
{
	device = nullptr;
	deviceContext = nullptr;
	enemy = nullptr;
}

MeshOb::~MeshOb()	
{
	if (enemy != nullptr)
		delete enemy;
}

bool MeshOb::Initialize(ID3D11Device * device, ID3D11DeviceContext * deviceContext, 
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
	////Default value, test
	this->SetPosition(0.0f, 0.0f, 0.0f);
	this->SetRotation(0.0f, 0.0f, 0.0f);
	this->SetScale(1.0f, 1.0f, 1.0f);
	this->UpdateMatrix();
	return true;
}


void MeshOb::Draw()
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

void MeshOb::SetColor(Color color)
{
	cb_vs_Color.data.color = color;
	this->cb_vs_Color.ApplyChanges();
}


void MeshOb::SetType(std::string type)
{
	if (type == "Environment" || type == "1")
	{
		SetColor(Colours::Grey);
		this->type = "Environment";
	}
	else if (type == "Enemy" || type == "2")
	{
		SetColor(Colours::Red);
		this->type = "Enemy";
	}
	else if (type == "Tower" || type == "3")
	{
		SetColor(Colours::Blue);
		this->type = "Tower";
	}
	else if (type == "Waypoint" || type == "4")
	{
		SetColor(Colours::White);
		this->type = "Waypoint";
	}
}

std::string MeshOb::GetType()
{
	return this->type;
}
void MeshOb::InitEnemy()
{
	this->enemy = new Enemy();
}
bool MeshOb::Update(float dt)
{
	bool result = false;
	SimpleMath::Vector3 enemyPos = this->GetPositionFloat3();
	
	SimpleMath::Vector3 currentTargetWP = enemy->waypoints.front(); //confirm if this is target wp
	//sqrtf(x*x+y*y+z*z);
	float length = SimpleMath::Vector3::Distance(enemyPos, currentTargetWP);

	if (length < 0.5)
		CalcNewWP();

	SimpleMath::Vector3 move = enemy->moveVec;
	this->AdjustPosition(move * enemy->speed * dt);
	//this->SetPosition(-2.5, 2.5, 0);
	return this->enemy->waypoints.empty();
}
void MeshOb::CalcNewWP()
{
	//XMFLOAT3 from;
	//XMFLOAT3 to;
	//from = enemy->waypoints.front();
	//enemy->waypoints.erase(enemy->waypoints.begin());

	//if (enemy->waypoints.empty() != true)
	//	to = enemy->waypoints.front();
	////else
	////	to = { 0.f,0.f,0.f }; //debug waypoint object goes to 0
	////don't delete front of list (used for next round of start/end)

	//SimpleMath::Vector3 start = from;
	//SimpleMath::Vector3 end = to;
	//enemy->moveVec = end - start;
	//enemy->moveVec.Normalize();
	//enemy->moveVec = enemy->moveVec * enemy->speed;

	SimpleMath::Vector3 start = enemy->waypoints[0];
	SimpleMath::Vector3 goal = enemy->waypoints[1];
	enemy->waypoints.erase(enemy->waypoints.begin());
	SimpleMath::Vector3 Dir = goal - start;
	Dir.Normalize();
	enemy->moveVec = Dir;

	
}
void MeshOb::initWaypoints(std::vector<XMFLOAT3> newList)
{
	this->enemy->waypoints = newList; //merge list into this->list
}
void MeshOb::UpdateMatrix()
{
	this->worldMatrix = XMMatrixRotationRollPitchYaw(this->rot.x, this->rot.y, this->rot.z)
		* XMMatrixTranslation(this->pos.x, this->pos.y, this->pos.z)
		* XMMatrixScaling(this->scale.x, this->scale.y, this->scale.z);
}
