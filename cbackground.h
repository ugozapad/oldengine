#ifndef CBACKGROUND_H
#define CBACKGROUND_H

#include "crenderer.h"

class CBackground
{
public:
	void Init();
	void Shutdown();

	void Render();

private:
	CVisual* visual;
};

#endif // !CBACKGROUND_H
