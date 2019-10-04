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

	void SetPositionTo(XMFLOAT3& pos);
	void SetPosition(const XMVECTOR& pos);
	void SetPosition(const XMFLOAT3& pos);
	void SetPosition(float x, float y, float z);
	void AdjustPosition(const XMVECTOR& pos);
	void AdjustPosition(const XMFLOAT3& pos);
	void AdjustPosition(float x, float y, float z);
	void SetRotation(const XMVECTOR& rot);
	void SetRotation(const XMFLOAT3& rot);
	void SetRotation(float x, float y, float z);
	void AdjustRotation(const XMVECTOR& rot);
	void AdjustRotation(const XMFLOAT3& rot);
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