#ifndef CGAME_H
#define CGAME_H

// forward declaration
class CWorld;
class CWndBase;
class CWndButton;

enum EGameState
{
	GAME_STATE_MENU,
	GAME_STATE_WORLD
};

// Game UI class.
class CFrontend
{
public:
	CFrontend();
	~CFrontend();

	void Init();
	void Shutdown();

	void Update();

	// Main Menu
	void OpenMainMenu();
	void CloseMainMenu();

	// In-Game HUD
	void OpenHUD();
	void CloseHUD();

public:
	CWndBase* m_pCursor;

private:
	CWndBase* m_pMainMenu;
	CWndButton* m_pNewGameButton;

	CWndBase* m_pHealthImage;
	CWndBase* m_pHealthText;

	CWndBase* m_pWeaponText;
};

class CGame
{
public:
	CGame();
	~CGame();
	
	void Init();
	void Shutdown();

	void Update();

	void Render();
	
	void LoadWorld(const char* filename);

public:
	CWorld* world;

private:
	CFrontend frontend;
};

extern CGame* game;

#endif
