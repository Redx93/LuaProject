#pragma once
#include <DirectXMath.h>
#include "Graphics/Mesh.h"
#include "Graphics/Camera.h"
using namespace DirectX;


struct Ray
{
	XMVECTOR Origin;
	XMVECTOR Dir;
	Ray(XMVECTOR Origin, XMVECTOR Dir) :Origin(Origin), Dir(Dir){}
};

class InputHandler
{
	Camera* camera;
	int width;
	int height;
private:
	bool PointInTriangle(XMVECTOR& triV1, XMVECTOR& triV2,
		XMVECTOR& triV3, XMVECTOR& point);
	bool PointInPlane(Ray& ray, RenderbleGameObject* m);
	float RayTriangle(XMVECTOR &Origin,XMVECTOR& Dir, RenderbleGameObject* m);
public:
	
	InputHandler(Camera * camera, int width, int height);
	bool Picking(Ray& ray, RenderbleGameObject* m);
	Ray GetRay(const int& x, const int& y);
	void FollowMouse(Ray &ray, RenderbleGameObject * m);

};

