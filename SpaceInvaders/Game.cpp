#include "Game.h"

template<typename T>
static void DeleteFromVector(std::vector<T> vec, T& itemDel) {

	auto it = std::find(vec.begin(), vec.end(), itemDel);
	if (it != vec.end())
		vec.erase(it);
}

void SpInvaders::Init() {

}

void SpInvaders::Draw() const {
	hero->Draw();
	for (const Row& row : rows)
		row.Draw();

	DrawPatron(alienPatron);
	DrawPatron(heroPatron);
}

void SpInvaders::Update() {

}

void SpInvaders::MoveObjects() {
	for (Row& row : rows) {
		//Дописать для y и сталкивание в  стену
		row.Move(speed);
	}
}

void SpInvaders::CheckShooting() {
	if (hero->CheckHit(alienPatron))
		hero->MinLife();

	for (Row& row : rows) {
		if (heroPatron.y > row.GetYcoord() - row.GetYsize()/2 && heroPatron.y < row.GetYcoord() + row.GetYsize()/2) {
			for (Alien& alien : row.GetAliens())
				if (alien.CheckHit(heroPatron)) {
					row.KillAlien(alien);
				}
		}
	}
}

void Row::Move(int x, int y) {
	for (Alien& alien : aliens)
		alien.Move(x, y);
}

void Row::KillAlien(Alien& alien) {
	alien.Die();
	DeleteFromVector(aliens, alien);
}

void Row::Draw() const {

}

bool Entity::CheckHit(ClassXY patron) const{
	if (patron.x > coord.x - size.x / 2 && patron.x < coord.x + size.x / 2)
		if (patron.y > coord.y - size.y / 2 && patron.y < coord.y + size.y / 2)
			return true;
	return false;
}

void Alien::Draw() const {

}

void Alien::Die() {

}

void Hero::Draw() const {

}

void Hero::MinLife() {
	lifes--;
	if (lifes == 0)
		Die();
}

void Hero::Die() {

}

void DrawPatron(ClassXY patron) {

}