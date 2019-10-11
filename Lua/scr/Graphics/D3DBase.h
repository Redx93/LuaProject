
#include <SpriteBatch.h>
#include <SpriteFont.h>
#include <WICTextureLoader.h>
#include "Shaders.h"
#include "..//Timer.h"
#include "Camera.h"
#include "..//InputHandler.h"
#include "..//Mouse/MouseClass.h"
#include "..//Mouse/MouseEvent.h"
#include "..//Keyboard/KeyboardClass.h"

class D3DBase
{
protected:
	D3DBase(){}
	~D3DBase();
	bool InitializeD3DBase(HWND hwnd, int width, int height);
	bool InitializeDirectX(HWND hwnd);
	bool InitializeShaders();
	void SetupShader(Shader shader, D3D11_PRIMITIVE_TOPOLOGY );
	void UpdateConstantBuffer();

	bool KeyBoardIsPressed(unsigned char& keycode);

	bool mouseEvent(MouseEvent::EventType type);
	MouseClass* mouse = nullptr;
	KeyboardClass *keyboard = nullptr;
	MeshOb* CurrentModels = nullptr;
	Microsoft::WRL::ComPtr<ID3D11Device> device;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> deviceContext;
	Microsoft::WRL::ComPtr<IDXGISwapChain> swapchain;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> renderTargetView;


	Shader ColorShader;
	Shader DefaultShader;

	ConstantBuffer<CB_VS_vertexshader> cb_vs_vertexshader;
	ConstantBuffer<CB_PS_pixelshader> cb_ps_pixelshader;


	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> depthStencilView;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> depthStencilBuffer;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> depthStencilState;

	Microsoft::WRL::ComPtr<ID3D11RasterizerState> rasterizerState;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> rasterizerState_CullFront;

	Microsoft::WRL::ComPtr<ID3D11BlendState> blendState;

	std::unique_ptr<DirectX::SpriteBatch> spriteBatch;
	std::unique_ptr<DirectX::SpriteFont> spriteFont;


public:
	Camera camera;
	int windowWidth = 0;
	int windowHeight = 0;
	Timer fpsTimer;
	bool AddedModel = false;
	InputHandler* inputHandler;
};

