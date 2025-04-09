#include "cwnd.h"
#include "cinput.h"
#include "crenderer.h"
#include "cfontmanager.h"

CWndBase* g_pDesktop = NULL;

bool CWndBase::ms_bDrawDebug = false;

CWndBase::CWndBase(void)
{
	m_pTexture = NULL;
	m_pFont = NULL;
	m_Position.x = 0.f;
	m_Position.y = 0.f;
	m_Size.x = 0.f;
	m_Size.y = 0.f;
}

CWndBase::~CWndBase(void)
{
	m_Text.Clear();
	m_Children.RemoveAll();

	m_pFont = NULL;
	m_pTexture = NULL;
}

void CWndBase::Render()
{
	uint Flags = WNDFLAG_TRANSPARENT;

	// render texture 
	if (m_pTexture)
	{
		if (Flags & WNDFLAG_TRANSPARENT)
			renderer->SetAlphaBlend(true);

		renderer->DrawTexture(m_pTexture, m_Position.x, m_Position.y, 
			m_Size.x, m_Size.y);
	}

	// render text
	if (!m_Text.IsEmpty())
	{
		// if font doesn't initialized - trying to use system font
		if (m_pFont == NULL)
			m_pFont = fontman.sysfont;

		m_pFont->Print(m_Position.x, m_Position.y, m_Text.GetText());
	}

	// draw debug rect
	if (ms_bDrawDebug)
	{
		renderer->DrawRectColoredWire(m_Position.x, m_Position.y,
			m_Size.x, m_Size.y,
			0xff0000ff);
	}

	// render children
	int numChildren = m_Children.GetSize();
	for (int i = 0; i < numChildren; i++)
	{
		CWndBase* pWnd = m_Children[i];
		if (pWnd) pWnd->Render();
	}
}

void CWndBase::SetPosition(float x, float y)
{
	m_Position.x = x;
	m_Position.y = y;
}

void CWndBase::SetSize(float x, float y)
{
	m_Size.x = x;
	m_Size.y = y;
}

void CWndBase::SetTexture(const char* filename)
{
	m_pTexture = texcontainer->LoadTexture(filename);
	
	// #TODO: FLAG ???
	AutoResize();
}

void CWndBase::SetText(const char* text)
{
	if (m_Text != text)
		m_Text = text;

	
	// #TODO: FLAG ???
	AutoResize();
}

void CWndBase::AutoResize()
{
	if (m_Text.IsEmpty())
	{
		//printf("CWndBase::AutoResize: canno't resize without text\n");
	}
	else
	{
		m_Size.x = fontman.sysfont->GetTextWidth(m_Text.GetText());
		m_Size.y = -fontman.sysfont->GetSymbolHeight();
	}

	if (m_pTexture)
	{
		m_Size.x = (float)m_pTexture->GetWidth();
		m_Size.y = (float)m_pTexture->GetHeight();
	}
	else
	{
		//printf("CWndBase::AutoResize: canno't resize without texture\n");
	}
}

void CWndBase::CalculateAnchorCenter()
{
	m_Position.x = m_Position.x - (m_Size.x * 0.5f);
	m_Position.y = m_Position.y - (m_Size.y * 0.5f);
}

void CWndBase::AddChildren(CWndBase* pWnd)
{
	assert( pWnd );

	// if children already added
	if ( m_Children.Contains( pWnd ) )
	{
		assert( !pWnd && "Trying to add already added wnd" );
		return;
	}

	m_Children.Add( pWnd ); 
}

void CWndBase::RemoveChildren(CWndBase* pWnd)
{
	assert( pWnd );

	// if children already added
	if ( !m_Children.Contains( pWnd ) )
	{
		assert( pWnd && "Trying to remove unknowed wnd" );
		return;
	}
	
	int idx = m_Children.IndexOf(pWnd);
	m_Children.Remove(idx);
}

CWndButton::CWndButton()
{
}

CWndButton::~CWndButton()
{
}

void CWndButton::Render()
{
	int mx, my;
	input->GetMousePos(&mx, &my);

	float x = m_Position.x;
	float y = m_Position.y;
	float cx = m_Position.x + m_Size.x;
	float cy = m_Position.y + m_Size.y;
	bool hovered = false;

	if (x <= (float)mx && (float)mx <= cx &&
		y <= (float)my && (float)my <= cy)
		hovered = true;

	CWndBase::Render();
}

CWndDebugText debugText;

CWndDebugText::CWndDebugText()
{
}

CWndDebugText::~CWndDebugText()
{
}

void CWndDebugText::AddText(const char* text)
{
	
}

void CWndDebugText::Render()
{
	CWndBase::Render();
	
	/*
	int numStrings = m_Strings.GetSize();
	float width 
	for (int i = 0; i < )
	
	m_Strings.RemoveAll();
	*/
}
