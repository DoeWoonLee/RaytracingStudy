#include "stdafx.h"
#include "Transform.h"

CTransform::CTransform(void)
{
}

CTransform::CTransform(const vec3 & vPos):
	m_vScale(1.f, 1.f, 1.f) ,  m_vPos(vPos){ }

CTransform::CTransform(const vec3 & vPos, const vec3 & vRotate) :
	m_vScale(1.f, 1.f, 1.f) , m_vPos(vPos), m_vRotate(vRotate) {}

CTransform::CTransform(const vec3 & vPos, const vec3 & vRotate, const vec3 vScale) :
	m_vPos(vPos), m_vRotate(vRotate), m_vScale(vScale){ }

void CTransform::InverseRay(const CRay & WorldRay, CRay & InverseRay)
{
	thread_local XMMATRIX InverseMatrix = m_matInvWorld.ToSIMD();
	InverseRay.SetOrigin(XMVector3Transform(WorldRay.GetOrigin().ToSIMD(), InverseMatrix));
	InverseRay.SetDirection(XMVector3Transform(WorldRay.GetDirection().ToSIMD(), InverseMatrix));
}

void CTransform::WorldRay(CRay & WorldRay, const CRay & InverseRay)
{

	thread_local XMMATRIX WorldMatrix = m_matWorld.ToSIMD();
	WorldRay.SetOrigin(XMVector3Transform(InverseRay.GetOrigin().ToSIMD(), WorldMatrix));
	WorldRay.SetDirection(XMVector3Transform(InverseRay.GetDirection().ToSIMD(), WorldMatrix));
}

void CTransform::Update(const float & fTimeDelta)
{
	m_vPos.LoadSIMD(m_vPos.ToSIMD() + m_vDir.ToSIMD() * m_fSpeed * fTimeDelta);
	m_vRotate.LoadSIMD(m_vPos.ToSIMD() + m_vRotateDir.ToSIMD() * m_fRotateSpeed * fTimeDelta);

	thread_local static XMMATRIX smatScale, smatTrans, smatRotate, smatWorld;

	smatScale = XMMatrixScaling(m_vScale.x, m_vScale.y, m_vScale.z);
	smatTrans = XMMatrixTranslation(m_vPos.x, m_vPos.y, m_vPos.z);
	smatRotate = XMMatrixRotationRollPitchYaw(m_vRotate.x, m_vRotate.y, m_vRotate.z);
	
	smatWorld = smatScale * smatTrans * smatRotate;
	XMVECTOR svDeterminant = XMMatrixDeterminant(smatWorld);

	m_matWorld.LoadSIMD(smatWorld);
	m_matInvWorld.LoadSIMD( XMMatrixInverse(&svDeterminant, smatWorld));
}
