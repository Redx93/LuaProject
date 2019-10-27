#include "Graphics.h"
ProjectileManager* Graphics::projectileManager = nullptr;
Graphics::Graphics()
{
	timer.Start();
	
}

Graphics::~Graphics()
{
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
	delete engine;
	delete inputManager;
	delete projectileManager;
	delete meshManager;
}

bool Graphics::Initialize(HWND hwnd, int width, int height,
	MouseClass * mouse,KeyboardClass *Keyboard)
{

	if (!InitializeD3DBase(hwnd, width, height))
		return false;
	if (!InitializeScene())
		return false;
	if (!InitizlizeGrid())
		return false;

	this->mouse = mouse;
	this->keyboard = Keyboard;
	//this->inputManager = new InputManager(this->mouse,this->keyboard,&this->camera, width, height);

	//this->projectileManager = new ProjectileManager(this->device.Get(),this->deviceContext.Get());

	//Setup ImGui
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	ImGui_ImplWin32_Init(hwnd);
	ImGui_ImplDX11_Init(this->device.Get(), this->deviceContext.Get());
	ImGui::StyleColorsDark();

	// script
	//meshManager.Init(device.Get(),deviceContext.Get(),&this->timer);
	ResetScript();

	//engine = new LuaEngine();
	//meshManager.AddScript(engine->L());
	//inputManager->AddScript(engine->L());
	//engine->ExecuteFile("mainLua.lua");
	//inputManager->setValues();
	//engine->CallGlobalVariable("ReadFile");
	//engine->CallGlobalVariable("spawnEnemy");
	return true;
}

