#include <stdlib.h>
#include "cobject.h"
#include "crenderer.h"

CObject::CObject()
{
	visual = NULL;
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
