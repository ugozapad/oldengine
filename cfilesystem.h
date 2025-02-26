// cfilesystem.h: interface for the cfilesystem class.
//
//////////////////////////////////////////////////////////////////////

#ifndef CFILESYSTEM_H
#define CFILESYSTEM_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "cstr.h"
#include "carray.h"

class CSearchPath
{
public:
	CSearchPath(const char* pathname, const char* fspath);
	~CSearchPath();

private:
	CStr name;
	CStr path;
};

class CFileSystem  
{
public:
	CFileSystem();
	~CFileSystem();

	void AddSearchPath(const char* pathname, const char* fspath);

private:
	CArray<CSearchPath*> searchPathes;

};

extern CFileSystem filesys;

#endif // !CFILESYSTEM_H
