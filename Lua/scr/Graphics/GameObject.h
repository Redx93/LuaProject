#pragma once
#include <DirectXMath.h>
using namespace DirectX;
class GameObject
{
public:
	const XMVECTOR& GetPositionVector() const;
	const XMFLOAT3& GetPositionFloat3() const;
	const XMVECTOR& GetRotationVector() const;
	const XMFLOAT3& GetRotationFloat3() const;
	const XMFLOAT3& GetScaleFloat3()const;

	XMFLOAT3& GetPositionFloat3();
	XMFLOAT3& GetScaleFloat3();
	void SetPosition(float x, float y, float z);
	void SetPosition(XMFLOAT3& pos);
	void AdjustPosition(float x, float y, float z);
	void SetRotation(float x, float y, float z);
	void AdjustRotation(float x, float y, float z);
	void SetScale(float xScale, float yScale, float zScale = 1.0f);
protected:
	virtual void UpdateMatrix();

	XMVECTOR posVector;
	XMVECTOR rotVector;
	XMFLOAT3 pos;
	XMFLOAT3 rot;
	XMFLOAT3 scale;


	XMVECTOR vec_forward;
	XMVECTOR vec_left;
	XMVECTOR vec_right;
	XMVECTOR vec_backward;
};