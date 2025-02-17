#ifndef CFONTMANAGER_H
#define CFONTMANAGER_H

#define MAX_FONT_TEX_WIDTH 256
#define MAX_FONT_TEX_HEIGHT 128
#define ASCII_START 32
#define NUM_CHARS 128

class CTexture;

struct SCharacter
{
	float minx, miny;
	float maxx, maxy;
	//float advance;
};

class CFont
{
public:
	SCharacter chars[NUM_CHARS];
	char texturename[260];
	CTexture* texture;
public:
	CFont();
	~CFont();

	void Load(const char* filename);
	void Save(const char* filename);

	void Print(float x, float y, const char* text);

	inline float GetSymbolHeight() { return chars[0].maxy - chars[0].miny * MAX_FONT_TEX_HEIGHT; }
};

class CFontManager
{
public:
	CFontManager(void);
	~CFontManager(void);

	void Init();
	void Shutdown();

	CFont* CompileFont(const char* fontname, const char* texturename, int fontsize);

public:
	CFont* sysfont;
};

extern CFontManager fontman;

#endif // !CFONTMANAGER_H
