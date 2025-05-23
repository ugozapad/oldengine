#ifndef COBJECT_H
#define COBJECT_H

#include "cmath.h"

class CVisual;

enum EObjectType
{
	OBJTYPE_OBJ,
	OBJTYPE_PLAYER,
	OBJTYPE_LEVELCONTROLLER,
	OBJTYPE_COUNT
};

class CObject
{
public:
	CObject();
	~CObject();

	virtual void Update();
	virtual void Render();

	void LoadVisual(const char* filename);
	
public:
	CVisual* visual;
	Vec3 position;
	Vec3 rotation;
	float scale;
};

class CObjectFactory
{
public:
	CObject* CreateByType(int type);
	CObject* CreateByName(const char* classname);
	
};

extern CObjectFactory objfactory;

#endif
