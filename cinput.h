#ifndef CINPUT_H
#define CINPUT_H

enum EKeys
{
	// service keys

	KEY_BACKSPACE = 0x08,
	KEY_TAB = 0x09,
	KEY_SPACE = 0x20,
	KEY_RETURN = 0x0D,
	KEY_SHIFT = 0x10,
	KEY_CONTROL = 0x11,
	KEY_MENU = 0x12,
	KEY_PAUSE = 0x13,

	// numeric keys
	KEY_0 = 0x30,
	KEY_1 = 0x31,
	KEY_2 = 0x32,
	KEY_3 = 0x33,
	KEY_4 = 0x34,
	KEY_5 = 0x35,
	KEY_6 = 0x36,
	KEY_7 = 0x37,
	KEY_8 = 0x38,
	KEY_9 = 0x39,

	// alphabetic keys
	KEY_A = 'A',
	KEY_B, 
	KEY_C, 
	KEY_D, 
	KEY_E, 
	KEY_F, 
	KEY_G, 
	KEY_H, 
	KEY_I, 
	KEY_J, 
	KEY_K, 
	KEY_L, 
	KEY_M, 
	KEY_N, 
	KEY_O, 
	KEY_P, 
	KEY_Q, 
	KEY_R, 
	KEY_S, 
	KEY_T, 
	KEY_U, 
	KEY_V, 
	KEY_W, 
	KEY_X, 
	KEY_Y, 
	KEY_Z
};

class CInput
{
public:
	CInput();
	~CInput();
	
	void Init();
	void Shutdown();
	
	void Update();
	
	void OnKeyAction(int key, bool action);
	
	bool IsPressed(char key) { return keys[key]; }
	void GetMousePos(int* pmouseX, int* pmouseY);
	
	void Reset();

private:
	bool keys[460];
	int mouseX, mouseY;
};

extern CInput* input;

#endif
