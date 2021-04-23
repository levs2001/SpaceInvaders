#include <Windows.h>
#include <memory>
#include "Drawing.h"
#include "Game.h"

char szClassName[] = "Window1";
HWND hWnd;

LRESULT CALLBACK WndProc(HWND, UINT, UINT, LONG);
ATOM registerMyClass(HINSTANCE hInstance);
int createMyWindow(HINSTANCE hInstance, int nCmdShow);

SpInvaders* game;

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpszCmdParam, _In_ int nCmdShow)
{
	MSG msg;
	game = new SpInvaders();
	game->Init();

	createMyWindow(hInstance, nCmdShow);

	while (GetMessage(&msg, 0, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_CREATE: {
		SetTimer(hWnd, ID_TIMER_DRAWING, PERIOD_TIMER_DRAWING, NULL);
		SetTimer(hWnd, ID_TIMER_UPDATE, PERIOD_TIMER_UPDATE, NULL);
		break;
	}
	case WM_TIMER: {
		switch (wParam) {
		case ID_TIMER_UPDATE:
			game->Update();
			break;
		case ID_TIMER_DRAWING:
			InvalidateRect(hWnd, NULL, TRUE);
			break;
		}
		break;
	}
	case WM_DESTROY: {
		KillTimer(hWnd, ID_TIMER_UPDATE);
		KillTimer(hWnd, ID_TIMER_DRAWING);
		PostQuitMessage(0);
		break;
	}
	case WM_KEYDOWN: {
		switch (wParam) {
		case VK_UP:
			break;
		case VK_DOWN:
			break;
		case VK_RIGHT:
			game->MoveHero(HERO_SPEED);
			break;
		case VK_LEFT:
			game->MoveHero(-HERO_SPEED);
			break;
		case VK_SPACE:
			game->HeroShot();
			break;
		}
		break;
	}
	case WM_PAINT:	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		//Rectangle(hdc, 0, 0, WINDOW_MAX_X, WINDOW_MAX_Y);
		game->Draw(hdc);
		EndPaint(hWnd, &ps);
	}
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

int createMyWindow(HINSTANCE hInstance, int nCmdShow) {
	registerMyClass(hInstance);

	hWnd = CreateWindow(szClassName, "SpaceInvaders", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, SCREEN_WIDTH, SCREEN_HEIGHT, NULL, NULL, hInstance, NULL);

	if (!hWnd) { return 0; }
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);


}

ATOM registerMyClass(HINSTANCE hInstance)
{
	WNDCLASS wc;
	wc.cbClsExtra = wc.cbWndExtra = 0;
	wc.style = 0;
	wc.lpfnWndProc = WndProc;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = szClassName;

	return RegisterClass(&wc);
}