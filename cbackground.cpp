#include "crenderer.h"
#include "cbackground.h"

#define SKY_X 128
#define SKY_Y 128

void CBackground::Init()
{
	visual = renderer->CreateVisual(VISUAL_SPRITE);
	visual->Load("data/textures/sky.bmp");
}

void CBackground::Shutdown()
{
}

void CBackground::Render()
{
	const SViewport& viewport = renderer->GetViewport();

	SRenderData renderData;
	memset(&renderData, 0, sizeof(renderData));
	renderData.scale = 1.0f;

	int width;
	int height;

	for (height = 0; height <= viewport.height; height += SKY_Y)
	{
		for (width = 0; width <= viewport.width; width += SKY_X)
		{
			renderData.position.x = (float)width;
			renderData.position.y = (float)height;
			visual->Render(&renderData);
		}
	}

	//for (; width <= viewport.width, height <= viewport.height; width += SKY_X, height += SKY_Y)
	//{
	//	renderData.position.x = (float)width;
	//	renderData.position.y = (float)height;
	//	visual->Render(&renderData);
	//}
}
