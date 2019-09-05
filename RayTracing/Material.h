#pragma once

#include "HitRecord.h"
#include "Ray.h"

class CMaterial
{
public :
	virtual bool Scatter(HitRecord& HitRecrd, CRay& OutRay) = 0;
};