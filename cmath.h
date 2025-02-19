#ifndef CMATH_H
#define CMATH_H

#include <math.h>

#define	MM_PI 3.14159265358979323846
#define DEGTORAD( a ) ( ( (a) * MM_PI ) / 180.0F )
#define RADTODEG( a ) ( ( (a) * 180.0f ) / MM_PI )

struct Vec2
{
	float x, y;
};

struct Vec3
{
	float x, y, z;
};

struct Vec4
{
	float x, y, z, w;
};

#define VEC2SET(v, x, y) \
	v.x = x;\
	v.y = y

inline float Vec2LengthSQR( const Vec2& v )
{
	return v.x * v.x + v.y * v.y;
}

inline float Vec2Length( const Vec2& v )
{
	return sqrt(v.x * v.x + v.y * v.y);
}

inline Vec2 Vec2Subtract( const Vec2& a, const Vec2& b )
{
	Vec2 v;
	v.x = a.x - b.x;
	v.y = a.y - b.y;
	return v;
}

inline float Vec2Distance( const Vec2& a, const Vec2& b )
{
	Vec2 v;
	v = Vec2Subtract (b, a);
	return Vec2Length( v );
}

inline Vec2 Vec2Normalize( const Vec2& v )
{
	float len;
	Vec2 n;
	len = Vec2Length( v );
	n.x = v.x / len;
	n.y = v.y / len;
	return n;
}

#endif
