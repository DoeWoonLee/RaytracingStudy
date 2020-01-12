#include "stdafx.h"
#include "Vector3.h"

tagVector3::tagVector3(XMVECTOR vSimd)
{
	XMStoreFloat3(this, vSimd);
}

void tagVector3::Normalize(void)
{
	XMVECTOR Vector = XMLoadFloat3(this);
	Vector = XMVector3Normalize(Vector);
	XMStoreFloat3(this, Vector);
}

float tagVector3::Length(const tagVector3 & vVector)
{
	XMVECTOR vLength = XMVector3Length(ToSIMD() - vVector.ToSIMD());

	return vLength.m128_f32[0];
}

float tagVector3::Length(void)
{
	
	XMVECTOR vLength = XMVector3Length(ToSIMD());

	return vLength.m128_f32[0];
}

float tagVector3::LengthSquared(void)
{
	XMVECTOR vLengthSq = XMVector3LengthSq(ToSIMD());

	return vLengthSq.m128_f32[0];
}

const tagVector3& tagVector3::TransformNormal(XMMATRIX mat)
{
	XMVECTOR vThis = ToSIMD();
	XMStoreFloat3(this, XMVector3TransformNormal(vThis, mat));
	return *this;
}

const tagVector3& tagVector3::TransformCoord(XMMATRIX mat)
{
	XMVECTOR vThis = ToSIMD();
	XMStoreFloat3(this, XMVector3TransformCoord(vThis, mat));
	return *this;
}

float tagVector3::Dot(const tagVector3 & v1, const  tagVector3 & v2)
{
	//return v1.x * v2.x + v1.y * v2.y  + v1.z * v2.z;
	return XMVector3Dot(v1.ToSIMD(), v2.ToSIMD()).m128_f32[0];
}

tagVector3 tagVector3::Cross(const tagVector3 & v1, const tagVector3 & v2)
{

	return tagVector3(XMVector3Cross(v1.ToSIMD(), v2.ToSIMD()));
}

XMVECTOR tagVector3::ToSIMD(void) const
{
	return XMLoadFloat3(this);
}

void tagVector3::LoadSIMD(XMVECTOR vSimd)
{
	XMStoreFloat3(this, vSimd);
}

void tagVector3::operator=(const XMVECTOR & sVector)
{
	this->LoadSIMD(sVector);
}

tagVector3 tagVector3::operator+(const tagVector3 & AddedVec3)const
{
	XMVECTOR VecInput = XMLoadFloat3(&AddedVec3);
	XMVECTOR VecThis = XMLoadFloat3(this);

	VecInput = VecInput + VecThis;

	return tagVector3(VecInput);
}

tagVector3 tagVector3::operator-(const tagVector3 & AddedVec3)const
{
	XMVECTOR VecInput = XMLoadFloat3(&AddedVec3);
	XMVECTOR VecThis = XMLoadFloat3(this);

	VecInput = VecThis - VecInput;
	return tagVector3(VecInput);
}

//============================= Vector 3 ================================
void tagVector3::operator +=(const tagVector3& AddedVec3)
{
	XMVECTOR vecInput = XMLoadFloat3(&AddedVec3);
	this->operator +=(vecInput);
}
void tagVector3::operator -=(const tagVector3& AddedVec3)
{
	XMVECTOR vecInput = XMLoadFloat3(&AddedVec3);
	this->operator -=(vecInput);
}
void tagVector3::operator*=(const float & Float)
{
	XMVECTOR v = ToSIMD();
	this->LoadSIMD(v * Float);
}
void tagVector3::operator/=(const float & Float)
{
	XMVECTOR v = ToSIMD();
	this->LoadSIMD(v / Float);
}
void tagVector3::operator *=(const tagVector3& AddedVec3)
{
	XMVECTOR vecInput = XMLoadFloat3(&AddedVec3);
	this->operator *=(vecInput);
}
void tagVector3::operator/=(const tagVector3 & AddedVec3)
{
	XMVECTOR vecInput = XMLoadFloat3(&AddedVec3);
	this->operator /=(vecInput);
}
//=============================== Vector3 SIMD ==========================
void tagVector3::operator +=(const XMVECTOR& Vector)
{
	XMVECTOR vecThis = XMLoadFloat3(this);
	vecThis += Vector;

	XMStoreFloat3(this, vecThis);
}
void tagVector3::operator *=(const XMVECTOR& Vector)
{
	XMVECTOR vecThis = XMLoadFloat3(this);
	vecThis *= Vector;

	XMStoreFloat3(this, vecThis);
}
void tagVector3::operator/=(const XMVECTOR & Vector)
{
	XMVECTOR vecThis = XMLoadFloat3(this);
	vecThis /= Vector;

	XMStoreFloat3(this, vecThis);
}
void tagVector3::operator -=(const XMVECTOR& Vector)
{
	XMVECTOR vecThis = XMLoadFloat3(this);
	vecThis -= Vector;

	XMStoreFloat3(this, vecThis);
}

bool tagVector3::operator==(const tagVector3 & Vector)
{
	if ((this->x == Vector.x) && (this->y == Vector.y) && (this->z == Vector.z))
		return true;

	return false;
}

tagVector3 tagVector3::operator*(const float & fFloat)
{
	XMVECTOR FVector = XMLoadFloat3(this);
	FVector = XMVectorScale(FVector, fFloat);

	return tagVector3(FVector);
}

tagVector3 tagVector3::operator/(const float & fFloat)
{
	XMVECTOR FVector = XMLoadFloat3(this) / fFloat;

	return tagVector3(FVector);
}

tagVector3 tagVector3::operator+(const float & fFloat)const
{
	return tagVector3(ToSIMD() + XMVectorSet(fFloat, fFloat, fFloat, fFloat));
}

tagVector3 tagVector3::operator-(const float & fFloat)const
{
	return tagVector3(ToSIMD() - XMVectorSet(fFloat, fFloat, fFloat, fFloat));
}

float& tagVector3::operator[](const int & iIndex)
{
	return ((float*)(&x))[iIndex];
}
