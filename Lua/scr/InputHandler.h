#pragma once
#include <DirectXMath.h>
#include "Graphics/Mesh.h"
#include "Graphics/Camera.h"
#include "Mouse/MouseClass.h"
//#include "Mouse.h"
#include "Keyboard/KeyboardClass.h"
//#include "Keyboard.h"
using namespace DirectX;

struct Ray
{
	XMVECTOR Origin;
	XMVECTOR Dir;
	Ray(XMVECTOR Origin, XMVECTOR Dir) :Origin(Origin), Dir(Dir){}
	Ray() :Origin(), Dir() {}
};
class EventLua
{
public:
	EventLua() :mouse(""), key("") {}
	EventLua(std::string m, std::string k) :mouse(m), key(k) {}
	std::string mouse;
	std::string key;
};
class InputHandler
{
	Camera* camera;

	EventLua luaEvent;
	Ray ray;
	//screen size
	int width;
	int height;
private:
	bool PointInTriangle(XMVECTOR& triV1, XMVECTOR& triV2,
		XMVECTOR& triV3, XMVECTOR& point);
	bool PointInPlane(Ray& ray, MeshOb* m);
	float RayTriangle(XMVECTOR &Origin,XMVECTOR& Dir, MeshOb* m);
	/* Test */
	bool PointInAABB(const XMFLOAT3& point, const XMFLOAT3& mesh);
public:
	
	InputHandler();//xy 
	~InputHandler();
	unsigned char GetKeyCode()const; //char
	Mouse* mouse;
	Keyboard* keyboard;
	EventLua& GetEvent();
	void setValues(Mouse* mouse, Keyboard* keyboard);
	void SetCamera(Camera* camera, int width, int height);
	bool Picking(Ray& ray, MeshOb* m);
	void GetRay(const int& x, const int& y);
	void FollowMouse(MeshOb * m);

	bool CollideWith(MeshOb* m);
	XMFLOAT2 GetMousePos();
};

