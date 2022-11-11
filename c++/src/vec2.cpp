#include "vec2.h"

Vec2::Vec2() 
{

}

Vec2::Vec2(float xin, float yin)
  : x(xin)
  , y(yin)
{

}

bool Vec2::operator == (const Vec2 & rhs) const
{
  return x == rhs.x && y == rhs.y;
}

bool Vec2::operator != (const Vec2 & rhs) const
{
  return x != rhs.x || y != rhs.y;
}

Vec2 Vec2::operator + (const Vec2 & rhs) const
{
  return Vec2(x+rhs.x, y+rhs.y);
}

Vec2 Vec2::operator - (const Vec2 & rhs) const
{
  return Vec2(x-rhs.x, y-rhs.y);
}

Vec2 Vec2::operator * (const float value) const
{
  return Vec2(x*value, y*value);
}

Vec2 Vec2::operator / (const float value) const
{
  return Vec2(x/value, y/value);
}

void Vec2::operator += (const Vec2 & rhs)
{
  x += rhs.x;
  y += rhs.y;
}

void Vec2::operator -= (const Vec2 & rhs)
{
  x -= rhs.x;
  y -= rhs.y; 
}

void Vec2::operator /= (const float val)
{
  x /= val;
  y /= val; 
}

void Vec2::operator *= (const float val)
{
  x *= val;
  y *= val; 
}

float Vec2::dist(const Vec2 & rhs) const
{
  return sqrtf((x-rhs.x)*(x-rhs.x) + (y-rhs.y)*(y-rhs.y));
}

float Vec2::length() const
{
  return sqrtf(x*x + y*y);
}

void Vec2::normalize()
{
  *this /= length();
}

void Vec2::origin()
{
  x = 0;
  y = 0;
}