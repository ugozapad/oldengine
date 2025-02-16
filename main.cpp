/**************************
 * Includes
 *
 **************************/
 
#include <stdio.h>
#include <windows.h>
#include <gl/gl.h>
#include "cinput.h"
#include "crenderer.h"
#include "ctimer.h"
#include "cgame.h"

/**************************
 * Function Declarations
 *
 **************************/

LRESULT CALLBACK WndProc (HWND hWnd, UINT message,WPARAM wParam, LPARAM lParam);
void InitConsole();
void LoadSettings(int* width, int* height);

/**************************
 * WinMain
 *
 **************************/

int WINAPI WinMain (HINSTANCE hInstance,
                    HINSTANCE hPrevInstance,
                    LPSTR lpCmdLine,
                    int iCmdShow)
{
    WNDCLASS wc;
    HWND hWnd;  
    MSG msg;
    BOOL bQuit = FALSE;
	int width = 800;
	int height = 600;
	
	/* Open console */
	InitConsole();
	
	/* Load settings */
	LoadSettings(&width, &height);

    /* register window class */
    wc.style = CS_OWNDC;
    wc.lpfnWndProc = WndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = LoadIcon (NULL, IDI_APPLICATION);
    wc.hCursor = LoadCursor (NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH) GetStockObject (BLACK_BRUSH);
    wc.lpszMenuName = NULL;
    wc.lpszClassName = "GLSample";
    RegisterClass (&wc);

    /* create main window */
    hWnd = CreateWindow (
      "GLSample", "Game", 
      WS_CAPTION | WS_POPUPWINDOW | WS_VISIBLE,
      0, 0, width, height,
      NULL, NULL, hInstance, NULL);

	printf("Starting rendering device at window %ix%i\n", width, height);

	/* Start input device */
	input = new CInput();
	input->Init();

    /* Start renderer */
    renderer = new CRenderer();
    renderer->Init(hWnd);
	
	// #TODO: Shit...
	renderer->SetViewport(0, 0, width, height);

	/* Start game */
	game = new CGame();
	game->Init();

    /* program main loop */
    while (!bQuit)
    {
        /* check for messages */
        if (PeekMessage (&msg, NULL, 0, 0, PM_REMOVE))
        {
            /* handle or dispatch messages */
            if (msg.message == WM_QUIT)
            {
                bQuit = TRUE;
            }
            else
            {
                TranslateMessage (&msg);
                DispatchMessage (&msg);
            }
        }
        else
        {
        	/* Input Device and Game update goes here */
        	
			timer.Update();

			input->Update();
        	
			game->Update();

            /* OpenGL animation code goes here */

			renderer->SetViewport(0, 0, width, height);
			
			renderer->BeginFrame();
			
			game->Render();

			renderer->EndFrame();

            renderer->Present();
            
            Sleep (1);
        }
    }

	/* shutdown game */
	game->Shutdown();
	delete game; game = NULL;

    /* shutdown renderer */
    renderer->Shutdown();
    delete renderer; renderer = NULL;
    
    /* destroy input */
	input->Shutdown();
	delete input; input = NULL;

    /* destroy the window explicitly */
    DestroyWindow (hWnd);

    return msg.wParam;
}


/********************
 * Window Procedure
 *
 ********************/

LRESULT CALLBACK WndProc (HWND hWnd, UINT message,
                          WPARAM wParam, LPARAM lParam)
{

    switch (message)
    {
    case WM_CREATE:
        return 0;
    case WM_DESTROY:
        PostQuitMessage (0);
        return 0;


    case WM_KEYDOWN:
        switch (wParam)
        {
        case VK_ESCAPE:
            PostQuitMessage(0);
            return 0;
		}
		
		input->OnKeyAction((int)wParam, true);
		
        return 0;

	case WM_KEYUP:
		input->OnKeyAction((int)wParam, false);
		
		return 0;

    default:
        return DefWindowProc (hWnd, message, wParam, lParam);
    }
}

/********************
 * Console Initialization
 *
 ********************/
 
void InitConsole()
{
	if (AllocConsole())
	{
        freopen("CONOUT$", "w", stdout);
        SetConsoleTitle("Console output");
        //SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED);  
    }
}

/********************
 * Settings Loading
 *
 ********************/

void LoadSettings(int* width, int* height)
{
	static char buf[256];
	FILE* f = fopen("data/settings.txt", "r");
	if (f)
	{
		while (feof(f) != EOF)
		{
			int ret = fscanf(f, "%s", buf);
			if (ret == EOF)
				break;
				
			if (strcmp(buf, "width") == 0)
			{
				int w = 800;
				fscanf(f, "%i", &w);
				*width = w;
			}
			else if (strcmp(buf, "height") == 0)
			{
				int h = 800;
				fscanf(f, "%i", &h);
				*height = h;
			}
		}
		
		fclose(f);
	}
}

