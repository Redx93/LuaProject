#include "Loader.h"

std::string Loader::GetFloat3AsString(XMFLOAT3 p)
{
	std::string str;
	str = std::to_string(p.x) + " " + std::to_string(p.y) + " "+
		std::to_string(p.z) + "\n";
	return str;
}

std::string Loader::GetColorAsString(Color  c)
{
	std::string str;
	str = std::to_string(c.r) + " " + std::to_string(c.g) + " " +
		std::to_string(c.b) + "\n";
	return str;
}

void Loader::Initialize(Microsoft::WRL::ComPtr<ID3D11Device> device, Microsoft::WRL::ComPtr<ID3D11DeviceContext> deviceContext)
{
	this->device = device;
	this->deviceContext = deviceContext;
}

void Loader::SaveLevel(std::vector<RenderbleGameObject*> meshes)
{
	std::ofstream file;
	std::string temp;
	file.open("Level.txt");
	for (int i = 0; i < meshes.size(); i++)
	{
		RenderbleGameObject* mesh = meshes[i];
		temp = mesh->GetMeshType(mesh->GetType());
		file << temp << " " << GetFloat3AsString(mesh->GetPositionFloat3());
	}
	file.close();
	
}

void Loader::LoadMap(std::vector<RenderbleGameObject*>&meshes)
{

	for (int i = 0; i < meshes.size(); i++)
	{
		delete meshes[i];
	}
	meshes.clear();

	std::ifstream file;
	MeshType meshType;
	XMFLOAT3 position;
	//std::string strtype, x, y, z;
	file.open("Level.txt");
	if (file.is_open())
	{
		while(!file.eof())
		{ 
			std::string strtype, x, y, z;
			file >> strtype >>	x >> y >> z;
			if (strtype != "")
			{
				position = XMFLOAT3(std::atof(x.c_str()), std::atof(y.c_str()), std::atof(z.c_str()));
				RenderbleGameObject* mesh = new RenderbleGameObject();
				mesh->Initialize(device.Get(), deviceContext.Get());
				meshType = mesh->GetMeshType(strtype);
				mesh->SetType(meshType);

				mesh->SetColor(meshType);
				mesh->SetPosition(position);
				meshes.push_back(mesh);
			}
		}
		file.close();
	}

}
