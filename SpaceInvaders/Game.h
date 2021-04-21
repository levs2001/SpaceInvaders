#pragma once

#include <Vector>

class ClassXY {
public:
	int x;
	int y;
	ClassXY(int x, int y) :x(x), y(y) {}
	ClassXY(const ClassXY& size) :x(size.x), y(size.y) {};
	ClassXY() : x(-1), y(1) {};
	friend bool operator== (const ClassXY& c1, const ClassXY& c2) {
		return (c1.x == c2.x && c1.y == c2.y);
	}
};

class Entity {
public:
	virtual void Draw() const = 0;
	virtual void Die() = 0;

	friend bool operator== (const Entity& a1, const Entity& a2) {
		return (a1.coord == a2.coord);
	}

	void Move(int x, int y = 0) {
		coord.x += x;
		coord.y += y;
	}

	ClassXY GetCoord() {
		return coord;
	}

	bool CheckHit(ClassXY patron) const;
private:
	
	ClassXY coord;
	ClassXY size;
};


class Hero : public Entity {
public:
	virtual void Draw() const override;
	virtual void Die() override;
	void MinLife();

private:
	size_t lifes;
};

class Alien : public Entity {
public:
	virtual void Draw() const override;
	virtual void Die() override;

private:
};

class Row {
public:
	void Draw() const;
	void Move(int x, int y = 0);
	void KillAlien(Alien& alien);

	~Row() {
		aliens.clear();
	}

	int GetYcoord() const {
		return yCoord;
	}

	size_t GetYsize() const {
		return ySize;
	}
	
	std::vector<Alien>& GetAliens() {
		return aliens;
	}

private:
	std::vector<Alien> aliens;
	int yCoord;
	size_t ySize;
};

class SpInvaders {
public:
	void Init();
	void Update();
	void Draw() const;

	~SpInvaders() {
		rows.clear();
		delete hero;
	}
private:
	void CheckShooting();
	void MoveObjects();

	Hero* hero;
	std::vector<Row> rows;
	int speed;
	ClassXY alienPatron;
	ClassXY heroPatron;
};

void DrawPatron(ClassXY patron);

