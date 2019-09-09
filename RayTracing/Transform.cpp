#include "stdafx.h"
#include "Transform.h"
#include "MemoryPool.h"

CTransform::CTransform(void)
{
}

CTransform::CTransform(const vec3 & vPos):
	m_vScale(1.f, 1.f, 1.f), m_vPos(vPos) {
	Update(0.f);
}

CTransform::CTransform(const vec3 & vPos, const vec3 & vRotate) :
	m_vScale(1.f, 1.f, 1.f) , m_vPos(vPos), m_vRotate(vRotate) {
	Update(0.f);
}

CTransform::CTransform(const vec3 & vPos, const vec3 & vRotate, const vec3 vScale) :
	m_vPos(vPos), m_vRotate(vRotate), m_vScale(vScale){
	Update(0.f);
}

thread_local XMMATRIX WorldMatrix;
thread_local XMMATRIX InverseMatrix;
void CTransform::InverseRay(const CRay & WorldRay, CRay & InverseRay)
{

	InverseMatrix = m_matInvWorld.ToSIMD();
	InverseRay.SetOrigin(XMVector3TransformCoord(WorldRay.GetOrigin().ToSIMD(), InverseMatrix));
	InverseRay.SetDirection(XMVector3TransformNormal(WorldRay.GetDirection().ToSIMD(), InverseMatrix));
}

void CTransform::WorldRay(CRay & WorldRay, const CRay & InverseRay)
{
	WorldMatrix = m_matWorld.ToSIMD();
	WorldRay.SetOrigin(XMVector3TransformCoord(InverseRay.GetOrigin().ToSIMD(), WorldMatrix));
	WorldRay.SetDirection(XMVector3Normalize(XMVector3TransformNormal(InverseRay.GetDirection().ToSIMD(), WorldMatrix)));
}

void CTransform::WorldNormal(vec3 & vNormal)
{
	WorldMatrix = m_matWorld.ToSIMD();

	vNormal.LoadSIMD(XMVector3Normalize(XMVector3TransformNormal(vNormal.ToSIMD(), WorldMatrix)));
}

thread_local XMMATRIX smatScale, smatTrans, smatRotate, smatWorld;
void CTransform::Update(const float & fTimeDelta)
{
	m_vPos.LoadSIMD(m_vPos.ToSIMD() + m_vDir.ToSIMD() * m_fSpeed * fTimeDelta);
	//m_vRotate.LoadSIMD(m_vPos.ToSIMD() + m_vRotateDir.ToSIMD() * m_fRotateSpeed * fTimeDelta);

	smatScale = XMMatrixScaling(m_vScale.x, m_vScale.y, m_vScale.z);
	smatTrans = XMMatrixTranslation(m_vPos.x, m_vPos.y, m_vPos.z);
	smatRotate = XMMatrixRotationRollPitchYaw(m_vRotate.x, m_vRotate.y, m_vRotate.z);
	

	smatWorld = smatScale * smatTrans * smatRotate;
	XMVECTOR svDeterminant = XMMatrixDeterminant(smatWorld);
	
	// World
	m_matWorld.LoadSIMD( smatWorld);
	// Inverse
	m_matInvWorld.LoadSIMD( XMMatrixInverse(&svDeterminant, smatWorld));
}

const vec3 & CTransform::GetPos(void)
{
	return m_vPos;
}
