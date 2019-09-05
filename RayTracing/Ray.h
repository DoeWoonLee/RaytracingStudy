#pragma once

#include "Vector3.h"


class CRay
{
public:
	explicit CRay();
	explicit CRay(vec3 vOrigin, vec3 vDirection);
public:
	const vec3& GetOrigin() const;
	void GetOrigin(XMVECTOR& sOut) const;
	XMVECTOR GetSOrigin(void)const ;

	const vec3& GetDirection() const ;
	void GetDirection(XMVECTOR& sOut) const;
	XMVECTOR GetSDirection(void) const;

	void SetOrigin(XMVECTOR& sOrigin);
	void SetOrigin(vec3& vOrigin);
	void SetDirection(XMVECTOR& sDirection);
	void SetDirection(vec3& vDirection);

private:
	vec3 m_vOrigin;
	vec3 m_vDirection;
};