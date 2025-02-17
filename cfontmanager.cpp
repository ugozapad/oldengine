#include <windows.h>
#include <stdio.h>
#include "ctypes.h"
#include "cfontmanager.h"
#include "crenderer.h"

CFontManager fontman;

CFontManager::CFontManager(void)
{
	sysfont = NULL;
}

CFontManager::~CFontManager(void)
{
}

void CFontManager::Init()
{
	bool found = false;
	FILE* fp = fopen("data/systemfont.fnt", "rb");
	if (fp)
	{
		fclose(fp);
		found=true;
	}

	if (found == false)
	{
		CFont* systemFont = CompileFont("Lucida Console", "data/systemfont.bmp", 12);
		systemFont->Save("data/systemfont.fnt");
		delete systemFont;
	}

	sysfont = new CFont();
	sysfont->Load("data/systemfont.fnt");
}

void CFontManager::Shutdown()
{
	delete sysfont; sysfont = NULL;
}

BOOL SaveHBITMAPToFile(HBITMAP hBitmap, LPCTSTR lpszFileName)
{
	HDC hDC;
	int iBits;
	WORD wBitCount;
	DWORD dwPaletteSize = 0, dwBmBitsSize = 0, dwDIBSize = 0, dwWritten = 0;
	BITMAP Bitmap0;
	BITMAPFILEHEADER bmfHdr;
	BITMAPINFOHEADER bi;
	LPBITMAPINFOHEADER lpbi;
	HANDLE fh, hDib, hPal, hOldPal2 = NULL;
	hDC = CreateDC(TEXT("DISPLAY"), NULL, NULL, NULL);
	iBits = GetDeviceCaps(hDC, BITSPIXEL) * GetDeviceCaps(hDC, PLANES);
	DeleteDC(hDC);
	if (iBits <= 1)
		wBitCount = 1;
	else if (iBits <= 4)
		wBitCount = 4;
	else if (iBits <= 8)
		wBitCount = 8;
	else
		wBitCount = 24;
	GetObject(hBitmap, sizeof(Bitmap0), (LPSTR)&Bitmap0);
	bi.biSize = sizeof(BITMAPINFOHEADER);
	bi.biWidth = Bitmap0.bmWidth;
	bi.biHeight = -Bitmap0.bmHeight;
	bi.biPlanes = 1;
	bi.biBitCount = wBitCount;
	bi.biCompression = BI_RGB;
	bi.biSizeImage = 0;
	bi.biXPelsPerMeter = 0;
	bi.biYPelsPerMeter = 0;
	bi.biClrImportant = 0;
	bi.biClrUsed = 256;
	dwBmBitsSize = ((Bitmap0.bmWidth * wBitCount + 31) & ~31) / 8
		* Bitmap0.bmHeight;
	hDib = GlobalAlloc(GHND, dwBmBitsSize + dwPaletteSize + sizeof(BITMAPINFOHEADER));
	lpbi = (LPBITMAPINFOHEADER)GlobalLock(hDib);
	*lpbi = bi;

	hPal = GetStockObject(DEFAULT_PALETTE);
	if (hPal)
	{
		hDC = GetDC(NULL);
		hOldPal2 = SelectPalette(hDC, (HPALETTE)hPal, FALSE);
		RealizePalette(hDC);
	}

	GetDIBits(hDC, hBitmap, 0, (UINT)Bitmap0.bmHeight, (LPSTR)lpbi + sizeof(BITMAPINFOHEADER)
		+ dwPaletteSize, (BITMAPINFO*)lpbi, DIB_RGB_COLORS);

	if (hOldPal2)
	{
		SelectPalette(hDC, (HPALETTE)hOldPal2, TRUE);
		RealizePalette(hDC);
		ReleaseDC(NULL, hDC);
	}

	fh = CreateFile(lpszFileName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN, NULL);

	if (fh == INVALID_HANDLE_VALUE)
		return FALSE;

	bmfHdr.bfType = 0x4D42; // "BM"
	dwDIBSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + dwPaletteSize + dwBmBitsSize;
	bmfHdr.bfSize = dwDIBSize;
	bmfHdr.bfReserved1 = 0;
	bmfHdr.bfReserved2 = 0;
	bmfHdr.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER) + (DWORD)sizeof(BITMAPINFOHEADER) + dwPaletteSize;

	WriteFile(fh, (LPSTR)&bmfHdr, sizeof(BITMAPFILEHEADER), &dwWritten, NULL);

	WriteFile(fh, (LPSTR)lpbi, dwDIBSize, &dwWritten, NULL);
	GlobalUnlock(hDib);
	GlobalFree(hDib);
	CloseHandle(fh);
	return TRUE;
}