bool Graphics::InitizlizeGrid()
{
	try
	{
		//draw x grid
		std::vector<Vertex_COLOR> v2;
		float alpha = 1.0f;
		const Color& color = Colours::Blue;
		//Draw -Z -> +Z (Red->White) //[X-Z Axis]
		for (int i = 0; i < gridSections; i++)
		{
			v2.push_back(Vertex_COLOR(0.0f - gridDimension / 2 + i * gridDimension / gridSections, 0.0f, -gridDimension / 2.0f, color));
			v2.push_back(Vertex_COLOR(0.0f - gridDimension / 2 + i * gridDimension / gridSections, 0.0f, +(gridDimension) / 2.0f - gridDimension / gridSections, color));
		}
		//Draw -X -> +X (Blue->White)  //[X-Z Axis]
		for (int i = 0; i < gridSections; i++)
		{
			v2.push_back(Vertex_COLOR(-gridDimension / 2.0f, 0.0f, 0.0f - gridDimension / 2 + i * gridDimension / gridSections, color));
			v2.push_back(Vertex_COLOR(+gridDimension / 2.0f - gridDimension / gridSections, 0.0f, 0.0f - gridDimension / 2 + i * gridDimension / gridSections, color));
		}
		//Draw -X -> +X (Blue->White) //[X-Y Axis]
		for (int i = 0; i < gridSections; i++)
		{
			v2.push_back(Vertex_COLOR(-gridDimension / 2.0f, 0.0f - gridDimension / 2 + i * gridDimension / gridSections, 0.0f, color));
			v2.push_back(Vertex_COLOR(+gridDimension / 2.0f - gridDimension / gridSections, 0.0f - gridDimension / 2 + i * gridDimension / gridSections, 0.0f, color));
		}
		//Draw -Y -> +Y (Green->White) //[X-Y Axis]
		for (int i = 0; i < gridSections; i++)
		{
			v2.push_back(Vertex_COLOR(0.0f - gridDimension / 2 + i * gridDimension / gridSections, -gridDimension / 2.0f, 0.0f, color));
			v2.push_back(Vertex_COLOR(0.0f - gridDimension / 2 + i * gridDimension / gridSections, +gridDimension / 2.0f - gridDimension / gridSections, 0.0f, color));
		}
		HRESULT hr = vb_grid.Initialize(this->device.Get(), v2.data(), v2.size());
		COM_ERROR_IF_FAILED(hr, "Failed to initialize Grid.");
	}
	catch (COMException& exception)
	{
		ErrorLogger::Log(exception);
		return false;
	}
	return true;
}
bool Intersect = false;
static int current = 1;
static int hasScriptChanged = -1;
static float timePassed = 0.f;
void Graphics::RenderFrame()
{
	float dt = timer.GetMilisecondsElapsed();
	timer.Restart();
	float bgcolor[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	this->deviceContext->ClearRenderTargetView(this->renderTargetView.Get(), bgcolor);
	this->deviceContext->ClearDepthStencilView(this->depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	this->SetupShader(this->DefaultShader, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//update camera buffer
	this->UpdateConstantBuffer();
	this->projectileManager->update(dt);
	{
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();
		//Create ImGui Test Window
		ImGui::Begin("InputManger");
		ImGui::Text("MousePosition ( %i , %i )", mouse->GetPosX(), mouse->GetPosY());
		ImGui::Checkbox("Render Grid", &renderGrid);
	
		//ImGui::Text("number of meshes : %i", meshManager.GetNumberOfMeshses());
		ImGui::Checkbox("Intersect with model", &Intersect);	
		ImGui::End();
	}
	{
		//menu in imgui
		ImGui::Begin("Menu");   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
		ImGui::SetWindowFontScale(1.5);
		ImGui::SliderInt("Menu State", &current, 1, 2);
		if (projectileManager->EndGame() == true)
			current = 1;
		if (current==1)
		{
			ImGui::Text("Edit Phase");
			this->ResetScript();
			engine->CallGlobalVariable("update");
			timePassed = 0;
		}
		else if (current == 2)
		{
			timePassed += dt/1000;
			ImGui::Text("Game Phase");
			ImGui::Text("Time: %f", timePassed);
			ImGui::Text("Score: %d", projectileManager->GetScore());
			this->ResetScript();
			engine->CallGlobalVariable("gamePhase");	
		}

		ImGui::End();

	}
	if (renderGrid)
	{
		this->UpdateGrid();
	}
	// Imgui editor
	{
		ImGui::Begin("Editor");   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
		ImGui::SetWindowFontScale(1.5);
		ImGui::Text(
			"Mouse :\nLeftMouse = Select\nRightMouse = Deselect\n\nButtons :\n1 = Environment\n2 = Enemy\n3 = Tower\n4 = Waypoint\n");
		bool SaveLevel = false;
		ImGui::Checkbox("Save Level", &SaveLevel);
		if (SaveLevel && fpsTimer.GetMilisecondsElapsed() >= 1)
		{
			engine->CallGlobalVariable("WritetoFile");
		}
		bool LoadLevel = false;
		ImGui::Checkbox("Load Level", &LoadLevel);
		if (LoadLevel && fpsTimer.GetMilisecondsElapsed() >= 1)
		{
			engine->CallGlobalVariable("ReadFile");
		}
		ImGui::End();
		ImGui::Render();
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	}
	this->swapchain->Present(1, NULL);
}

void Graphics::UpdateGrid()
{
	this->SetupShader(this->ColorShader, D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
	//Update Constant buffer with WVP Matrix
	XMMATRIX world = XMMatrixIdentity();
	this->cb_vs_vertexshader.data.mat = world * camera.GetViewMatrix() * camera.GetProjectionMatrix(); //Calculate World-View-Projection Matrix
	this->cb_vs_vertexshader.data.mat = XMMatrixTranspose(this->cb_vs_vertexshader.data.mat);
	this->cb_vs_vertexshader.ApplyChanges();
	this->deviceContext->VSSetConstantBuffers(0, 1, this->cb_vs_vertexshader.GetAddressOf());

	UINT offset = 0;
	UINT stride2 = sizeof(Vertex_COLOR);
	static bool renderXYaxis = true;
	static bool renderXZaxis = false;
	this->deviceContext->IASetVertexBuffers(0, 1, vb_grid.GetAddressOf(), vb_grid.Stride(), &offset);
	if (renderXZaxis)
		this->deviceContext->Draw(this->vb_grid.VertexCount() / 2, 0);
	if (renderXYaxis)
		this->deviceContext->Draw(this->vb_grid.VertexCount() / 2, this->vb_grid.VertexCount() / 2);
}


void Graphics::ResetScript()
{
	if (current != hasScriptChanged)
	{
		if (inputManager != nullptr)
			delete inputManager;
		if (projectileManager != nullptr)
			delete projectileManager;
		if(engine != nullptr)
			delete this->engine;
		if (meshManager != nullptr)
			delete meshManager;

		this->inputManager = new InputManager(this->mouse, this->keyboard,&this->camera, windowWidth, windowHeight);
		this->projectileManager = new ProjectileManager(this->device.Get(),this->deviceContext.Get());
		this->meshManager = new MeshManger();
		meshManager->Init(device.Get(), deviceContext.Get(), &this->timer);

		engine = new LuaEngine();
		meshManager->AddScript(engine->L());
		inputManager->AddScript(engine->L());
		engine->ExecuteFile("mainLua.lua");
		inputManager->setValues();
		engine->CallGlobalVariable("ReadFile");
		if (current == 2)
		{		engine->CallGlobalVariable("spawnEnemy");
			this->projectileManager->GetEnemies(this->meshManager->GetEnemies());
		}
	

		hasScriptChanged = current;
	}


}



bool Graphics::InitializeScene()
{
	try
	{
		//Initialize Constant Buffer(s)
		HRESULT hr = this->cb_vs_vertexshader.Initialize(this->device.Get(), this->deviceContext.Get());
		COM_ERROR_IF_FAILED(hr, "Failed to initialize constant buffer.");

		hr = this->cb_ps_pixelshader.Initialize(this->device.Get(), this->deviceContext.Get());
		COM_ERROR_IF_FAILED(hr, "Failed to initialize constant buffer.");

		camera.SetPosition(0.0f, 0.0f, -3.0f);
		camera.SetProjectionValues(90.0f, static_cast<float>(windowWidth) /
			static_cast<float>(windowHeight), 0.1f, 1000.0f);
	}
	catch (COMException & exception)
	{
		ErrorLogger::Log(exception);
		return false;
	}
	return true;
}