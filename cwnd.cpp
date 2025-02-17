#include "cwnd.h"
#include "crenderer.h"
#include "cfontmanager.h"

CWndBase* g_pDesktop = NULL;

CWndBase::CWndBase(void)
{
	m_pTexture = NULL;

	m_Position.x = 0.f;
	m_Position.y = 0.f;
	m_Size.x = 0.f;
	m_Size.y = 0.f;
}

CWndBase::~CWndBase(void)
{
	m_Text.Clear();
	m_Children.RemoveAll();

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
		fontman.sysfont->Print(m_Position.x, m_Position.y, m_Text.GetText());
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
	m_Text = text;
}

void CWndBase::AutoResize()
{
	if (!m_pTexture)
	{
//		printf("CWndBase::AutoResize: canno't resize without texture\n");
		return;
	}

	m_Size.x = (float)m_pTexture->GetWidth();
	m_Size.y = (float)m_pTexture->GetHeight();
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
		assert( pWnd && "Trying to add already added wnd" );
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
	CWndBase::Render();
}