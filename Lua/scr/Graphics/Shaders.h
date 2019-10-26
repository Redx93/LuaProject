#pragma once
#include "..\\ErrorLogger.h"
#pragma comment(lib, "D3DCompiler.lib")
#include <d3d11.h>
#include <wrl/client.h>
#include <d3dcompiler.h>

class VertexShader
{
public:
	bool Initialize(Microsoft::WRL::ComPtr<ID3D11Device> &device, std::wstring shaderpath, D3D11_INPUT_ELEMENT_DESC * layoutDesc, UINT numElements);
	ID3D11VertexShader * GetShader();
	ID3D10Blob * GetBuffer();
	ID3D11InputLayout * GetInputLayout();
private:
	Microsoft::WRL::ComPtr<ID3D11VertexShader> shader;
	Microsoft::WRL::ComPtr<ID3D10Blob> shader_buffer;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> inputLayout;
};

class PixelShader
{
public:
	bool Initialize(Microsoft::WRL::ComPtr<ID3D11Device> &device, std::wstring shaderpath);
	ID3D11PixelShader * GetShader();
	ID3D10Blob * GetBuffer();
private:
	Microsoft::WRL::ComPtr<ID3D11PixelShader> shader;
	Microsoft::WRL::ComPtr<ID3D10Blob> shader_buffer;
};

class GeometryShader
{
public:
	bool CreatGeometryShader(ID3D11Device* device, std::wstring fileName);
	ID3D11GeometryShader* GetShader() { return shader.Get(); }
	ID3D10Blob* GetBuffer() { return buffer.Get(); }
private:
	Microsoft::WRL::ComPtr<ID3D11GeometryShader> shader;
	Microsoft::WRL::ComPtr<ID3D10Blob> buffer;
};


class Shader
{
public:
	bool InitializePixelShader(Microsoft::WRL::ComPtr<ID3D11Device>& , std::wstring );
	bool InitializeVertexShader(Microsoft::WRL::ComPtr<ID3D11Device>& , std::wstring , D3D11_INPUT_ELEMENT_DESC* , UINT );

	PixelShader PS;
	VertexShader VS;
};