#pragma once

#include "ImGUI\\imgui.h"
#include "ImGUI\\imgui_impl_win32.h"
#include "ImGUI\\imgui_impl_dx11.h"
#include "Mesh.h"
#include "D3DBase.h"
class Graphics : public D3DBase
{
public:
	Graphics(){}
	~Graphics();
	bool Initialize(HWND hwnd, int width, int height, MouseClass* mouse ,
		KeyboardClass* keyboard);
	bool InitizlizeGrid();
	void RenderFrame();
	void UpdateGrid();

	VertexBuffer<Vertex_COLOR> vb_grid;
	std::vector<RenderbleGameObject*> models;
private:
	bool InitializeScene();
	//grid
	float gridDimension = 10.0f;
	int gridSections = 10;

	//imgui
	bool renderGrid = true;
	bool FollowMouse = true;
};