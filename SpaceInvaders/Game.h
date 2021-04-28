#pragma once

#include <Vector>
#include "Drawing.h"
#include <Windows.h>
#include <string>
#include <deque>

#define ALIEN_ROW_COUNT 11
#define ALIEN_DEFAULT_SIZE 40
#define ALIEN_DIST_X 50
#define ALIEN_DIST_Y 100
#define ALIEN_SHOT_FREQUENCY 30
#define ALIEN_MOVE_FREQUENCY_IF_LOST 1
#define ROW_COUNT 5
#define HERO_DEFAULT_SIZE_X 40
#define HERO_DEFAULT_SIZE_Y 40
#define OUT -10
#define HERO_SPEED 20
#define DEFAULT_ALIEN_SPEED 10

#define HERO_SHOT_SPEED 30
#define ALIEN_SHOT_SPEED 30

#define ID_TIMER_DRAWING 1
#define ID_TIMER_UPDATE 2
#define PERIOD_TIMER_DRAWING 50
#define PERIOD_TIMER_UPDATE 50

#define FILE_LEVELS "lvlTest.xml"

class Hero;
class Alien;
class Entity;
class GameMenu;
class Row;

struct TimeAliensC {
	size_t time;
	size_t aliensC;
};

enum class EGameCond;

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

struct Level {
	size_t GetAliensCount() const;
	std::vector<Row> rows;
	std::deque<TimeAliensC> timesToMove;
	std::deque<TimeAliensC> timesToShot;
};

class SpInvaders {
public:
	void Init();
	void Start();
	void Update();
	void Draw(HDC hdc) const;
	void HeroShot();
	void DrawPointsLifes(HDC hdc, ClassXY position) const;
	void MoveHero(int x);
	bool IsLost() const;
	bool IsWon() const;

	GameMenu& Menu() {
		return *gameMenu;
	}

	~SpInvaders() {
		ClearLevels();
		delete hero;
		delete gameMenu;
	}

private:
	void CheckShooting();
	void MoveObjects();
	void DownRows(int y);
	void AlienShot();
	void ClearLevels();
	void InitLevels(std::string filename);
	void EndGame();
	void PassLevel();
	void LoseGame();
	size_t GetAlienShotFrequency();
	size_t GetAlienMoveFrequency();
	
	GameMenu* gameMenu;
	Hero* hero;
	std::deque<Level> levels;
	Level* activeLevel;
	int speed;
	size_t timeToShot;
	size_t timeToMove;
	std::vector<ClassXY> alienShots;
	std::vector<ClassXY> heroShots;
	EGameCond gameCond;
	size_t lvlNum;
};

class Row {
public:
	Row(int yCentr, size_t xSize, size_t ySize);
	void Draw(HDC hdc) const;
	void Move(int x, int y = 0);
	void KillAlien(Alien& alien);
	bool CheckHit(ClassXY heroShot);
	size_t GetAliensCount() const;
	
	friend bool operator== (const Row& r1, const Row& r2) {
		return (r1.yCentr == r2.yCentr);
	}

	~Row() {
		aliens.clear();
	}

	/*int GetYcoord() const {
		return yCentr;
	}*/

	//size_t GetYsize() const {
	//	return ySize;
	//}

	std::vector<Alien>& GetAliens() {
		return aliens;
	}

private:
	std::vector<Alien> aliens;
	int yCentr;
	size_t ySize;
};

class Entity {
public:
	Entity(ClassXY coord, ClassXY size) : coord(coord), size(size) {}
	virtual void Draw(HDC hdc) const = 0;
	virtual bool IsDied() = 0;

	friend bool operator== (const Entity& a1, const Entity& a2) {
		return (a1.coord == a2.coord);
	}

	void Move(int x, int y = 0);

	void Shot(ClassXY& shot);

	ClassXY GetCoord() const {
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
	void PlusPoints(size_t points) {
		this->points += points;
	}

	void PlusLifes(size_t lifes) {
		this->lifes += lifes;
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
	Alien(ClassXY coord, ClassXY size) : Entity(coord, size) {

	}
	virtual void Draw(HDC hdc) const override;
	virtual bool IsDied() override;

private:
};

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

enum class EGameCond {
	NOT_PLAYED,
	LOST,
	WON
};
