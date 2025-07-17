#include <assert.h>
#include <stdlib.h>
#include "crenderer.h"
#include "cobject.h"
#include "cplayer.h"
#include "cworld.h"

CObjectFactory objfactory;
const char* objectTypeNames[OBJTYPE_COUNT] = { "CObject", "CPlayer", "CLevelController", "CNpc" };

CObject::CObject()
{
	visual = NULL;
	
	memset(&position, 0, sizeof(position));
	memset(&rotation, 0, sizeof(rotation));
	scale = 1.0f;
	
	bounds = BoundingBoxIdentity(bounds);
}

CObject::~CObject()
{
	//visual = NULL;
	
	if (visual)
	{
		delete visual; visual = NULL;
	}
}

void CObject::Update()
{
}

void CObject::Render()
{
	static SRenderData renderData;
	
	if (!visual)
		return;
	
	renderData.position.x=position.x;
	renderData.position.y=position.y;
	renderData.position.z=position.z;
	renderData.rotation.x=rotation.x;
	renderData.rotation.y=rotation.y;
	renderData.rotation.z=rotation.z;
	renderData.scale=scale;
	visual->Render(&renderData);

	renderer->PushTransformMatrix();
	renderer->Translate(position.x, position.y, 0.0f);

	renderer->DrawBounds2DColoredWire(bounds, 0xffff00ff);

	renderer->PopTransformMatrix();
}

void CObject::LoadVisual(const char* filename)
{
	if (strstr(filename, ".jpg") || strstr(filename, ".png") || strstr(filename, ".bmp"))
	{
		visual = renderer->CreateVisual(VISUAL_SPRITE);
		visual->Load(filename);
	
		CTexture* texture = texcontainer->LoadTexture(filename);

		// move picture to the center
		float w = (float)texture->GetWidth();
		float h = (float)texture->GetHeight();
		float x = (w * 0.5f);
		float y = (h * 0.5f);

		bounds.min.x = x;
		bounds.min.x = y;
		bounds.max.x = w;
		bounds.max.y = h;
	}
}

CObject* CObjectFactory::CreateByType(int type)
{
	assert(type<=OBJTYPE_COUNT);
	
	CObject* object = NULL;
	
	switch (type)
	{
	case OBJTYPE_OBJ:
		object = new CObject();
		break;
	case OBJTYPE_PLAYER:
		object = new CPlayer();
		break;

	case OBJTYPE_LEVELCONTROLLER:
		object = new CLevelController();
		break;
	}
	
	assert(object);
	
	return object;
}

CObject* CObjectFactory::CreateByName(const char* classname)
{
	assert(classname);
	
	for (int i = 0; i < OBJTYPE_COUNT; i++)
	{
		if (strcmp(objectTypeNames[i], classname) == 0)
			return CreateByType(i);
	}
	
	return NULL;	
}

