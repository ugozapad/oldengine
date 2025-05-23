#include "cbullet.h"
#include "ctimer.h"

//#define BULLET_DEBUG
#define BULLET2D
#define BULLET_TRANSFORM
#define NEW_RENDERING

#ifdef BULLET_DEBUG
#undef MAX_BULLET_LIFETIME
#define MAX_BULLET_LIFETIME 10.0f
#endif // BULLET_DEBUG

CBulletManager bulletMan;

CBulletManager::CBulletManager()
{
	memset(bullets, 0, sizeof(bullets));
	bulletcounter = 0;
	activebullet = 0;
	pTracerTexture = NULL;
	pTracer = NULL;
}

CBulletManager::~CBulletManager()
{
}

void CBulletManager::Create()
{
	pTracerTexture = texcontainer->LoadTexture("data/textures/tracer.jpg");

	pTracer = renderer->CreateVisual(VISUAL_SPRITE);
	pTracer->Load("data/textures/tracer.jpg");
}

void CBulletManager::Destroy()
{
	if (pTracer)
	{
		delete pTracer;
		pTracer = NULL;
	}
}

void CBulletManager::AddBullet(const Vec3& pos, const Vec3& dir, float speed, float damage)
{
	//int idx;
	//if (bulletcounter <= MAX_BULLETS && bullets[bulletcounter].active)
	//	idx = bulletcounter++;
	//else 
	//	idx = bulletcounter;

	//int idx = bulletcounter;
	//if (bulletcounter <= MAX_BULLETS && bullets[bulletcounter].active)
	//{
	//	idx = bulletcounter = bulletcounter + 1;
	//}

	if (bulletcounter >= MAX_BULLETS - 1)
		bulletcounter = 0;

	int idx = bulletcounter;
	if (bullets[bulletcounter].active)
		idx = bulletcounter = bulletcounter + 1;


	// reset
	memset(&bullets[idx], 0, sizeof(bullets[idx]));

	bullets[idx].pos = pos;
	bullets[idx].dir = dir;
	bullets[idx].speed = speed;
	bullets[idx].damage = damage;
	bullets[idx].active = true;
}

void CBulletManager::Render()
{
	activebullet = 0;

	renderer->SetAlphaBlend(true);
	renderer->SetBlackAlpha(true);

#ifdef NEW_RENDERING
	SRenderData renderData;
	memset(&renderData, 0, sizeof(renderData));
	renderData.scale = 1.0f;
#endif

	bool noActive = false;

	for (int i = 0; i < MAX_BULLETS; i++)
	{
		if (!bullets[i].active)
			continue;

		noActive = true;
		activebullet++;

#ifdef BULLET_DEBUG
		bullets[i].speed = 1.0f;
#endif // BULLET_DEBUG

		// update bullet
		float ds = bullets[i].speed * timer.GetDt();
		Vec3 vel = Vec3Mulf(bullets[i].dir, ds);
		bullets[i].pos = Vec3Add(bullets[i].pos, vel);
		bullets[i].time += timer.GetDt();

#ifdef BULLET2D
		float angle = atan2f(bullets[i].dir.y, bullets[i].dir.x);
		Vec2 rot = Vec2AngleToDirection(angle);
		VEC3SET(bullets[i].rot, rot.x, rot.y, 0.0f);
#endif // BULLET2D

		// draw bullet

#ifdef NEW_RENDERING
		VEC3SET(renderData.position,
			bullets[i].pos.x,
			bullets[i].pos.y,
			0.0f);
		
		VEC3SET(renderData.rotation, 0.0f, 0.0f, RADTODEG(angle));

		pTracer->Render(&renderData);
#else

#ifndef BULLET_TRANSFORM
		float cx = bullets[i].pos.x;
		float cy = bullets[i].pos.y;

		renderer->DrawTexture(
			pTracerTexture,
			cx,
			cy,
			128.0f,
			128.0f);

		renderer->DrawRectColoredWire(
			cx,
			cy,
			128.0f,
			128.0f,
			0xff0000ff);
#else
		float cx = bullets[i].pos.x;
		float cy = bullets[i].pos.y;

		renderer->PushTransformMatrix();
		renderer->Translate(cx, cy, 0.0f);
		renderer->RotateZ(angle);

		renderer->DrawTexture(
			pTracerTexture,
			0.0f,
			0.0f,
			128.0f,
			128.0f);

		/*renderer->DrawRectColoredWire(
			0.0f,
			0.0f,
			128.0f,
			128.0f,
			0xff0000ff);*/

		renderer->PopTransformMatrix();
#endif // !BULLET_TRANSFORM

#endif // NEW_RENDERING

		if (bullets[i].time >= MAX_BULLET_LIFETIME)
			bullets[i].active = false;
	}

	renderer->SetBlackAlpha(false);
	renderer->SetAlphaBlend(false);


	//if (noActive)
	//{
	//	for (int i = 0; i < MAX_BULLETS; i++)
	//	{
	//		if (bullets[i].time >= MAX_BULLET_LIFETIME)
	//		{
	//			bullets[i].active = false;
	//			bulletcounter--;
	//		}
	//	}
	//}
}

// weapon stuff

float g_WeaponThresholdTable[WEAPON_MAX] =
{
	0.5f, // WEAPON_PISTOL
	0.05f,  // WEAPON_AUTOMATIC
	0.8f  // WEAPON_SHOTGUN
};

float g_WeaponDamageTable[WEAPON_MAX] =
{
	15.0f, // WEAPON_PISTOL
	8.0f,  // WEAPON_AUTOMATIC
	25.0f  // WEAPON_SHOTGUN
};