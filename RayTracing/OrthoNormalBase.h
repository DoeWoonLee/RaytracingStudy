#pragma once

#include "Vector3.h"

class OrthoNormalBase
{
public:
	OrthoNormalBase() {}
	inline const vec3& operator[](int i) const { return m_vAxis[i]; }
	vec3 u() const { return m_vAxis[0]; }
	vec3 v() const { return m_vAxis[1]; }
	vec3 w() const { return m_vAxis[2]; }

	vec3 Local(float a, float b, float c) const { return a * m_vAxis[0] + b * m_vAxis[1] + c * m_vAxis[2]; }
	vec3 Local(const vec3& a) const { return a.x *u() + a.y * v() + a.z *w(); }
	void BuildFromW(const vec3& vNormal);

private:
	vec3 m_vAxis[3];

};