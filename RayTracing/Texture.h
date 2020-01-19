#pragma once

#include "Vector3.h"

class ITexture
{
public:
	ITexture(void) : m_Ref(0) {}
	virtual ~ITexture(void) {}
public:
	virtual const vec3& Value(const float& u, const float& v, const vec3& p) const = 0;
	void AddRef(void) { m_Ref++; }
	bool Release(void){
		if(m_Ref == 0)
		{
			delete this;
			return true;
		} 
		else
		{
			m_Ref--;
		}
		return false;
	}
private:
	uint32_t m_Ref;
};