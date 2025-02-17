#ifndef CWND_H
#define CWND_H

#include "cmath.h"
#include "carray.h"
#include "cstr.h"

enum EWndFlags
{
	WNDFLAG_TRANSPARENT = 1 << 1,
};

class CTexture;

class CWndBase
{
public:
	CWndBase(void);
	virtual ~CWndBase(void);

	virtual void Render();

	void SetPosition(float x, float y);
	void SetSize(float x, float y);
	void SetTexture(const char* filename);
	void SetText(const char* text);

	void AutoResize();
	void CalculateAnchorCenter();
	

	void AddChildren(CWndBase* pWnd);
	void RemoveChildren(CWndBase* pWnd);

public:
	static bool ms_bDrawDebug;

	CArray<CWndBase*> m_Children;
	CTexture* m_pTexture;
	CStr m_Text;
	Vec2 m_Position;
	Vec2 m_Size;
};

extern CWndBase* g_pDesktop;

class CWndButton : public CWndBase
{
public:
	CWndButton();
	virtual ~CWndButton();

	virtual void Render();
};

#endif