CFont* CFontManager::CompileFont(const char* fontname, const char* texturename, int fontsize)
{
	printf("Compining font %s %i\n", fontname, fontsize);

	// generate bitmap

	DWORD* pBitmapBits = NULL;
	BITMAPINFO bmi;
	memset(&bmi.bmiHeader, 0, sizeof(bmi.bmiHeader));
	bmi.bmiHeader.biSize		= sizeof(BITMAPINFOHEADER);
	bmi.bmiHeader.biWidth		= MAX_FONT_TEX_WIDTH;
	bmi.bmiHeader.biHeight		= -MAX_FONT_TEX_HEIGHT;
	bmi.bmiHeader.biPlanes		= 1;
	bmi.bmiHeader.biCompression = BI_RGB;
	bmi.bmiHeader.biBitCount	= 32;
	bmi.bmiHeader.biSizeImage	= ((bmi.bmiHeader.biWidth * bmi.bmiHeader.biBitCount + 31) & -31) * bmi.bmiHeader.biHeight;

	// Create a DC and a bitmap for the font
	HDC hDC = CreateCompatibleDC(NULL);
	HBITMAP hbmBitmap = CreateDIBSection(
		hDC,
		&bmi,
		DIB_RGB_COLORS,
		(void**)&pBitmapBits,
		NULL,
		0
	);
	
	SetMapMode(hDC, MM_TEXT);

	// Create a font.  By specifying ANTIALIASED_QUALITY, we might get an
	// antialiased font, but this is not guaranteed.
	int iHeight = -MulDiv(
		fontsize,
		(int)(GetDeviceCaps(hDC, LOGPIXELSY)),
		72
	);

	HFONT hFont = CreateFont(
		iHeight, 0, 0, 0,
		FW_NORMAL, // no bold
		FALSE, // no italic
		FALSE,
		FALSE,
		DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS,
		ANTIALIASED_QUALITY,
		VARIABLE_PITCH,
		fontname
	);

	SelectObject(hDC, hbmBitmap);
	SelectObject(hDC, hFont);

	// Set text properties
	SetTextColor(hDC, RGB(255, 255, 255));
	SetBkColor(hDC, 0x00000000);
	SetTextAlign(hDC, TA_TOP);

	// create font
	CFont* font = new CFont();

	strcpy(font->texturename, texturename);

	// Loop through all printable character and output them to the bitmap..
	// Meanwhile, keep track of the corresponding tex coords for each character.
	uint x = 0;
	uint y = 0;
	char str[2] = "\0";
	SIZE size = { 0, 0 };

	for (unsigned short c = ASCII_START; c < NUM_CHARS; c++)
	{
		str[0] = static_cast<unsigned char>(c);

		GetTextExtentPoint32(hDC, str, 1, &size);

		if (uint((x + size.cx + 1)) > MAX_FONT_TEX_WIDTH)
		{
			x = 0;
			y += size.cy + 1;
		}

		ExtTextOut(hDC, x + 0, y + 0, ETO_OPAQUE, NULL, str, 1, NULL);

		SCharacter cords;
		cords.minx = ((float)(x + 0)) / MAX_FONT_TEX_WIDTH;
		cords.miny = ((float)(y + 0)) / MAX_FONT_TEX_HEIGHT;
		cords.maxx = ((float)(x + 0 + size.cx)) / MAX_FONT_TEX_WIDTH;
		cords.maxy = ((float)(y + 0 + size.cy)) / MAX_FONT_TEX_HEIGHT;

		font->chars[c - ASCII_START] = cords;
		x += size.cx + 1;
	}

	// write bitmap
	SaveHBITMAPToFile(hbmBitmap, texturename);

	DeleteObject(hbmBitmap);
	DeleteDC(hDC);
	DeleteObject(hFont);

	return font;
}

CFont::CFont()
{
	texture = NULL;
	memset(chars, 0, sizeof(chars));
	texturename[0] = '\0';
}

CFont::~CFont()
{
}

void CFont::Load(const char* filename)
{
	FILE* file = fopen(filename, "rb");
	if (!file)
	{
		printf("CFont::Load: failed to open %s\n", filename);
		return;
	}

	byte version;
	fread(&version, sizeof(version), 1, file);
	fread(texturename, sizeof(texturename), 1, file);
	fread(chars, sizeof(chars), 1, file);
	fclose(file);

	// create texture
	texture = renderer->CreateTexture();
	texture->LoadFromFile(texturename);
}

void CFont::Save(const char* filename)
{
	FILE* file = fopen(filename, "wb");
	if (!file)
	{
		printf("CFont::Save: failed to save %s\n", filename);
		return;
	}

	byte version = 1;
	fwrite(&version, sizeof(version), 1, file);
	fwrite(texturename, sizeof(texturename), 1, file);
	fwrite(chars, sizeof(chars), 1, file);
	fclose(file);
}

void CFont::Print(float x, float y, const char* text)
{
	renderer->SetAlphaBlend(true);
	renderer->SetBlackAlpha(true);

	float x2 = x;
	float y2 = y;
	float symbolHeight = GetSymbolHeight();
	float width = 0.0f;
	for (; *text; text++)
	{
		unsigned char c = *text;
		if (c == 13) continue;
		if (c == 10)
		{
			x2 = x;
			y2 += symbolHeight;
			width = 0.0f;
			continue;
		}

		const float fW = (chars[c - ASCII_START].maxx - chars[c - ASCII_START].minx) * MAX_FONT_TEX_WIDTH;
		const float fH = (chars[c - ASCII_START].maxy - chars[c - ASCII_START].miny) * MAX_FONT_TEX_HEIGHT;
		width += fW;
		if (c != ' ')
		{
			renderer->DrawTile(texture, x2, y2, fW, fH,
				chars[c - ASCII_START].minx, chars[c - ASCII_START].miny, 
				chars[c - ASCII_START].maxx, chars[c - ASCII_START].maxy);
		}

		x2 += fW;
	}

	renderer->SetBlackAlpha(false);
}

float CFont::GetTextWidth(const char* text)
{
	float width = 0.0f;
	for (; *text; text++)
	{
		unsigned char c = *text;
		if (c == 13) continue;
		if (c == 10)
		{
			width = 0.0f;
			continue;
		}

		const float fW = (chars[c - ASCII_START].maxx - chars[c - ASCII_START].minx) * MAX_FONT_TEX_WIDTH;
		const float fH = (chars[c - ASCII_START].maxy - chars[c - ASCII_START].miny) * MAX_FONT_TEX_HEIGHT;
		width += fW;
	}

	return width;
}
