#ifndef CWORLD_H
#define CWORLD_H

#include <stdio.h>
#include "carray.h"

class CObject;

class CResourceManager
{
};

/*
*  Chunks list:
*   - Object [CHUNK_OBJECT]				
*   - Textures [CHUNK_TEXTURES]
*/

class CWorld
{
public:
	CWorld();
	~CWorld();

	void LoadWorld(const char* filename);

private:
	void LoadTextWorld(FILE* file);
	
public:
	void Update();
	void Render();
	
	void InsertObject(CObject* object);
	void RemoveObject(CObject* object);

private:
	CArray<CObject*> objects;
};

#endif
