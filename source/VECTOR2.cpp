#include <iostream>
#include "VECTOR2.h"

using namespace std;

VECTOR2::VECTOR2()
{

}

VECTOR2::VECTOR2(int x, int y)
{
	this->x = x;
	this->y = y;
}

VECTOR2::~VECTOR2()

{

}


VECTOR2 & VECTOR2::operator=(const VECTOR2 & vec)
{
	this->x = vec.x;
	this->y = vec.y;
	return (*this);
}


int & VECTOR2::operator[](int i)
{
	if (i == 0)
	{
		return x;
	}
	else if (i == 1)
	{
		return y;
	}
	else
	{
		return x;
	}
}


bool VECTOR2::operator==(const VECTOR2 & vec) const
{
	return ((this->x == vec.x) && (this->y == vec.y));
}

bool VECTOR2::operator!=(const VECTOR2 & vec) const
{
	//return (!(this->x == vec.x) || !(this->y == vec.y));
	return !((this->x == vec.x) && (this->y == vec.y));
}

bool VECTOR2::operator>=(const VECTOR2 & vec) const
{
	return ((this->x >= vec.x) && (this->y >= vec.y));
}

bool VECTOR2::operator<=(const VECTOR2 & vec) const
{
	return ((this->x <= vec.x) && (this->y <= vec.y));
}

bool VECTOR2::operator>(const VECTOR2 & vec) const
{
	return ((this->x > vec.x) && (this->y > vec.y));
}

bool VECTOR2::operator>(int k) const
{
	return ((this->x > k) && (this->y > k));
}

bool VECTOR2::operator<(const VECTOR2 & vec) const
{
	return ((this->x < vec.x) && (this->y < vec.y));
}


VECTOR2 & VECTOR2::operator+=(const VECTOR2 & vec)
{
	this->x += vec.x;
	this->y += vec.y;
	return (*this);
}

VECTOR2 & VECTOR2::operator-=(const VECTOR2 & vec)
{
	this->x -= vec.x;
	this->y -= vec.y;
	return (*this);
}

VECTOR2 & VECTOR2::operator*=(int k)
{
	this->x *= k;
	this->y *= k;
	return (*this);
}

VECTOR2 & VECTOR2::operator/=(int k)
{
	//0Š„‚è‚Ì¶Þ°ÄÞˆ—‚Í—L–³–³–â
	this->x /= k;
	this->y /= k;
	return (*this);
}


VECTOR2 VECTOR2::operator+() const
{
	return (*this);
}

VECTOR2 VECTOR2::operator-() const
{
	return VECTOR2(-this->x, -this->y);
}

VECTOR2 operator+(const VECTOR2 & u, int v)
{
	VECTOR2 vec;
	vec.x = u.x + v;
	vec.y = u.y + v;
	return vec;
}

VECTOR2 operator-(const VECTOR2 & u, int v)
{
	VECTOR2 vec;
	vec.x = u.x - v;
	vec.y = u.y - v;
	return vec;
}

VECTOR2 operator+(const VECTOR2 & u, const VECTOR2 & v)
{
	VECTOR2 vec;
	vec.x = u.x + v.x;
	vec.y = u.y + v.y;
	return vec;
}

VECTOR2 operator-(const VECTOR2 & u, const VECTOR2 & v)
{
	VECTOR2 vec;
	vec.x = u.x - v.x;
	vec.y = u.y - v.y;
	return vec;
}

VECTOR2 operator*(const VECTOR2 & u, int v)
{
	VECTOR2 vec;
	vec.x = u.x * v;
	vec.y = u.y * v;
	return vec;
}

VECTOR2 operator*(int u, const VECTOR2 & v)
{
	VECTOR2 vec;
	vec.x = u * v.x;
	vec.y = u * v.y;
	return vec;
}

VECTOR2 operator*(const VECTOR2 & u, const VECTOR2 & v)
{
	VECTOR2 vec;
	vec.x = u.x * v.x;
	vec.y = u.y * v.y;
	return vec;
}

VECTOR2 operator/(const VECTOR2 & u, int v)
{
	VECTOR2 vec;
	vec.x = u.x / v;
	vec.y = u.y / v;
	return vec;
}

VECTOR2 operator/(const VECTOR2 & u, const VECTOR2 & v)
{
	VECTOR2 vec;
	vec.x = u.x / v.x;
	vec.y = u.y / v.y;
	return vec;
}

VECTOR2 operator%(const VECTOR2 & u, int v)
{
	VECTOR2 vec;
	vec.x = u.x % v;
	vec.y = u.y % v;
	return vec;
}

VECTOR2 operator%(const VECTOR2 & u, const VECTOR2 & v)
{
	VECTOR2 vec;
	vec.x = u.x % v.x;
	vec.y = u.y % v.y;
	return vec;
}
