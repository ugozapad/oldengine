#ifndef CINPUT_H
#define CINPUT_H

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
	
private:
	bool keys[460];
	int mouseX, mouseY;
};

extern CInput* input;

#endif
