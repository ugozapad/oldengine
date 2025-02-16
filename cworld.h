#ifndef CWORLD_H
#define CWORLD_H

#include "carray.h"

class CObject;

class CResourceManager
{
};

class CWorld
{
public:
	CWorld();
	~CWorld();

	void Update();
	void InsertObject(CObject* object);

private:
	CArray<CObject*> objects;
};

#endif
