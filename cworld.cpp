#include <assert.h>
#include "cworld.h"
#include "cobject.h"
#include "crenderer.h"
#include "ctimer.h"
#include "cplayer.h"

CWorld::CWorld()
{
}

CWorld::~CWorld()
{
	objects.RemoveAll();
}

void CWorld::LoadWorld(const char* filename)
{
	bool isText = !!strstr(filename, ".txt");
	
	FILE* file = fopen(filename, isText ? "r" : "rb");
	if (!file)
	{
		printf("CWorld::LoadWorld: World '%s' is not found\n", filename);
		return;
	}
	
	if (isText)
	{
		LoadTextWorld(file);
	}
	
	fclose(file);
}

void RemoveNextLine(char *buffer, size_t size)
{
	for (size_t i = 0; i < size; i++)
	{
		if (buffer[i] == '\n')
			buffer[i] = '\0';
	}
}

void CWorld::LoadTextWorld(FILE* file)
{
	assert(file);
	
	bool beginObject = false;
	bool endObject = false;
	static char entitynameBuffer[256];
	static char classnameBuffer[256];
	static char visualnameBuffer[256];
	static char tagnameBuffer[256];
	
	Vec3 pos; memset(&pos, 0, sizeof(pos));
	Vec3 rot; memset(&rot, 0, sizeof(rot));
	entitynameBuffer[0] = '\0';
	classnameBuffer[0] = '\0';
	visualnameBuffer[0] = '\0';
	tagnameBuffer[0] = '\0';
	
	while (true)
	{
		if (endObject) {
			CObject* object = objfactory.CreateByName(classnameBuffer);
			object->position = pos;
			object->rotation = rot;
			object->LoadVisual(visualnameBuffer);
			InsertObject(object);
			
			endObject = false;

			memset(entitynameBuffer, 0, sizeof(entitynameBuffer));
			entitynameBuffer[0] = '\0';
			memset(classnameBuffer, 0, sizeof(classnameBuffer));
			classnameBuffer[0] = '\0';
			memset(visualnameBuffer, 0, sizeof(visualnameBuffer));
			visualnameBuffer[0] = '\0';
			memset(tagnameBuffer, 0, sizeof(tagnameBuffer));
			tagnameBuffer[0] = '\0';
		}

		char lineHeader[128];

		// read the first word of the line
		int res = fscanf(file, "%s", lineHeader);
		if (res == EOF)
			break; // EOF = End Of File. Quit the loop.
		
		if (lineHeader[0] == '#')
		{
			// Probably a comment, eat up the rest of the line
			static char stupidBuffer[1000];
			fgets(stupidBuffer, 1000, file);
			continue;
		}

		if (!beginObject)
		{
			if (strcmp(lineHeader, "BeginObject") == 0)
			{
				fscanf(file, "%s", entitynameBuffer);
				//fgets(entitynameBuffer, sizeof(entitynameBuffer), file);
				RemoveNextLine(entitynameBuffer, sizeof(entitynameBuffer));
				beginObject = true;
			}
		}
		else
		{
			if (strcmp(lineHeader, "ClassName") == 0) {
				fscanf(file, "%s", classnameBuffer);
				//fgets(classnameBuffer, sizeof(classnameBuffer), file);
				RemoveNextLine(classnameBuffer, sizeof(classnameBuffer));
			} else if (strcmp(lineHeader, "Visual") == 0) {
				fscanf(file, "%s", visualnameBuffer);
				//fgets(visualnameBuffer, sizeof(visualnameBuffer), file);
				RemoveNextLine(visualnameBuffer, sizeof(visualnameBuffer));
			} else if (strcmp(lineHeader, "Position") == 0) {
				fscanf(file, "%f %f %f\n", &pos.x, &pos.y, &pos.z);
			} else if (strcmp(lineHeader, "Rotation") == 0) {
				fscanf(file, "%f %f %f\n", &rot.x, &rot.y, &rot.z);
			} else if (strcmp(lineHeader, "EndObject") == 0) {
				beginObject = false;
				endObject = true;
			} /*else if (strcmp(lineHeader, "Tag") == 0) {
				fscanf(file, "%s", tagnameBuffer);
				RemoveNextLine(tagnameBuffer);
			} */
		}
	}

	fclose(file);
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

void CWorld::Render()
{
	int numObjects = objects.GetSize();
	for (int i = 0; i < numObjects; i++)
	{
		if (objects[i])
		{
			objects[i]->Render();
		}
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

void CWorld::RemoveObject(CObject* object)
{
	assert( object );

	// if object already added
	if ( !objects.Contains( object ) )
	{
		assert( object && "Trying to remove unknowed object" );
		return;
	}
	
	int idx = objects.IndexOf(object);
	objects.Remove(idx);
}
