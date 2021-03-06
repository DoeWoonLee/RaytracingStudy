#pragma once

#include "Component.h"
#include "Matrix.h"
#include "Vector3.h"
#include "Ray.h"
#include "HitRecord.h"
#include "MemoryPool.h"


class CTransform : public CComponent
{
public:
	explicit CTransform(void);
	explicit CTransform(const vec3& vPos);
	explicit CTransform(const vec3& vPos, const vec3& vRotate);
	explicit CTransform(const vec3& vPos, const vec3& vRotate, const vec3 vScale);
public:
	DECLARE_CREATE_BY_MEMORYPOOL(CTransform, CMemoryPool::OBJECT)


	void InverseRay(const CRay& WorldRay, CRay& InverseRay) const;
	void WorldRay(CRay& WorldRay, const CRay& InverseRay) const;
	void WorldNormal(vec3& vNormal) const;
	void WorldPos(vec3& vPos) const;
	void WorldRecord(HitRecord& Record) const;
	void Update(const float& fTimeDelta);

	const vec3& GetPos(void);
private: // Static Values
	vec3 m_vScale;
	vec3 m_vPos;
	vec3 m_vRotate;
	_matrix m_matInvWorld;
	_matrix m_matWorld;
	// Update Values
	vec3 m_vDir;
	float m_fSpeed;
	vec3 m_vRotateDir;
	float m_fRotateSpeed;
	CTransform* m_pParent = nullptr;
};
