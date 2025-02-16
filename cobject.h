#ifndef COBJECT_H
#define COBJECT_H

class CVisual;

enum EObjectType
{
	OBJTYPE_OBJ,
	OBJTYPE_PLAYER
};

class CObject
{
public:
	CObject();
	~CObject();

	void Update();

	void LoadSprite(const char* filename);
	
public:
	CVisual* visual;
};

#endif
