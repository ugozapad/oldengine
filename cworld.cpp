#include "cworld.h"
#include "cobject.h"

CWorld::CWorld()
{
}

CWorld::~CWorld()
{
	objects.RemoveAll();
}

void CWorld::Update()
{
	int numObjects = objects.GetSize();
	for (int i = 0; i < numObjects; i++)
	{
		CObject* object = objects[i];
		if (object)
			object->Update();
	}
}

void CWorld::InsertObject(CObject* object)
{
	// if object already exist
	if ( objects.Contains(object) )
	{
		assert(object && "Trying to add already existed object");
		return;
	}

	objects.Add( object ); 
}