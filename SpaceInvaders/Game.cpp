#include "Game.h"
#include "Drawing.h"

template<typename T>
static void DeleteFromVector(std::vector<T>& vec, T& itemDel) {

	auto it = std::find(vec.begin(), vec.end(), itemDel);
	if (it != vec.end())
		vec.erase(it);
}

void SpInvaders::Init() {
	size_t lifes = 3;
	hero = new Hero(lifes, ClassXY(WINDOW_MAX_X/2, WINDOW_MAX_Y - HERO_DEFAULT_SIZE_Y/2), ClassXY(HERO_DEFAULT_SIZE_X, HERO_DEFAULT_SIZE_Y));
	speed = DEFAULT_ALIEN_SPEED;
	for (size_t i = 0; i != ROW_COUNT; i++) {
		Row row((i+1) * ALIEN_DEFAULT_SIZE / 2 + i * ALIEN_DIST_Y, ALIEN_DEFAULT_SIZE, ALIEN_DEFAULT_SIZE);
		rows.push_back(row);
	}
	heroShot = ClassXY();
}


void SpInvaders::Update() {
	CheckShooting();
	MoveObjects();
}

void SpInvaders::MoveObjects() {
	for (Row& row : rows) {
		const Alien& firstAlien = speed > 0 ? row.GetAliens().back() : row.GetAliens().front();
		if (firstAlien.GetCoord().x > WINDOW_MAX_X - firstAlien.GetSize().x + firstAlien.GetSize().x / 2 || firstAlien.GetCoord().x - firstAlien.GetSize().x / 2 < 0) {
			speed = -speed;
			DownRows(ALIEN_DEFAULT_SIZE);
		}
		row.Move(speed);
	}
	
	if (heroShot.x != OUT && heroShot.y) {
		heroShot.y -= SHOT_SPEED;
	}
}

void SpInvaders::CheckShooting() {
	for (const ClassXY& aShot : alienShots)
		if (hero->CheckHit(aShot))
			hero->MinLife();

	for (Row& row : rows) {
		if (heroShot.y > row.GetYcoord() - row.GetYsize() / 2 && heroShot.y < row.GetYcoord() + row.GetYsize() / 2) {
			for (Alien& alien : row.GetAliens())
				if (alien.CheckHit(heroShot)) {
					row.KillAlien(alien);
				}
		}
	}
}

void SpInvaders::DownRows(int y) {
	for (Row& row : rows) 
		row.Move(0, y);
}

void SpInvaders::HeroShot() {
	hero->Shot(heroShot);
}

void SpInvaders::AlienShot() {
	//«десь пушим в вектор и делаем шот
}

Row::Row(int yCentr, size_t ySize, size_t xSize) :yCentr(yCentr), ySize(ySize) {
	for (size_t i = 0; i < ALIEN_ROW_COUNT; i++) {
		ClassXY size(xSize, ySize);
		ClassXY coord((i+1)*size.x + i*ALIEN_DIST_X, yCentr);
		aliens.push_back(Alien(coord, size));
	}
}

void Row::Move(int x, int y) {
	yCentr += y;
	for (Alien& alien : aliens)
		alien.Move(x, y);
}

void Row::KillAlien(Alien& alien) {
	alien.Die();
	DeleteFromVector(aliens, alien);
}


bool Entity::CheckHit(ClassXY shot) const {
	if (shot.x > coord.x - size.x / 2 && shot.x < coord.x + size.x / 2)
		if (shot.y > coord.y - size.y / 2 && shot.y < coord.y + size.y / 2)
			return true;
	return false;
}

void Entity::Shot(ClassXY& shot) {
	shot.x = coord.x;
	shot.y = coord.y;
}
//void Alien::Shot(ClassXY& shot) {
//
//}

void Alien::Die() {

}


void Hero::MinLife() {
	lifes--;
	if (lifes == 0)
		Die();
}

//void Hero::Shot(ClassXY& shot) {
//
//}

void Hero::Die() {

}

