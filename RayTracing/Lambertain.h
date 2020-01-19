#pragma once

#include "Material.h"
#include "MemoryPool.h"

class Texture;
class CLambertain : public CMaterial
{
public:
	explicit CLambertain(void);
	explicit CLambertain(Texture* pTexture);
	virtual ~CLambertain(void);
	virtual bool Scatter(HitRecord& Record, const CRay& InRay, CRay& OutRay, vec3& vColor);
	DECLARE_CREATE_BY_MEMORYPOOL(CLambertain, CMemoryPool::OBJECT)
private:
	Texture* m_pAlbedo;
};