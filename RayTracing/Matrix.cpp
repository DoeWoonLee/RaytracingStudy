#include "stdafx.h"
#include "Matrix.h"

#include "Matrix.h"

tagMatrix::tagMatrix(XMMATRIX matSimd)
{
	XMStoreFloat4x4(this, matSimd);
}

tagMatrix tagMatrix::Inverse(void)
{
	XMMATRIX mat = XMLoadFloat4x4(this);
	XMMATRIX matInverse = XMMatrixInverse(&XMMatrixDeterminant(mat), mat);

	XMStoreFloat4x4(this, matInverse);
	return *this;
}

tagMatrix tagMatrix::Identity(void)
{
	XMStoreFloat4x4(this, XMMatrixIdentity());
	return *this;
}

XMMATRIX tagMatrix::ToSIMD(void) const
{
	return XMLoadFloat4x4(this);
}

void tagMatrix::LoadSIMD(XMMATRIX matSimd)
{
	XMStoreFloat4x4(this, matSimd);
}

tagMatrix & tagMatrix::operator=(const tagMatrix & Float4x4)
{
	XMVECTOR V1 = XMLoadFloat4((const XMFLOAT4*)&Float4x4._11);
	XMVECTOR V2 = XMLoadFloat4((const XMFLOAT4*)&Float4x4._21);
	XMVECTOR V3 = XMLoadFloat4((const XMFLOAT4*)&Float4x4._31);
	XMVECTOR V4 = XMLoadFloat4((const XMFLOAT4*)&Float4x4._41);

	XMStoreFloat4((XMFLOAT4*)&_11, V1);
	XMStoreFloat4((XMFLOAT4*)&_21, V2);
	XMStoreFloat4((XMFLOAT4*)&_31, V3);
	XMStoreFloat4((XMFLOAT4*)&_41, V4);

	return *this;
}

tagMatrix tagMatrix::operator+(const tagMatrix & Float4x4)
{
	XMMATRIX matInput = XMLoadFloat4x4(&Float4x4);
	XMMATRIX matThis = XMLoadFloat4x4(this);
	matThis += matInput;

	tagMatrix matTemp;
	XMStoreFloat4x4(&matTemp, matThis);

	return matTemp;
}
tagMatrix tagMatrix::operator-(const tagMatrix & Float4x4)
{
	XMMATRIX matInput = XMLoadFloat4x4(&Float4x4);
	XMMATRIX matThis = XMLoadFloat4x4(this);
	matThis -= matInput;

	tagMatrix matTemp;
	XMStoreFloat4x4(&matTemp, matThis);

	return matTemp;
}
tagMatrix tagMatrix::operator*(const tagMatrix & Float4x4)
{
	XMMATRIX matInput = XMLoadFloat4x4(&Float4x4);
	XMMATRIX matThis = XMLoadFloat4x4(this);
	matThis *= matInput;

	tagMatrix matTemp;
	XMStoreFloat4x4(&matTemp, matThis);

	return matTemp;
}

void tagMatrix::operator*=(const tagMatrix & Float4x4)
{
	XMMATRIX matInput = XMLoadFloat4x4(&Float4x4);
	XMMATRIX matThis = XMLoadFloat4x4(this);
	matThis *= matInput;

	XMStoreFloat4x4(this, matThis);
}

void tagMatrix::operator+=(const tagMatrix & Float4x4)
{
	XMMATRIX matInput = XMLoadFloat4x4(&Float4x4);
	XMMATRIX matThis = XMLoadFloat4x4(this);
	matThis += matInput;

	XMStoreFloat4x4(this, matThis);
}

void tagMatrix::operator-=(const tagMatrix & Float4x4)
{
	XMMATRIX matInput = XMLoadFloat4x4(&Float4x4);
	XMMATRIX matThis = XMLoadFloat4x4(this);
	matThis -= matInput;

	XMStoreFloat4x4(this, matThis);
}

