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

/* Vector 2D */

#define VEC2SET(V, X, Y) \
	V.x = X;\
	V.y = Y

#define VEC2ZERO(v) \
	v.x = 0.f; \
	v.y = 0.f

inline float Vec2LengthSQR( const Vec2& v )
{
	return v.x * v.x + v.y * v.y;
}

inline float Vec2Length( const Vec2& v )
{
	return (float)sqrt(v.x * v.x + v.y * v.y);
}


inline Vec2 Vec2Add( const Vec2& a, const Vec2& b )
{
	Vec2 v;
	v.x = a.x + b.x;
	v.y = a.y + b.y;
	return v;
}

inline Vec2 Vec2Subtract( const Vec2& a, const Vec2& b )
{
	Vec2 v;
	v.x = a.x - b.x;
	v.y = a.y - b.y;
	return v;
}

inline Vec2 Vec2Mul( const Vec2& a, const Vec2& b )
{
	Vec2 v;
	v.x = a.x * b.x;
	v.y = a.y * b.y;
	return v;
}

inline Vec2 Vec2Div( const Vec2& a, const Vec2& b )
{
	Vec2 v;
	v.x = a.x / b.x;
	v.y = a.y / b.y;
	return v;
}

inline Vec2 Vec2Addf( const Vec2& a, const float b )
{
	Vec2 v;
	v.x = a.x + b;
	v.y = a.y + b;
	return v;
}

inline Vec2 Vec2Subtractf( const Vec2& a, const float b )
{
	Vec2 v;
	v.x = a.x - b;
	v.y = a.y - b;
	return v;
}

inline Vec2 Vec2Mulf( const Vec2& a, const float b )
{
	Vec2 v;
	v.x = a.x * b;
	v.y = a.y * b;
	return v;
}

inline Vec2 Vec2Divf( const Vec2& a, const float b )
{
	Vec2 v;
	v.x = a.x / b;
	v.y = a.y / b;
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

inline Vec2 Vec2Rotate(const Vec2& p, float angle)
{
	Vec2 v;
	v.x = p.x * (float)cos(angle) - p.y * (float)sin(angle);
	v.y = p.x * (float)sin(angle) + p.y * (float)cos(angle);
	return v;
}

inline Vec2 Vec2AngleToDirection( float angle )
{
	Vec2 v;
	v.x = (float)cosf( angle );
	v.y = (float)sinf( angle );
	return v;
}

/* Vector 3D */

#define VEC3SET(V, X, Y, Z) \
	V.x = X;\
	V.y = Y;\
	V.z = Z

#define VEC3ZERO(v) \
	v.x = 0.f; \
	v.y = 0.f; \
	v.z = 0.f 

inline float Vec3LengthSQR(const Vec3& v)
{
	return v.x * v.x + v.y * v.y + v.z * v.z;
}

inline float Vec3Length(const Vec3& v)
{
	return (float)sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

inline Vec3 Vec3Add(const Vec3& a, const Vec3& b)
{
	Vec3 v;
	v.x = a.x + b.x;
	v.y = a.y + b.y;
	v.z = a.z + b.z;
	return v;
}

inline Vec3 Vec3Subtract(const Vec3& a, const Vec3& b)
{
	Vec3 v;
	v.x = a.x - b.x;
	v.y = a.y - b.y;
	v.z = a.z - b.z;
	return v;
}

inline Vec3 Vec3Mul(const Vec3& a, const Vec3& b)
{
	Vec3 v;
	v.x = a.x * b.x;
	v.y = a.y * b.y;
	v.z = a.z * b.z;
	return v;
}

inline Vec3 Vec3Div(const Vec3& a, const Vec3& b)
{
	Vec3 v;
	v.x = a.x / b.x;
	v.y = a.y / b.y;
	v.z = a.z / b.z;
	return v;
}

inline Vec3 Vec3Addf(const Vec3& a, const float b)
{
	Vec3 v;
	v.x = a.x + b;
	v.y = a.y + b;
	v.z = a.z + b;
	return v;
}

inline Vec3 Vec3Subtractf(const Vec3& a, const float b)
{
	Vec3 v;
	v.x = a.x - b;
	v.y = a.y - b;
	v.z = a.z - b;
	return v;
}

inline Vec3 Vec3Mulf(const Vec3& a, const float b)
{
	Vec3 v;
	v.x = a.x * b;
	v.y = a.y * b;
	v.z = a.z * b;
	return v;
}

inline Vec3 Vec3Divf(const Vec3& a, const float b)
{
	Vec3 v;
	v.x = a.x / b;
	v.y = a.y / b;
	v.z = a.z / b;
	return v;
}

inline float Vec3Distance(const Vec3& a, const Vec3& b)
{
	Vec3 v;
	v = Vec3Subtract(b, a);
	return Vec3Length(v);
}

inline Vec3 Vec3Normalize(const Vec3& v)
{
	float len;
	Vec3 n;
	len = Vec3Length(v);
	n.x = v.x / len;
	n.y = v.y / len;
	n.z = v.z / len;
	return n;
}

inline Vec3 Vec3Rotate(const Vec3& p, float angle)
{
	Vec3 v;
	v.x = p.x * (float)cos(angle) - p.y * (float)sin(angle);
	v.y = p.x * (float)sin(angle) + p.y * (float)cos(angle);
	return v;
}

#endif
