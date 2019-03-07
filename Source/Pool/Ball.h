#pragma once

#include <string>

#include <include/glm.h>
#include <Core/GPU/Mesh.h>
#include <Core/Engine.h>
#include "Player.h"

using namespace std;

class Ball
{
public:
	float x, z, radius, speed = 0.0f, angle = 0, delay = 0.0f, vertical, horizontal, collissions = 0;
	bool visible = true, frameMove = false;
	string type;
	Ball(float x, float z, float radius, string type);
	void Collission(float top, float bottom, float left_border, float right_border);
	void BallsCollission(Ball* ball, float deltaTimeSeconds, int c, Ball* balls[16], float top, float bottom, float left_border, float right_border, Player* current);
	bool checkCollission(Ball* ball1, Ball* ball2);
	void Move(float deltaTimeSeconds);
	void Friction();
	void inHole(float top, float bottom, float left_border, float right_border, Player* current, Player* other);
	void MoveBall(Ball* balls[16], float deltaTimeSeconds, float top, float bottom, float left_border, float right_border, Player* current, Player* other);
	~Ball();
};

