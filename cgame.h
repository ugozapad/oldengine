#ifndef CGAME_H
#define CGAME_H

// forward declaration
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

	// Main Menu
	void OpenMainMenu();
	void CloseMainMenu();

	// In-Game HUD
	void OpenHUD();
	void CloseHUD();

private:
	CWndBase* m_pMainMenu;
	CWndButton* m_pNewGameButton;

	CWndBase* m_pHealthImage;
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

private:
	CFrontend frontend;
};

extern CGame* game;

#endif
