#include <windows.h>
#include <stdio.h>
#include "cfontmanager.h"

#define MAX_FONT_TEX_WIDTH 256
#define MAX_FONT_TEX_HEIGHT 256

CFontManager fontman;

CFontManager::CFontManager(void)
{
}

CFontManager::~CFontManager(void)
{
}

void CFontManager::Init()
{
	bool found = false;
	FILE* fp = fopen("data/system.fnt", "rb");
	if (fp)
	{
		fclose(fp);
		found=true;
	}

	if (found == false)
	{
		CFont* systemFont = CompileFont("Lucida Console", 12);
	}
}

void CFontManager::Shutdown()
{
}

CFont* CFontManager::CompileFont(const char* fontname, int size)
{
	// generate bitmap
	
	BITMAPINFO bmi;
	memset(&bmi.bmiHeader, 0, sizeof(bmi.bmiHeader));
	bmi.bmiHeader.biSize		= sizeof(BITMAPINFOHEADER);
	bmi.bmiHeader.biWidth		= MAX_FONT_TEX_WIDTH;
	//bmi.bmiHeader.biHeight		= -(s32)m_dwTextureHeight;
	bmi.bmiHeader.biPlanes		= 1;
	bmi.bmiHeader.biCompression = BI_RGB;
	bmi.bmiHeader.biBitCount	= 32;
	bmi.bmiHeader.biSizeImage	= ((bmi.bmiHeader.biWidth * bmi.bmiHeader.biBitCount + 31) & -31) * bmi.bmiHeader.biHeight;


	return NULL;
}