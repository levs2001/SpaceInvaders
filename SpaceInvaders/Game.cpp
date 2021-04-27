#include "Game.h"
#include "Drawing.h"
#include <random>
#include "TextSystem/TextSystem.h"

template<typename T>
static void DeleteFromVector(std::vector<T>& vec, const T& itemDel);
static inline bool IsOut(ClassXY point);
static inline bool CheckCapture(ClassXY coordCenter);

void SpInvaders::Init() {
	size_t lifes = 3;
	lvlNum = 1;
	hero = new Hero(lifes, ClassXY(FIELD_MAX_X / 2, FIELD_MAX_Y - HERO_DEFAULT_SIZE_Y / 2), ClassXY(HERO_DEFAULT_SIZE_X, HERO_DEFAULT_SIZE_Y));
	gameMenu = new GameMenu;
	gameMenu->Init(this);
}

void SpInvaders::Start() {
	speed = DEFAULT_ALIEN_SPEED;
	timeToShot = 0;
	timeToMove = 0;
	gameCond = EGameCond::NOT_PLAYED;
	InitLevels(FILE_LEVELS);
	hero->SetNewHero();
}

void SpInvaders::Update() {
	if (CheckCapture(activeLevel->rows.back().GetAliens().back().GetCoord()))
		LoseGame();
	CheckShooting();

	timeToMove++;
	MoveObjects();

	timeToShot++;
	if (!IsLost() && timeToShot > GetAlienShotFrequency()) {
		AlienShot();
		timeToShot = 0;
	}
	if (activeLevel->GetAliensCount() == 0)
		PassLevel();
}

size_t SpInvaders::GetAlienShotFrequency() {
	if (activeLevel->GetAliensCount() < activeLevel->timesToShot.front().aliensC)
		activeLevel->timesToShot.pop_front();
	return activeLevel->timesToShot.front().time;
}

size_t SpInvaders::GetAlienMoveFrequency() {
	if (!IsLost()) {
		if (activeLevel->GetAliensCount() < activeLevel->timesToMove.front().aliensC)
			activeLevel->timesToMove.pop_front();
		return activeLevel->timesToMove.front().time;
	}
	else
		return ALIEN_MOVE_FREQUENCY_IF_LOST;
}

void SpInvaders::MoveObjects() {
	if (timeToMove> GetAlienMoveFrequency()) {
		for (Row& row : activeLevel->rows) {
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
		timeToMove = 0;
	}

	for (ClassXY& aShot : alienShots) {
		if (!IsOut(aShot))
			aShot.y += ALIEN_SHOT_SPEED;
		else
			DeleteFromVector(alienShots, aShot);
	}
	for (ClassXY& heroShot : heroShots) {
		if (!IsOut(heroShot)) 
			heroShot.y -= HERO_SHOT_SPEED;
		else
			DeleteFromVector(heroShots, heroShot);
	}
}

void SpInvaders::CheckShooting() {
	for (const ClassXY& aShot : alienShots)
		if (hero->CheckHit(aShot)) {
			hero->MinLife();
			DeleteFromVector(alienShots, aShot);
			if (hero->IsDied()) {
				LoseGame();
			}
		}
	for (ClassXY& heroShot : heroShots) {
		for (Row& row : activeLevel->rows) {
			if (heroShot.y > row.GetYcoord() - row.GetYsize() / 2 && heroShot.y < row.GetYcoord() + row.GetYsize() / 2) {
				for (Alien& alien : row.GetAliens())
					if (alien.CheckHit(heroShot)) {
						row.KillAlien(alien);
						hero->PlusPoints(10);
						DeleteFromVector(heroShots, heroShot);
						break;
					}

				if (row.GetAliens().size() == 0)
					DeleteFromVector(activeLevel->rows, row);
			}
		}
	}
}

void SpInvaders::DownRows(int y) {
	for (Row& row : activeLevel->rows)
		row.Move(0, y);
}

void SpInvaders::HeroShot() {
	ClassXY shot;
	heroShots.push_back(shot);
	hero->Shot(heroShots.back());
}

void SpInvaders::AlienShot() {
	std::random_device rd;  
	std::mt19937 gen(rd()); 

	std::uniform_int_distribution<> randRow(0, activeLevel->rows.size() - 1);
	size_t rowN = randRow(gen);
	size_t alienC = activeLevel->rows[rowN].GetAliens().size();
	std::uniform_int_distribution<> randAlien(0, alienC - 1);

	ClassXY shot;
	alienShots.push_back(shot);
	activeLevel->rows[rowN].GetAliens()[randAlien(gen)].Shot(alienShots.back());
}

void SpInvaders::InitLevels(std::string filename) {
	SetLevelsFromFile(levels, filename);
	activeLevel = &levels.front();
}

void SpInvaders::PassLevel() {
	alienShots.clear();
	heroShots.clear();
	levels.pop_front();
	if (levels.size()!=0) {
		activeLevel = &levels.front();
		lvlNum++;
		hero->PlusLifes(1);
	}
	else {
		gameCond = EGameCond::WON;
		EndGame();
	}
}

void SpInvaders::ClearLevels() {
	levels.clear();
	alienShots.clear();
}

void SpInvaders::EndGame() {
	ClearLevels();
	gameMenu->SetActive();
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


Row::Row(int yCentr, size_t xSize, size_t ySize) :yCentr(yCentr), ySize(ySize) {
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

size_t Row::GetAliensCount() const {
	return aliens.size();
}

size_t Level::GetAliensCount() const{
	size_t aCount = 0;
	for (const Row& row : rows) {
		aCount += row.GetAliensCount();
	}
	return aCount;
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

template<typename T>
static void DeleteFromVector(std::vector<T>& vec, const T& itemDel) {
	auto it = std::find(vec.begin(), vec.end(), itemDel);
	if (it != vec.end())
		vec.erase(it);
}

static inline bool IsOut(ClassXY point) {
	if (point.x<FIELD_MIN_X || point.x>FIELD_MAX_X || point.y<FIELD_MIN_Y || point.y>FIELD_MAX_Y)
		return true;
	return false;
}

static inline bool CheckCapture(ClassXY coordCenter) {
	if (coordCenter.y + ALIEN_DEFAULT_SIZE / 2 > FIELD_CAPTURE_Y)
		return true;
	return false;
}