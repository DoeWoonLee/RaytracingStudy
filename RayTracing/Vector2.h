#pragma once

class vec2
{
public:
	vec2() :x(0.f), y(0.f) {}
	vec2(float _x, float _y) : x(_x), y(_y) {}
	float x;
	float y;

	vec2 operator +(const vec2& v) const
	{
		return vec2(this->x + v.x, this->y + v.y);
	}
	vec2 operator -(const vec2& v) const
	{
		return vec2(this->x - v.x, this->y - v.y);
	}
};
inline vec2	operator * (vec2& v, const float& fValue)
{
	return vec2(v.x * fValue, v.y * fValue);
}