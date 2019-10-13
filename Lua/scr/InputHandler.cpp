#include "InputHandler.h"

InputHandler::InputHandler()
{
	//this->camera = camera;
	//this->mouse = mouse;
	//this->keyboard = keyboard;
	//this->width = width;
	//this->height = height;
}

InputHandler::~InputHandler()
{
	//delete mouse;
	//delete keyboard;
}

unsigned char InputHandler::GetKeyCode()const 
{
	return this->keyboard->ReadChar();
}

void InputHandler::setValues(MouseClass* mouse, KeyboardClass* keyboard)
{
	this->keyboard = keyboard;
	this->mouse = mouse;
}

bool InputHandler::Picking(Ray &ray, MeshOb* m)
{
	int distanse = RayTriangle(ray.Origin, ray.Dir, m);
	if (distanse > 0)
	{
		return true;
	}	
	return false;
}

float InputHandler::RayTriangle(XMVECTOR &Origin, XMVECTOR& Dir, MeshOb* m)
{
	//pick vertices
	auto vertices = m->vertices;
	UINT triCount = m->IndexCount;
	XMMATRIX& worldSpace = m->worldMatrix;
	float tMin = 0.0f;
	for (UINT i = 0; i < triCount / 3; i++)
	{
		UINT i0 = i * 3 + 0;
		UINT i1 = i * 3 + 1;
		UINT i2 = i * 3 + 2;

		// Vertices for this triangle.
		XMVECTOR v0 = XMLoadFloat3(&vertices[i0].Position);
		XMVECTOR v1 = XMLoadFloat3(&vertices[i1].Position);
		XMVECTOR v2 = XMLoadFloat3(&vertices[i2].Position);

		//Transform the vertices to world space
		v0 = XMVector3TransformCoord(v0, worldSpace);
		v1 = XMVector3TransformCoord(v1, worldSpace);
		v2 = XMVector3TransformCoord(v2, worldSpace);

		XMVECTOR U = v1 - v0;
		XMVECTOR V = v2 - v0;

		//Compute face normal by crossing U, V
		XMVECTOR Normal = XMVector3Normalize(XMVector3Cross(U, V));
		//Get plane equation ("Ax + By + Cz + D = 0") Variables
		float PlaneA = XMVectorGetX(Normal);
		float PlaneB = XMVectorGetY(Normal);
		float PlaneC = XMVectorGetZ(Normal);
		//D = (-n.x * p.x) - (n.y * p.y) - (n.z * p.z)
		float PlaneD = (-PlaneA * XMVectorGetX(v0) - PlaneB * XMVectorGetY(v0) - PlaneC * XMVectorGetZ(v0));

		//Now we find where (on the ray) the ray intersects with the triangles plane
		//Ax + By + Cz + D
		//float ep1 = (orgin * normal)
		float ep1 = (XMVectorGetX(Origin) * PlaneA) + (XMVectorGetY(Origin) * PlaneB) + (XMVectorGetZ(Origin) * PlaneC);
		//float ep2 = (orgin * normal)
		float ep2 = (XMVectorGetX(Dir) * PlaneA) + (XMVectorGetY(Dir) * PlaneB) + (XMVectorGetZ(Dir) * PlaneC);

		float t = 0.f;
		//Make sure there are no divide-by-zeros
		if (ep2 != 0.0f)
			//t = -(orgin*n) + d/(dir*n)
			t = -(ep1 + PlaneD) / (ep2);

		//Make sure you don't pick objects behind the camera
		if (t > 0.0f)
		{
			//Get the point on the plane
			float	PointX = XMVectorGetX(Origin) + XMVectorGetX(Dir) * t;
			float	PointY = XMVectorGetY(Origin) + XMVectorGetY(Dir) * t;
			float	PointZ = XMVectorGetZ(Origin) + XMVectorGetZ(Dir) * t;
			// point = orgin + Dir * t;
			// w = 0 because its a point and not a vector
			XMVECTOR pointInPlane = XMVectorSet(PointX, PointY, PointZ, 0.0f);

			//Call function to check if point is in the triangle
			if (PointInTriangle(v0, v1, v2, pointInPlane))
			{
				//Return the distance to the hit, so you can check all the other pickable objects in your scene
				//and choose whichever object is closest to the camera
				return t / 2.0f;
			}
		}
	}
	return -1;
}

bool InputHandler::PointInAABB(const XMFLOAT3& point, const XMFLOAT3& mesh)
{
	/*construct the aabb*/
	XMFLOAT3 max = XMFLOAT3(mesh.x + 1, mesh.y + 1, mesh.z + 1);
	XMFLOAT3 min = XMFLOAT3(mesh.x - 1, mesh.y - 1, mesh.z - 1);
	bool inside = false;
	/*check if its inside*/
	if (point.x > min.x && point.x < max.x &&
		point.y > min.y && point.y < max.y &&
		point.z > min.z && point.z < max.z)
	{
		inside = true;
	}
	return inside;
}

