#include "GameObject.h"

const XMVECTOR& GameObject::GetPositionVector() const
{
	return this->posVector;
}

const XMFLOAT3& GameObject::GetPositionFloat3() const
{
	return this->pos;
}

const XMVECTOR& GameObject::GetRotationVector() const
{
	return this->rotVector;
}

const XMFLOAT3& GameObject::GetRotationFloat3() const
{
	return this->rot;
}

const XMFLOAT3& GameObject::GetScaleFloat3() const
{
	return this->scale;
}

XMFLOAT3& GameObject::GetPositionFloat3()
{
	return this->pos;
}

XMFLOAT3& GameObject::GetScaleFloat3()
{
	return this->scale;
}

void GameObject::SetPosition(float x, float y, float z)
{
	this->pos = XMFLOAT3(x, y, z);
	this->posVector = XMLoadFloat3(&this->pos);
	this->UpdateMatrix();
}

void GameObject::SetPosition(XMFLOAT3& pos)
{
	this->pos = pos;
}


void GameObject::AdjustPosition(float x, float y, float z)
{
	this->pos.x += x;
	this->pos.y += y;
	this->pos.z += z;
	this->posVector = XMLoadFloat3(&this->pos);
	this->UpdateMatrix();
}

void GameObject::SetRotation(float x, float y, float z)
{
	this->rot = XMFLOAT3(x, y, z);
	this->rotVector = XMLoadFloat3(&this->rot);
	this->UpdateMatrix();
}

void GameObject::AdjustRotation(float x, float y, float z)
{
	this->rot.x += x;
	this->rot.y += y;
	this->rot.z += z;
	this->rotVector = XMLoadFloat3(&this->rot);
	this->UpdateMatrix();
}

void GameObject::UpdateMatrix()
{
	assert("UpdateMatrix overridden." && 0);
}

void GameObject::SetScale(float xScale, float yScale, float zScale)
{
	scale.x = xScale;
	scale.y = yScale;
	scale.z = zScale;
	UpdateMatrix();
}
