#include "Game.h"
#include "Drawing.h"
#include <random>

template<typename T>
static void DeleteFromVector(std::vector<T>& vec, const T& itemDel) {
	auto it = std::find(vec.begin(), vec.end(), itemDel);
	if (it != vec.end())
		vec.erase(it);
}

void SpInvaders::Init() {
	size_t lifes = 3;
	hero = new Hero(lifes, ClassXY(FIELD_MAX_X / 2, FIELD_MAX_Y - HERO_DEFAULT_SIZE_Y / 2), ClassXY(HERO_DEFAULT_SIZE_X, HERO_DEFAULT_SIZE_Y));
	gameMenu = new GameMenu;
	gameMenu->Init(this);
}

void SpInvaders::Start() {
	speed = DEFAULT_ALIEN_SPEED;
	for (size_t i = 0; i != ROW_COUNT; i++) {
		Row row((i + 1) * ALIEN_DEFAULT_SIZE / 2 + i * ALIEN_DIST_Y + FIELD_MIN_Y, ALIEN_DEFAULT_SIZE, ALIEN_DEFAULT_SIZE);
		rows.push_back(row);
	}
	heroShot = ClassXY();
	timeToShot = 0;
	hero->SetNewHero();
}

void SpInvaders::ClearAliens() {
	rows.clear();
	alienShots.clear();
}

void GameMenu::Init(SpInvaders* host) {
	activity = true;
	size.x = 200;
	size.y = 100;
	stLocation.x = FIELD_MAX_X / 2;
	stLocation.y = FIELD_MAX_Y / 2;
	buttonsCount = 2;
	activeButton = 0;
	this->host = host;
}

void GameMenu::KeyUp() {
	if (activeButton > 0)
		activeButton--;
}

void GameMenu::KeyDown() {
	if (activeButton < buttonsCount - 1)
		activeButton++;
}

void GameMenu::KeyPress(HWND hWnd) {
	if (activeButton == 0) {
		activity = false;
		host->Start();
	}
	else
		DestroyWindow(hWnd);
}

void SpInvaders::Update() {
	if (CheckCapture(rows.back().GetAliens().back().GetCoord()))
		LoseGame();
	CheckShooting();
	MoveObjects();

	timeToShot++;
	if (timeToShot > ALIEN_SHOT_FREQUENCY) {
		AlienShot();
		timeToShot = 0;
	}
}

void SpInvaders::MoveObjects() {
	for (Row& row : rows) {
		const Alien& firstAlien = speed > 0 ? row.GetAliens().back() : row.GetAliens().front();
		if (!IsLost()) {
			if (firstAlien.GetCoord().x > FIELD_MAX_X - firstAlien.GetSize().x + firstAlien.GetSize().x / 2 || firstAlien.GetCoord().x - firstAlien.GetSize().x / 2 < FIELD_MIN_X) {
				speed = -speed;
				DownRows(ALIEN_DEFAULT_SIZE);
			}
			row.Move(speed);
		}
		else if (!IsOut(firstAlien.GetCoord())) {
			DownRows(ALIEN_DEFAULT_SIZE);
			break;
		}
		else {
			EndGame();
			break;
		}
	}

	for (ClassXY& aShot : alienShots) {
		if (!IsOut(aShot))
			aShot.y += ALIEN_SHOT_SPEED;
		else
			DeleteFromVector(alienShots, aShot);
	}

	if (!IsOut(heroShot)) {
		heroShot.y -= HERO_SHOT_SPEED;
	}
}

void SpInvaders::EndGame() {
	ClearAliens();
	gameMenu->SetActive();
}

void SpInvaders::CheckShooting() {
	for (const ClassXY& aShot : alienShots)
		if (hero->CheckHit(aShot)) {
			hero->MinLife();
			DeleteFromVector(alienShots, aShot);
			if (hero->IsDied()) {
				//ClearAliens();
				//gameMenu->SetActive();
				LoseGame();
			}
		}

	for (Row& row : rows) {
		if (heroShot.y > row.GetYcoord() - row.GetYsize() / 2 && heroShot.y < row.GetYcoord() + row.GetYsize() / 2) {
			for (Alien& alien : row.GetAliens())
				if (alien.CheckHit(heroShot)) {
					row.KillAlien(alien);
					hero->PlusPoints(10);
					heroShot = ClassXY(OUT, OUT);
				}

			if (row.GetAliens().size() == 0)
				DeleteFromVector(rows, row);
		}
	}
}

void SpInvaders::DownRows(int y) {
	for (Row& row : rows)
		row.Move(0, y);
}

void SpInvaders::HeroShot() {
	if (IsOut(heroShot))
		hero->Shot(heroShot);
}

void SpInvaders::AlienShot() {
	//«десь пушим в вектор и делаем шот
	std::random_device rd;  //Will be used to obtain a seed for the random number engine
	std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()

	std::uniform_int_distribution<> randRow(0, rows.size() - 1);
	size_t rowN = randRow(gen);
	size_t alienC = rows[rowN].GetAliens().size();
	std::uniform_int_distribution<> randAlien(0, alienC - 1);

	ClassXY shot;
	alienShots.push_back(shot);
	rows[rowN].GetAliens()[randAlien(gen)].Shot(alienShots.back());
}

Row::Row(int yCentr, size_t ySize, size_t xSize) :yCentr(yCentr), ySize(ySize) {
	for (size_t i = 0; i < ALIEN_ROW_COUNT; i++) {
		ClassXY size(xSize, ySize);
		ClassXY coord((i + 1) * size.x + i * ALIEN_DIST_X, yCentr);
		aliens.push_back(Alien(coord, size));
	}
}

void Row::Move(int x, int y) {
	yCentr += y;
	for (Alien& alien : aliens)
		alien.Move(x, y);
}

void Row::KillAlien(Alien& alien) {
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

bool Alien::IsDied() {
	return true;
}

void Hero::MinLife() {
	lifes--;
}

bool Hero::IsDied() {
	if (lifes == 0)
		return true;
	return false;
}

void Hero::SetNewHero() {
	lifes = 3;
	points = 0;
}

inline bool IsOut(ClassXY point) {
	if (point.x<FIELD_MIN_X || point.x>FIELD_MAX_X || point.y<FIELD_MIN_Y || point.y>FIELD_MAX_Y)
		return true;
	return false;
}

inline bool CheckCapture(ClassXY coordCenter) {
	if (coordCenter.y + ALIEN_DEFAULT_SIZE / 2 > FIELD_CAPTURE_Y)
		return true;
	return false;
}