bool InputHandler::PointInTriangle(XMVECTOR& v0, XMVECTOR& v1, XMVECTOR& v2, XMVECTOR& point)
{
	bool InsideOfTriangle = false;
	//To find out if the point is inside the triangle, we will check to see if the point
	//is on the correct side of each of the triangles edges.
	XMVECTOR cross1 = XMVector3Cross((v2 - v1), (point - v1));
	XMVECTOR cross2 = XMVector3Cross((v2 - v1), (v0 - v1));
	// Check if cross1 & cross2 are not facing opposite direction
	if (XMVectorGetX(XMVector3Dot(cross1, cross2)) >= 0)
	{
		cross1 = XMVector3Cross((v2 - v0), (point - v0));
		cross2 = XMVector3Cross((v2 - v0), (v1 - v0));
		// Check cross1 & cross2 
		if (XMVectorGetX(XMVector3Dot(cross1, cross2)) >= 0)
		{
			cross1 = XMVector3Cross((v1 - v0), (point - v0));
			cross2 = XMVector3Cross((v1 - v0), (v2 - v0));
			// Check cross1 & cross2 
			if (XMVectorGetX(XMVector3Dot(cross1, cross2)) >= 0)
			{
				InsideOfTriangle = true;
			}
		}
	}
	return InsideOfTriangle;
}

bool InputHandler::PointInPlane(Ray& ray, MeshOb* m)
{
	XMVECTOR Origin = ray.Origin;
	XMVECTOR Dir = ray.Dir;

	XMVECTOR v0 = XMVectorSet(0, 0, 0, 0);
	XMVECTOR Normal = XMVectorSet(0, 0, 1, 0);
	//Get plane equation ("Ax + By + Cz + D = 0") Variables
	float PlaneA = XMVectorGetX(Normal);
	float PlaneB = XMVectorGetY(Normal);
	float PlaneC = XMVectorGetZ(Normal);
	//D = (-n.x * p.x) - (n.y * p.y) - (n.z * p.z)
	float PlaneD = (-PlaneA * XMVectorGetX(v0) - PlaneB * XMVectorGetY(v0) - PlaneC * XMVectorGetZ(v0));

	//Now we find where (on the ray) the ray intersects with the triangles plane
		//Ax + By + Cz + D
		//float ep1 = (orgin * normal)
	float ep1 = (XMVectorGetX(Origin) * PlaneA) + (XMVectorGetY(Origin) * PlaneB) + (XMVectorGetZ(Origin) * PlaneC);
	//float ep2 = (orgin * normal)
	float ep2 = (XMVectorGetX(Dir) * PlaneA) + (XMVectorGetY(Dir) * PlaneB) + (XMVectorGetZ(Dir) * PlaneC);

	float t = 0.f;
	//Make sure there are no divide-by-zeros
	if (ep2 != 0.0f)
		//t = -(orgin*n) + d/(dir*n)
		t = -(ep1 + PlaneD) / (ep2);

	//Make sure you don't pick objects behind the camera
	if (t > 0.0f)
	{
		float half = 0.5;
		//Get the point on the plane
		float	PointX = XMVectorGetX(Origin) + XMVectorGetX(Dir) * t;
		float	PointY = XMVectorGetY(Origin) + XMVectorGetY(Dir) * t;
		//floor to one digit then + 0.5 
		PointX = floor(PointX) + half;
		PointY = floor(PointY) + half;
		float	PointZ = XMVectorGetZ(Origin) + XMVectorGetZ(Dir) * t;
		// point = orgin + Dir * t;
		// w = 0 because its a point and not a vector
		XMVECTOR pointInPlane = XMVectorSet(PointX, PointY, PointZ, 0.0f);
		XMFLOAT3 newPostion;
		XMStoreFloat3(&newPostion, pointInPlane);
		m->SetPosition(newPostion);
		return true;
	}
	return false;
}

Ray InputHandler::GetRay(const int& x, const int& y)
{
	XMFLOAT4X4 P;
	XMStoreFloat4x4(&P, camera->GetProjectionMatrix());

	XMVECTOR pickRayInViewSpaceDir = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	XMVECTOR pickRayInViewSpacePos = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);

	float PRVecX, PRVecY, PRVecZ;
	//Transform 2D pick position on screen space to 3D ray in View space
	PRVecX = (+2.0f * x / width - 1.0f) / P(0, 0);
	PRVecY = (-2.0f * y / height + 1.0f) / P(1, 1);
	//MousePosition.x = PRVecX;
	//MousePosition.y = PRVecY;

	PRVecZ = 1.0f;  //View space's Z direction ranges from 0 to 1, so we set 1 since the ray goes "into" the screen
	pickRayInViewSpaceDir = XMVectorSet(PRVecX, PRVecY, PRVecZ, 0.0f);

	//Uncomment this line if you want to use the center of the screen (client area)
	//to be the point that creates the picking ray (eg. first person shooter)
	//pickRayInViewSpaceDir = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);

	// Transform 3D Ray from View space to 3D ray in World space
	XMMATRIX pickRayToWorldSpaceMatrix;
	XMVECTOR matInvDeter;   //We don't use this, but the xna matrix inverse function requires the first parameter to not be null

	pickRayToWorldSpaceMatrix = XMMatrixInverse(&matInvDeter, camera->GetViewMatrix());   //Inverse of View Space matrix is World space matrix

	XMVECTOR Origin = XMVector3TransformCoord(pickRayInViewSpacePos, pickRayToWorldSpaceMatrix);
	XMVECTOR Dir = XMVector3TransformNormal(pickRayInViewSpaceDir, pickRayToWorldSpaceMatrix);
	Dir = XMVector3Normalize(Dir);

	return Ray(Origin,Dir);

}

void InputHandler::FollowMouse(Ray& ray,MeshOb* m)
{
	PointInPlane(ray, m);
}
