#include <assert.h>
#include <stdlib.h>
#include "crenderer.h"
#include "cobject.h"
#include "cplayer.h"
#include "cworld.h"

CObjectFactory objfactory;
const char* objectTypeNames[OBJTYPE_COUNT] = { "CObject", "CPlayer", "CLevelController" };

CObject::CObject()
{
	visual = NULL;
	
	memset(&position, 0, sizeof(position));
	memset(&rotation, 0, sizeof(rotation));
	scale = 1.0f;
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
}

void CObject::LoadVisual(const char* filename)
{
	if (strstr(filename, ".jpg") || strstr(filename, ".png") || strstr(filename, ".bmp"))
	{
		visual = renderer->CreateVisual(VISUAL_SPRITE);
		visual->Load(filename);
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

