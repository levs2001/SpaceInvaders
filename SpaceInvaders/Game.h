#pragma once

#include <Vector>
#include "Drawing.h"
#include <Windows.h>
#include <string>


#define ALIEN_ROW_COUNT 11
#define ALIEN_DEFAULT_SIZE 40
#define ALIEN_DIST_X 50
#define ALIEN_DIST_Y 100
#define ALIEN_SHOT_FREQUENCY 30
#define ROW_COUNT 5
#define HERO_DEFAULT_SIZE_X 40
#define HERO_DEFAULT_SIZE_Y 40
#define OUT -10
#define HERO_SPEED 10
#define DEFAULT_ALIEN_SPEED 1

#define HERO_SHOT_SPEED 30
#define ALIEN_SHOT_SPEED 30

#define ID_TIMER_DRAWING 1
#define ID_TIMER_UPDATE 2
#define PERIOD_TIMER_DRAWING 50
#define PERIOD_TIMER_UPDATE 50


class ClassXY {
public:
	int x;
	int y;
	ClassXY(int x, int y) :x(x), y(y) {}
	ClassXY(const ClassXY& size) :x(size.x), y(size.y) {};
	ClassXY() : x(OUT), y(OUT) {};
	friend bool operator== (const ClassXY& c1, const ClassXY& c2) {
		return (c1.x == c2.x && c1.y == c2.y);
	}
};

class Entity {
public:
	Entity(ClassXY coord, ClassXY size) : coord(coord), size(size){}
	virtual void Draw(HDC hdc) const = 0;
	virtual bool IsDied() = 0;

	friend bool operator== (const Entity& a1, const Entity& a2) {
		return (a1.coord == a2.coord);
	}

	void Move(int x, int y = 0) {
		coord.x += x;
		coord.y += y;
	}
	
	void Shot(ClassXY& shot);
	
	ClassXY GetCoord() const{
		return coord;
	}
	
	ClassXY GetSize() const {
		return size;
	}

	bool CheckHit(ClassXY shot) const;

protected:
	ClassXY coord;
	ClassXY size;
};


class Hero : public Entity {
public:
	Hero(size_t lifes, ClassXY coord, ClassXY size, size_t points = 0) : Entity(coord, size), lifes(lifes), points(points) {
		
	}

	virtual void Draw(HDC hdc) const override;
	virtual bool IsDied() override;
	void MinLife();
	void SetNewHero();
	void PlusPoints(size_t plPoints) {
		points += plPoints;
	}

	size_t GetLifes() {
		return lifes;
	}

	size_t GetPoints() {
		return points;
	}

private:
	size_t lifes;
	size_t points;
};

class Alien : public Entity {
public:
	Alien(ClassXY coord, ClassXY size) : Entity(coord, size){

	}
	virtual void Draw(HDC hdc) const override;
	virtual bool IsDied() override;
	//virtual void Shot(ClassXY& shot) override;

private:
};

class Row {
public:
	Row(int yCentr, size_t ySize, size_t xSize);
	void Draw(HDC hdc) const;
	void Move(int x, int y = 0);
	void KillAlien(Alien& alien);

	friend bool operator== (const Row& r1, const Row& r2) {
		return (r1.yCentr == r2.yCentr);
	}

	~Row() {
		aliens.clear();
	}

	int GetYcoord() const {
		return yCentr;
	}

	size_t GetYsize() const {
		return ySize;
	}
	
	std::vector<Alien>& GetAliens() {
		return aliens;
	}

private:
	std::vector<Alien> aliens;
	int yCentr;
	size_t ySize;
};

class SpInvaders;

class GameMenu {
public:
	void Init(SpInvaders* host);
	void Draw(HDC hdc);
	void KeyUp();
	void KeyDown();
	void KeyPress(HWND hWnd);
	void SetActive() {
		activity = true;
	}
	bool IsActive() const {
		return activity;
	}

private:
	size_t buttonsCount;
	ClassXY size;
	ClassXY stLocation;
	size_t activeButton;
	bool activity;
	SpInvaders* host;
};

class SpInvaders {
public:
	void Init();
	void Start();
	void Update();
	void Draw(HDC hdc) const;
	void HeroShot();
	void AlienShot();
	void DrawPointsLifes(HDC hdc, ClassXY position) const;
	void EndGame();
	void MoveHero(int x) {
		hero->Move(x);
	}
	
	bool IsLost() const{
		return gameLost;
	}
	
	void LoseGame() {
		gameLost = true;
	}
	
	GameMenu& Menu() {
		return *gameMenu;
	}
	
	~SpInvaders() {
		ClearAliens();
		delete hero;
		delete gameMenu;
	}


private:
	void CheckShooting();
	void MoveObjects();
	void DownRows(int y);
	void ClearAliens();

	GameMenu* gameMenu;
	Hero* hero;
	std::vector<Row> rows;
	int speed;
	size_t timeToShot;
	std::vector<ClassXY> alienShots;
	ClassXY heroShot;
	bool gameLost;
};

inline bool IsOut(ClassXY point);
inline bool CheckCapture(ClassXY coordCenter);