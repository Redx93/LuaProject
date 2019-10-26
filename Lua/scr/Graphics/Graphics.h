#pragma once

#include "ImGUI\\imgui.h"
#include "ImGUI\\imgui_impl_win32.h"
#include "ImGUI\\imgui_impl_dx11.h"
#include "Mesh.h"
#include "D3DBase.h"
#include "..//LuaEngine.h"
#include "MeshManger.h"
#include "..//InputManager.h"
#include "ProjectileManager.h"
class Graphics : public D3DBase
{
public:
	Graphics();
	~Graphics();
	bool Initialize(HWND hwnd, int width, int height, MouseClass* mouse ,
		KeyboardClass* keyboard);
	bool InitizlizeGrid();
	void RenderFrame();
	void UpdateGrid();
	void ResetScript();

	static ProjectileManager* projectileManager;
	VertexBuffer<Vertex_COLOR> vb_grid;
	ID3D11Device* GetDevice() { return this->device.Get(); }
	ID3D11DeviceContext* GetDeviceContext() { return this->deviceContext.Get(); }
private:
	LuaEngine* engine;
	MeshManger *meshManager;
	InputManager* inputManager;
	

	bool InitializeScene();
	//grid
	float gridDimension = 10.0f;
	int gridSections = 10;
	//imgui
	bool renderGrid = true;

	Timer enemySpawnTimer;
	Timer timer;
};