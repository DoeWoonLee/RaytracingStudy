#pragma once

#include "Vector3.h"

struct TexInfo
{
	TexInfo() : bBumpMap(false){}
	TexInfo(vec3 _vAlbedo) : vAlbedo(_vAlbedo), bBumpMap(false) {}
	TexInfo(vec3 _vAlbedo, vec3 _vNormal) 
		: vAlbedo(_vAlbedo), vNormal(_vNormal), bBumpMap(true) {}
	vec3 vAlbedo;
	vec3 vNormal;
	bool bBumpMap;
};
class ITexture
{
public:
	ITexture(void) : m_Ref(0) {}
	virtual ~ITexture(void) {}
public:
	virtual TexInfo Value(const float& u, const float& v, const vec3& p) const = 0;
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