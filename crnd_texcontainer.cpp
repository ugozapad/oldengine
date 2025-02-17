#include <stdio.h>
#include <string.h>
#include "crenderer.h"

CTextureContainer* texcontainer = NULL;

CTextureContainer::CTextureContainer()
{
}

CTextureContainer::~CTextureContainer()
{
	int numTextures = textures.GetSize();
	if (numTextures > 0)
	{
		for (int i = 0; i < numTextures; i++)
		{
			if (textures[i].texture)
			{
				delete textures[i].texture;
				textures[i].texture = NULL;
			}
		}

		textures.RemoveAll();
	}
}

CTexture* CTextureContainer::LoadTexture(const char* filename)
{
	int numTextures = textures.GetSize();
	for (int i = 0; i < numTextures; i++)
	{
		if (strcmp(textures[i].filename, filename) == 0)
			return textures[i].texture; 
	}

	// create texture

	printf("Uncached loading : %s\n", filename);

	CTexture* texture = renderer->CreateTexture();
	texture->LoadFromFile(filename);

	STextureEntry entry;
	strcpy(entry.filename, filename);
	entry.texture = texture;
	textures.Add(entry);

	return texture;
}
