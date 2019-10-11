#pragma once
#include "scr/Graphics/Mesh.h"
#include <vector>
#include <fstream>
class Loader
{
private:
	std::string GetFloat3AsString(XMFLOAT3);
	std::string GetColorAsString(Color);

	Microsoft::WRL::ComPtr<ID3D11Device> device;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> deviceContext;
public:
	void Initialize(Microsoft::WRL::ComPtr<ID3D11Device> device, Microsoft::WRL::ComPtr<ID3D11DeviceContext> deviceContext);
	void SaveLevel(std::vector<MeshOb*> meshes);
	void LoadMap(std::vector<MeshOb*>& meshes);

};

