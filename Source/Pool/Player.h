#pragma once

#include <string>

#include <include/glm.h>
#include <Core/GPU/Mesh.h>
#include <Core/Engine.h>

using namespace std;

class Player
{
public:
	int potted_balls, number, fouls = 0;
	bool change, win = false;
	bool foul = false;
	string type;
	Player(int potted_balls, bool player1, string type, int number);
	~Player();
};

