#include <Windows.h>
#include <string>
#include "Game.h"
#include "Drawing.h"

#define PATRON_WIDTH 7 
#define CAPRURE_WIDTH 1 

#define PINK RGB(255, 100, 200) 
#define RED RGB(255, 0, 0)
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

static void DrawLine(HDC hdc, COLORREF color, int x1, int y1, int x2, int y2, int width = PATRON_WIDTH) {
	HPEN hPen = CreatePen(PS_SOLID, width, color);
	SelectObject(hdc, hPen);

	MoveToEx(hdc, x1, y1, NULL);
	LineTo(hdc, x2, y2);

	DeleteObject(hPen);
}

static void DrawPatron(const ClassXY patron, HDC hdc) {
	DrawLine(hdc, PINK, patron.x, patron.y - PATRON_SIZE / 2, patron.x, patron.y + PATRON_SIZE / 2);
}

void SpInvaders::Draw(HDC hdc) const {
	if (!IsLost())
		hero->Draw(hdc);
	for (const Row& row : activeLevel->rows)
		row.Draw(hdc);
	for (const ClassXY& aShot : alienShots)
		DrawPatron(aShot, hdc);
	for (const ClassXY& heroShot : heroShots)
		DrawPatron(heroShot, hdc);
	DrawLine(hdc, RED, FIELD_MIN_X, FIELD_CAPTURE_Y, FIELD_MAX_X, FIELD_CAPTURE_Y, CAPRURE_WIDTH);
}

void SpInvaders::DrawPointsLifes(HDC hdc, ClassXY position) const {
	SetTextColor(hdc, PINK);
	std::string points = std::to_string(hero->GetPoints());
	std::string lifes = std::to_string(hero->GetLifes());
	std::string lvl = std::to_string(lvlNum);
	std::string allText = "Points: " + points + "    " + "Lifes: " + lifes + "    " + "level: " + lvl;
	TextOut(hdc, position.x, position.y, allText.c_str(), allText.size());
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

void BigText(HDC hdc, std::string text) {
	ClassXY size(400, 100);
	ClassXY position(FIELD_MAX_X / 2, FIELD_MAX_Y / 3);

	DrawSquare(hdc, position, size, PINK);
	TextOutA(hdc, position.x - size.x / 9, position.y, text.c_str(), text.size());
}


void GameMenu::Draw(HDC hdc) {
	COLORREF colB0, colB1;
	colB0 = activeButton == 0 ? PINK : RED;
	colB1 = activeButton == 1 ? PINK : RED;

	host->DrawPointsLifes(hdc, ClassXY(stLocation.x - size.x / 2, stLocation.y - size.y / 1.2));
	ClassXY secondButCentr = ClassXY(stLocation.x, stLocation.y + size.y);
	DrawSquare(hdc, stLocation, size, colB0);
	DrawSquare(hdc, secondButCentr, size, colB1);
	TextOutA(hdc, stLocation.x - size.x / 9, stLocation.y, "START", strlen("START"));
	TextOutA(hdc, secondButCentr.x - size.x / 12, secondButCentr.y, "END", strlen("END"));
	if (host->IsLost())
		BigText(hdc, "You lose!");

}

