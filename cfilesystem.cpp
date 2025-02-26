// cfilesystem.cpp: implementation of the CFileSystem class.
//
//////////////////////////////////////////////////////////////////////

#include "cfilesystem.h"

// Global instance of the CFileSystem class.
CFileSystem filesys;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSearchPath::CSearchPath(const char* pathname, const char* fspath)
{
	name = pathname;
	path = fspath;
}

CSearchPath::~CSearchPath()
{
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFileSystem::CFileSystem()
{
	AddSearchPath("root", "");	
	AddSearchPath("data", "data/");	
}

CFileSystem::~CFileSystem()
{
	int numPathes = searchPathes.GetSize();
	for (int i = 0; i < numPathes; i++)
	{
		delete searchPathes[i];
	}

	searchPathes.RemoveAll();
}

void CFileSystem::AddSearchPath(const char* pathname, const char* fspath)
{
	CSearchPath* searchPath = new CSearchPath(pathname, fspath);
	searchPathes.Add(searchPath);
}