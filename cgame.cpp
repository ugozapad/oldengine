#include <stdio.h>
#include "ctimer.h"
#include "cinput.h"
#include "crenderer.h"
#include "cwnd.h"
#include "cgame.h"
#include "cworld.h"

CGame* game = NULL;

CFrontend::CFrontend()
{
	m_pCursor = NULL;
	m_pMainMenu = NULL;
	m_pNewGameButton = NULL;
	m_pHealthImage= NULL;
	m_pHealthText = NULL;
}

CFrontend::~CFrontend()
{
	Shutdown();
}

void CFrontend::Init()
{
	m_pCursor = new CWndBase();
	m_pCursor->SetTexture("data/textures/ui_cursor.png");
	g_pDesktop->AddChildren(m_pCursor);

	m_pMainMenu = new CWndBase();
	m_pMainMenu->SetTexture("data/textures/mainbg.jpg");

	SViewport viewport = renderer->GetViewport();
	m_pMainMenu->SetSize((float)viewport.width, (float)viewport.height);

	m_pNewGameButton = new CWndButton();
	m_pNewGameButton->SetTexture("data/textures/btn_newgame.png");
	m_pNewGameButton->SetPosition(200.f, 200.f);
	m_pMainMenu->AddChildren(m_pNewGameButton);

	// HUD
	float width = 95.0f;
	float height = (float)viewport.height - 50.0f;
	m_pHealthImage = new CWndBase();
	m_pHealthImage->SetTexture("data/textures/health.png");
	m_pHealthImage->SetPosition(width, height);
	m_pHealthImage->SetSize(128.0f, 128.0f);
	m_pHealthImage->CalculateAnchorCenter();

	m_pHealthText = new CWndBase();
	m_pHealthText->SetPosition(m_pHealthImage->m_Position.x + m_pHealthImage->m_Size.x, height - 5.0f);
	m_pHealthText->SetSize(128.0f, 128.0f);
	m_pHealthText->SetText("100");
}

void CFrontend::Shutdown()
{
	if (!g_pDesktop)
		return;

	delete m_pHealthText; m_pHealthText = NULL;
	delete m_pHealthImage; m_pHealthImage = NULL;

	if (m_pMainMenu)
	{
		m_pNewGameButton->RemoveChildren(m_pNewGameButton);
		delete m_pNewGameButton; m_pNewGameButton = NULL;
		delete m_pMainMenu; m_pMainMenu = NULL;
	}

	g_pDesktop->RemoveChildren(m_pCursor);
}

void CFrontend::Update()
{
	int x, y;
	input->GetMousePos(&x, &y);
	m_pCursor->m_Position.x = (float)x;
	m_pCursor->m_Position.y = (float)y;
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
	g_pDesktop->AddChildren(m_pHealthText);
}

void CFrontend::CloseHUD()
{
	g_pDesktop->RemoveChildren(m_pHealthText);
	g_pDesktop->RemoveChildren(m_pHealthImage);
}

///////////////////////////////////////////////////////////////////////////////

CGame::CGame()
{
	world = NULL;
}

CGame::~CGame()
{
	if (world)
	{
		delete world; world = NULL;
	}
}

void CGame::Init()
{
	// initialize UI library

	SViewport viewport = renderer->GetViewport();

	g_pDesktop = new CWndBase();
	g_pDesktop->SetSize((float)viewport.width, (float)viewport.height);

	frontend.Init();
	frontend.OpenHUD();
	
	LoadWorld("data/levels/test.txt");
}

void CGame::Shutdown()
{
	frontend.CloseHUD();
	frontend.CloseMainMenu();
	frontend.Shutdown();

	delete g_pDesktop; g_pDesktop = NULL;
}

void CGame::LoadWorld(const char* filename)
{
	static char worldFilename[260 + 1];
	strcpy(worldFilename, filename);
	
	printf("Loading world %s\n", filename);
	
	// Check for file
	FILE* file = fopen(worldFilename, "rb");
	if (!file)
	{
		// check for compiled world
		sprintf(worldFilename, "data/worlds/%s.cwf", filename);
		file = fopen(worldFilename, "rb");
		if (!file)
		{
			// check for raw world
			sprintf(worldFilename, "data/worlds/%s.txt", filename);
			file = fopen(worldFilename, "r");
			if (!file)
			{
				printf("CGame::LoadWorld: World '%s' is not found\n", filename);
				return;
			}
		}
	}
	
	fclose(file);
	
	static bool s_worldTransition = false;
	if (!s_worldTransition)
	{
		if (world) delete world;
		
		world = new CWorld();
		world->LoadWorld(worldFilename);
	}
}

void CGame::Update()
{
	world->Update();

	frontend.Update();
}

void CGame::Render()
{
	// render world
	world->Render();
	
	// Draw UI
	g_pDesktop->Render();
}
