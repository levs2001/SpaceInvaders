#include <Windows.h>
#include "Game.h"
#include "Drawing.h"

#define LINE_WIDTH 7 
#define PINK RGB(255, 100, 200) 
#define PATRON_SIZE 20 

static void DrawSquare(HDC hdc, ClassXY centr, ClassXY size, COLORREF color) {
	int left = centr.x - size.x / 2;
	int top = centr.y - size.y / 2;
	int right = centr.x + size.x / 2;
	int bottom = centr.y + size.y / 2;

	HBRUSH hBrush = CreateSolidBrush(color);
	SelectObject(hdc, hBrush);
	Rectangle(hdc, left, top, right, bottom);
	DeleteObject(hBrush);
}

static void DrawLine(HDC hdc, COLORREF color, int x1, int y1, int x2, int y2) {
	HPEN hPen = CreatePen(PS_SOLID, LINE_WIDTH, color); //Создаётся объект
	SelectObject(hdc, hPen); //Объект делается текущим

	MoveToEx(hdc, x1, y1, NULL); //сделать текущими координаты x1, y1
	LineTo(hdc, x2, y2);

	DeleteObject(hPen);
}

static void DrawPatron(const ClassXY patron, HDC hdc) {
	DrawLine(hdc, PINK, patron.x, patron.y - PATRON_SIZE / 2, patron.x, patron.y + PATRON_SIZE / 2);
}

void SpInvaders::Draw(HDC hdc) const {
	hero->Draw(hdc);
	for (const Row& row : rows)
		row.Draw(hdc);
	for (const ClassXY& aShot : alienShots)
		DrawPatron(aShot, hdc);
	DrawPatron(heroShot, hdc);
}

void Row::Draw(HDC hdc) const {
	for (const Alien& alien : aliens)
		alien.Draw(hdc);
}

void Alien::Draw(HDC hdc) const {
	DrawSquare(hdc, coord, size, RGB(0, 0, 255));
}

void Hero::Draw(HDC hdc) const {
	DrawSquare(hdc, coord, size, RGB(255, 0, 0));
}



