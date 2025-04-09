#include <stdlib.h>
#include "cvisual_sprite.h"

bool CVisual_Sprite::debugDrawRect = false;

CVisual_Sprite::CVisual_Sprite()
{
	texture = NULL;
}

CVisual_Sprite::~CVisual_Sprite()
{
	texture = NULL;
}

void CVisual_Sprite::Render(const SRenderData* rnd_data)
{
	assert(rnd_data);

	float angle = rnd_data->rotation.z;

	// move picture to the center
	float w = (float)texture->GetWidth();
	float h = (float)texture->GetHeight();
	float x = rnd_data->position.x;
	float y = rnd_data->position.y;

	renderer->SetAlphaBlend(true);

	renderer->PushTransformMatrix();

	renderer->Translate(x, y, 0.0f);
	renderer->RotateZ( angle );
	renderer->Scale(rnd_data->scale);
	
	//w = w * 0.5f;
	//h = h * 0.5f;
	
//	x = -w;//(w* 0.5f);
//	y = -h;//(h* 0.5f);

	x = 0.f;
	y = 0.f;

	x = x - (w * 0.5f);
	y = y - (h * 0.5f);

	renderer->DrawTexture(
		texture, 
		x,
		y,
		w,
		h);
	
	if (debugDrawRect)
	{
		renderer->DrawRectColoredWire(
			x,
			y,
			w,
			h,
			0xff00ffff);
	}

	renderer->PopTransformMatrix();
}

void CVisual_Sprite::Load(const char* filename)
{
	texture = texcontainer->LoadTexture(filename);
}


/*
	renderer->SetAlphaBlend(true);

	renderer->PushTransformMatrix();
	renderer->RotateZ( rnd_data->rotation.z ); 
	renderer->Translate(rnd_data->position.x, rnd_data->position.y, 0.0f);
	
  // matrix api
	renderer->DrawTexture(
		texture, 
		0.0f,
		0.0f,
		(float)texture->GetWidth(),
		(float)texture->GetHeight());

	
	renderer->DrawRectColoredWire(
		0.f, 
		0.f, 
		(float)texture->GetWidth(), 
		(float)texture->GetHeight(),
		0xff00ffff);

  // old code
	renderer->DrawTexture(
		texture, 
		rnd_data->position.x,
		rnd_data->position.y,
		(float)texture->GetWidth(),
		(float)texture->GetHeight());

  	renderer->DrawRectColoredWire(
		rnd_data->position.x, 
		rnd_data->position.y, 
		(float)texture->GetWidth(), 
		(float)texture->GetHeight(),
		0xff00ffff);

	renderer->PopTransformMatrix();
*/
