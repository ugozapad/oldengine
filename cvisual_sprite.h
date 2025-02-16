#ifndef CVISUAL_SPRITE_H
#define CVISUAL_SPRITE_H

#include "crenderer.h"

class CVisual_Sprite : public CVisual
{
public:
	CVisual_Sprite();
	~CVisual_Sprite();

    void Render(const SRenderData* rnd_data);
    void Load(const char* filename);
    
private:
	CTexture* texture;
};

#endif
