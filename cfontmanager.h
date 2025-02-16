#ifndef CFONTMANAGER_H
#define CFONTMANAGER_H

class CFont
{
	
};

class CFontManager
{
public:
	CFontManager(void);
	~CFontManager(void);

	void Init();
	void Shutdown();

	CFont* CompileFont(const char* fontname, int size);
};

extern CFontManager fontman;

#endif // !CFONTMANAGER_H
