#include <stdio.h>
#include "cgame.h"
#include "crenderer.h"
#include "cinput.h"
#include "ctimer.h"
#include "cwnd.h"

CGame* game = NULL;

CFrontend::CFrontend()
{
	m_pMainMenu = NULL;
	m_pNewGameButton = NULL;
	m_pHealthImage= NULL;
}

CFrontend::~CFrontend()
{
	Shutdown();
}

void CFrontend::Init()
{
	m_pMainMenu = new CWndBase();
	m_pMainMenu->SetTexture("data/textures/mainbg.jpg");

	SViewport viewport = renderer->GetViewport();
	m_pMainMenu->SetSize((float)viewport.width, (float)viewport.height);

	m_pNewGameButton = new CWndButton();
	m_pNewGameButton->SetTexture("data/textures/btn_newgame.png");
	m_pNewGameButton->SetPosition(200.f, 200.f);
	m_pMainMenu->AddChildren(m_pNewGameButton);

	// HUD
	
	m_pHealthImage = new CWndBase();
	m_pHealthImage->SetTexture("data/textures/health.png");
	m_pHealthImage->SetPosition(200.0f, 200.0f);
	m_pHealthImage->SetSize(128.0f, 128.0f);
}

void CFrontend::Shutdown()
{
	delete m_pHealthImage; m_pHealthImage = NULL;

	if (m_pMainMenu)
	{
		m_pNewGameButton->RemoveChildren(m_pNewGameButton);
		delete m_pNewGameButton; m_pNewGameButton = NULL;
		delete m_pMainMenu; m_pMainMenu = NULL;
	}
}

void CFrontend::OpenMainMenu()
{
	g_pDesktop->AddChildren(m_pMainMenu);
}

void CFrontend::CloseMainMenu()
{
	g_pDesktop->RemoveChildren(m_pMainMenu);
}

void CFrontend::OpenHUD()
{
	g_pDesktop->AddChildren(m_pHealthImage);
}

void CFrontend::CloseHUD()
{
	g_pDesktop->RemoveChildren(m_pHealthImage);
}

CGame::CGame()
{
}

CGame::~CGame()
{
}

void CGame::Init()
{
	// initialize UI library

	SViewport viewport = renderer->GetViewport();

	g_pDesktop = new CWndBase();
	g_pDesktop->SetSize((float)viewport.width, (float)viewport.height);

	frontend.Init();
	//frontend.OpenHUD();
}

void CGame::Shutdown()
{
	frontend.Shutdown();

	delete g_pDesktop; g_pDesktop = NULL;
}

void CGame::Update()
{
}

void CGame::Render()
{
	// Draw UI
	g_pDesktop->Render();
}
