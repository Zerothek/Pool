#include "Player.h"

#include <Core/Engine.h>

using namespace std;

Player::Player(int potted_balls, bool change, string type, int number) {
	this->potted_balls = potted_balls;
	this->change = change;
	this->type = type;
	this->number = number;
}

