#include <stdio.h>
#include <string.h>
#include "cplayer.h"
#include "cinput.h"
#include "ctimer.h"
#include "cwnd.h"
#include "cfontmanager.h"
#include "crenderer.h"

static char debugBuf[128];

CPlayer::CPlayer()
{
	scale = 0.2f;
}

CPlayer::~CPlayer()
{
}

void CPlayer::Update()
{
	CObject::Update();

	/*
	if (input->IsPressed('W'))
		position.y -= DEGTORAD(rotation.z) * timer.GetDt() * 200.0f;
	if (input->IsPressed('S'))
		position.y += DEGTORAD(rotation.z) * timer.GetDt() * 200.0f;
*/

	if (input->IsPressed('A'))
		rotation.z -= timer.GetDt() * 200.0f;
	if (input->IsPressed('D'))
		rotation.z += timer.GetDt() * 200.0f;

	Vec2 pos; pos.x = position.x; pos.y = position.y;

	float speed = timer.GetDt() * 200.0f;
	Vec2 dir = Vec2AngleToDirection( DEGTORAD( rotation.z )  - ( MM_PI * 0.5f ) );
	//dir = Vec2Mulf(dir, speed);

	//renderer->DrawLine(dir.x, dir.y, dir.x * 10.0f, dir.y * 10.0f, 0xff0000ff);
	
	if (input->IsPressed('W'))
	{
		pos = Vec2Add(pos, dir);
	}
	if (input->IsPressed('S'))
	{
		pos = Vec2Subtract(pos, dir);
	}

	position.x = pos.x;
	position.y = pos.y;

	sprintf(debugBuf, "pos %f %f\nrot %f\ndir %f %f", position.x, position.y, rotation.z, dir.x, dir.y);
	debugText.SetPosition( position.x, position.y);
	debugText.SetText(debugBuf);

	debugUtils->DrawLine(pos.x + dir.x, pos.y + dir.y, pos.x + dir.x * 200.0f, pos.y + dir.y * 200.0f, 0xff0000ff);

#if 0
	if (input->IsPressed('W'))
		position.y -= timer.GetDt() * 200.0f;
	if (input->IsPressed('S'))
		position.y += timer.GetDt() * 200.0f;
	if (input->IsPressed('A'))
		position.x -= timer.GetDt() * 200.0f;
	if (input->IsPressed('D'))
		position.x += timer.GetDt() * 200.0f;
	
	int x, y;
	input->GetMousePos(&x, &y);
	
	Vec2 pos2d;
	Vec2 vdist;
	Vec2 mouse;
	float dist;
	float rot;
	
	pos2d.x=position.x;
	pos2d.y=position.y;
	VEC2SET(mouse, x, y);

	vdist = Vec2Subtract(mouse, pos2d);
	//vdist = Vec2Normalize( vdist );
	dist = Vec2Distance(pos2d, mouse);
	rot = atan2( vdist.y,vdist.x  );
	rotation.z= DEGTORAD(rot);

	sprintf(debugBuf, "mouse %i %i\npos %.2f %.2f\nvdist %.2f %.2f\ndist %.2f\nr %f",
		x, y, pos2d.x, pos2d.y, vdist.x, vdist.y, dist, rot);
/*	

	sprintf(debugBuf, "pos %f %f", position.x, position.y);
	*/
	//debugText.SetPosition(100.f, 100.f);
	debugText.SetPosition( position.x, position.y);
	debugText.SetText(debugBuf);
#endif
}

