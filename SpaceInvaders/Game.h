#pragma once

#include <Vector>
#include "Drawing.h"
#include <Windows.h>

#define ALIEN_ROW_COUNT 11
#define ALIEN_DEFAULT_SIZE 40
#define ALIEN_DIST_X 50
#define ALIEN_DIST_Y 100
#define ALIEN_SHOT_FREQUENCY 10
#define ROW_COUNT 5
#define HERO_DEFAULT_SIZE_X 40
#define HERO_DEFAULT_SIZE_Y 40
#define OUT -1
#define HERO_SPEED 10
#define DEFAULT_ALIEN_SPEED 3

#define SHOT_SPEED 10

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
	virtual void Die() = 0;

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
	virtual void Die() override;
	//virtual void Shot(ClassXY& shot) override;
	void MinLife();

private:
	size_t lifes;
	size_t points;
};

class Alien : public Entity {
public:
	Alien(ClassXY coord, ClassXY size) : Entity(coord, size){

	}
	virtual void Draw(HDC hdc) const override;
	virtual void Die() override;
	//virtual void Shot(ClassXY& shot) override;

private:
};

class Row {
public:
	Row(int yCentr, size_t ySize, size_t xSize);
	void Draw(HDC hdc) const;
	void Move(int x, int y = 0);
	void KillAlien(Alien& alien);

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

class SpInvaders {
public:
	void Init();
	void Update();
	void Draw(HDC hdc) const;
	void HeroShot();
	void AlienShot();
	void MoveHero(int x) {
		hero->Move(x);
	}

	~SpInvaders() {
		rows.clear();
		alienShots.clear();
		delete hero;
	}
private:
	void CheckShooting();
	void MoveObjects();
	void DownRows(int y);

	Hero* hero;
	std::vector<Row> rows;
	int speed;
	size_t timeToShot;
	std::vector<ClassXY> alienShots;
	ClassXY heroShot;
};

inline bool IsOut(ClassXY point);
