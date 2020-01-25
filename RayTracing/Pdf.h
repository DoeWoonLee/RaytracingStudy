#pragma once

#include "Vector3.h"

class IPdf 
{
public:
	virtual float Value(const vec3& vDirection) const = 0;
	virtual vec3 Generate() const = 0;
};