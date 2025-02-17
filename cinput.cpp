#include <stdio.h>
#include <string.h>
#include <windows.h>
#include "cinput.h"

CInput* input = NULL;

CInput::CInput()
{
	memset(keys, 0, sizeof(keys));
}

CInput::~CInput()
{
}

void CInput::Init()
{
}

void CInput::Shutdown()
{
}

void CInput::Update()
{
	POINT pt;
	GetCursorPos(&pt);
	mouseX = (int)pt.x;
	mouseY = (int)pt.y;

	ShowCursor(0);
}

void CInput::OnKeyAction(int key, bool action)
{
	keys[key] = action;
}

void CInput::GetMousePos(int* pmouseX, int* pmouseY)
{
	if (pmouseX) *pmouseX=mouseX;
	if (pmouseY) *pmouseY=mouseY;
}
