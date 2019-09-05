#pragma once

#include "Ray.h"

class CResources
{
	virtual bool Hit(const CRay& inRay, float& fMin, float& fMax) = 0;
};