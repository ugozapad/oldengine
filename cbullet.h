#ifndef CBULLET_H
#define CBULLET_H

#include "ctypes.h"
#include "cmath.h"
#include "crenderer.h"

#define MAX_BULLETS 256
#define MAX_BULLET_LIFETIME 2.0f //5.0f // secs

struct SBullet
{
	// parameters
	Vec3 dir;
	float speed;
	float damage;

	// actually bullet self parameters
	Vec3 pos;
	Vec3 rot;
	float time;
	bool active;
};

class CBulletManager
{
public:
	CBulletManager();
	~CBulletManager();

	void Create();
	void Destroy();

	void AddBullet( const Vec3& pos, const Vec3& dir, float speed, float damage );

	void Render();

	int GetBulletCounter() { return bulletcounter; }
	int GetActiveBullets() { return activebullet; }

private:
	SBullet bullets[MAX_BULLETS];
	int bulletcounter;
	int activebullet;
	CTexture* pTracerTexture;
	CVisual* pTracer;
};

extern CBulletManager bulletMan;

// weapon stuff

enum EWeapons
{
	WEAPON_PISTOL,
	WEAPON_AUTOMATIC,
	WEAPON_SHOTGUN,

	WEAPON_MAX
};

extern float g_WeaponThresholdTable[WEAPON_MAX];
extern float g_WeaponDamageTable[WEAPON_MAX];

#endif // !CBULLET_H
