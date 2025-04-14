#ifndef CWORLD_H
#define CWORLD_H

#include <stdio.h>
#include "carray.h"
#include "cobject.h"

class CQuadTree;

/*
***********************************************************************************************
Game and World architecture :

Object is the base class for 2D\3D dynamic or static object, it have position,
orientation and visual representation. Object class itself doesn't have any game logic for 
more extensibility engine user should inheritance from Object and has free to make any
logic or dependency for their own objects.

Custom objects must be registered through the object factory, and since most objects
are persistable, engine created objects by using this factory.

World contains all objects that will be live in there. Object access can be done by name or ID.
***********************************************************************************************
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

class CLevelController : public CObject
{
public:
	CLevelController();
	~CLevelController();

public:
	int m_score;
};

extern CLevelController* levelcontroller;

/*
*  Chunks list:
*   - Object [CHUNK_OBJECT]
*   - Textures [CHUNK_TEXTURES]
*/

class CResource
{
	enum EResourceType
	{
		RESTYPE_NONE,
		RESTYPE_TEXTURE,
		RESTYPE_VISUAL,
		RESTYPE_SOUND,

		RESTYPE_MAX
	};

public:
	union
	{
		class CTexture* m_pTexture;
		class CVisual* m_pVisual;
		class CSound* m_pSound;
	};

	EResourceType m_resourceType;

public:
	CResource()
	{
		m_pTexture = NULL;
		m_pVisual = NULL;
		m_pSound = NULL;
		m_resourceType = CResource::RESTYPE_NONE;
	}

	~CResource()
	{
		m_pTexture = NULL;
		m_pVisual = NULL;
		m_pSound = NULL;
	}

	void Load(const char* filename, CResource::EResourceType resourceType);


};

class CResourceManager
{
};

#endif
