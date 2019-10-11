#include "Graphics.h"

Graphics::~Graphics()
{
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
	delete engine;
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

	//Setup ImGui
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	ImGui_ImplWin32_Init(hwnd);
	ImGui_ImplDX11_Init(this->device.Get(), this->deviceContext.Get());
	ImGui::StyleColorsDark();

	// script
	engine = new LuaEngine();
	meshManager.Init(device.Get(),deviceContext.Get());
	meshManager.AddScript(engine->L());
	engine->ExecuteFile("mainLua.lua");
	engine->CallGlobalVariable("ReadFile");
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
void Graphics::RenderFrame()
{
	float bgcolor[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	this->deviceContext->ClearRenderTargetView(this->renderTargetView.Get(), bgcolor);
	this->deviceContext->ClearDepthStencilView(this->depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	this->SetupShader(this->DefaultShader, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//update camera buffer
	this->UpdateConstantBuffer();
	
	engine->CallGlobalVariable("update");
	{ 
	////picking get the ray
	//Ray ray = inputHandler->GetRay(mouse->GetPosX(), mouse->GetPosY());
	//{
	//	
	//	char keyCode = this->keyboard->ReadKey().GetKeyCode();
	//	//Models
	//	unsigned char buttonPressed = 'l';
	//	if (this->CurrentModels == nullptr &&
	//		this->KeyBoardIsPressed(buttonPressed) && 
	//		fpsTimer.GetAsSeconds() >= 0.6)
	//	{
	//		if (buttonPressed != 'l')
	//		{
	//		MeshType type = this->GetMeshType(buttonPressed);
	//		RenderbleGameObject* m  = new RenderbleGameObject();
	//		m->Initialize(device.Get(), deviceContext.Get());
	//		m->SetType(type);
	//		m->SetColor(type);

	//		this->models.push_back(m);
	//		UINT index = models.size() - 1;
	//		this->CurrentModels = models[models.size() - 1];
	//		}
	//	}
	//	//Mouse Intersection
	//	for (auto& mesh : models)
	//	{
	//		Intersect = inputHandler->Picking(ray, mesh);
	//		if (CurrentModels != nullptr)
	//		{
	//			inputHandler->FollowMouse(ray, CurrentModels);
	//			if (mouseEvent(MouseEvent::EventType::RPress))
	//			{
	//				CurrentModels = nullptr;
	//				this->SaveLevel(this->models);
	//				fpsTimer.Restart();
	//			}
	//		}
	//		else if(this->CurrentModels == nullptr)
	//		{
	//			if (mouseEvent(MouseEvent::EventType::LPress) && Intersect)
	//			{
	//				this->CurrentModels = mesh;
	//			}
	//		}
	//	}
	//}

	//Draw
	//for (auto& mesh : models)
	//{
	//	this->UpdateConstantBuffer(mesh);
	//	mesh->Draw();
	////}
	}

	{
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();
		//Create ImGui Test Window
		ImGui::Begin("InputManger");
		ImGui::Text("MousePosition ( %i , %i )", mouse->GetPosX(), mouse->GetPosY());
		ImGui::Checkbox("Render Grid", &renderGrid);
		if (renderGrid)
		{
			this->UpdateGrid();
		}
		ImGui::Checkbox("Intersect with model", &Intersect);	
		ImGui::End();
	}
	// Imgui editor
	{
		ImGui::Begin("Editor");   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
		ImGui::SetWindowFontScale(1.5);
		ImGui::Text(
			"Mouse :\nLeftMouse = Select\nRightMouse = Deselect\n\nButtons :\n1 = Environment\n2 = Enemy\n3 = Player\n4 = Teleport\n");
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


bool Graphics::InitializeScene()
{
	try
	{
		//Initialize Constant Buffer(s)
		HRESULT hr = this->cb_vs_vertexshader.Initialize(this->device.Get(), this->deviceContext.Get());
		COM_ERROR_IF_FAILED(hr, "Failed to initialize constant buffer.");

		hr = this->cb_ps_pixelshader.Initialize(this->device.Get(), this->deviceContext.Get());
		COM_ERROR_IF_FAILED(hr, "Failed to initialize constant buffer.");

		camera.SetPosition(0.0f, 0.0f, -2.0f);
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