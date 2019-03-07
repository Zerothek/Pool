#include "Ball.h"

#include <Core/Engine.h>
#include <vector>
#include <string>
#include <iostream>
#include <stdio.h>
#include "Pool.h"

using namespace std;

Ball::Ball(float x, float z, float radius, string type) {
	this->x = x;
	this->z = z;
	this->radius = radius;
	this->type = type;
	this->vertical = 1.0f;
	this->horizontal = 1.0f;
}

void Ball::Collission(float top, float bottom, float left_border, float right_border)
{
	/*check if the ball collided with the left, right, bottom or top border and adjust the vertical
	and horizontal variables which will be used to calculate the trajectory of the ball*/

	if (this->z - this->radius <= top && this->speed != 0) {
		this->z = top + this->radius;
		this->vertical *= -1;
	}
	if (this->z + this->radius >= bottom && this->speed != 0) {
		this->z = bottom - this->radius;
		this->vertical *= -1;
	}
	if (this->x + this->radius >= right_border && this->speed != 0) {
		this->x = right_border - this->radius;
		this->horizontal *= -1;
	}
	if (this->x - this->radius <= left_border && this->speed != 0) {
		this->x = left_border + this->radius;
		this->horizontal *= -1;
	}

}

/*this function is used to check if a ball collided with another ball and if so to calculate their speeds
and their tranjectories*/
void Ball::BallsCollission(Ball * ball, float deltaTimeSeconds, int c, Ball* balls[16], float top, float bottom, float left_border, float right_border, Player* current)
{
	if (ball->speed != 0) {
		float cat1 = abs(this->x - ball->x), cat2 = abs(this->z - ball->z), ip = 2 * this->radius, new_angle;
		if (cat1 * cat1 + cat2 * cat2 < ip * ip) {

			float error = 0.025;

			//Caz 1
			if (ball->x >= this->x && ball->z >= this->z) {
				this->angle = asin(cat1 / ip);
				this->horizontal = this->vertical = 1;
				if (this->speed == 0) {
					this->speed = 0.8f * ball->speed / c;
				}
				else {
					this->speed = 0.8f * ball->speed / c;
					this->angle = (this->angle + ball->angle) / 2;
				}
				if (this->angle > M_PI / 4.05f && this->angle < M_PI / 3.95f) {
					this->speed /= 4;
					ball->speed *= 4;
				}
				if (ball->x > this->x + error && ball->z > this->z + error) {
					ball->angle = this->angle - M_PI / 2;
				}
				ball->x = this->x + 2 * this->radius * sin(asin(cat1 / ip));
				ball->z = this->z + 2 * this->radius * cos(asin(cat1 / ip));
				this->frameMove = true;
			}

			//Caz 4
			if (ball->x < this->x && ball->z >= this->z) {
				this->angle = 2 * M_PI - asin(cat1 / ip);
				this->horizontal = this->vertical = 1;
				if (this->speed == 0) {
					this->speed = 0.8f * ball->speed / c;
				}
				else {
					this->speed = 0.8f * ball->speed / c;
					this->angle = (this->angle + ball->angle) / 2;
				}
				if (this->angle > M_PI / 4.05f && this->angle < M_PI / 3.95f) {
					this->speed /= 4;
					ball->speed *= 4;
				}
				if (ball->x < this->x - error && ball->z > this->z + error) {
					ball->angle = this->angle + M_PI / 2;
				}
				ball->x = this->x - 2 * this->radius * sin(asin(cat1 / ip));
				ball->z = this->z + 2 * this->radius * cos(asin(cat1 / ip));
				this->frameMove = true;
			}

			//Caz 2
			if (ball->x >= this->x && ball->z < this->z) {
				this->angle = M_PI - asin(cat1 / ip);
				this->horizontal = this->vertical = 1;
				if (this->speed == 0) {
					this->speed = 0.8f * ball->speed / c;
				}
				else {
					this->speed = 0.8f * ball->speed / c;
					this->angle = (this->angle + ball->angle) / 2;
				}
				if (this->angle > M_PI / 4.05f && this->angle < M_PI / 3.95f) {
					this->speed /= 4;
					ball->speed *= 4;
				}
				if (ball->x > this->x + error && ball->z < this->z - error) {
					ball->angle = this->angle - M_PI / 2;
				}
				ball->x = this->x + 2 * this->radius * sin(asin(cat1 / ip));
				ball->z = this->z - 2 * this->radius * cos(asin(cat1 / ip));
				this->frameMove = true;
			}

			//Caz3
			if (ball->x < this->x && ball->z < this->z) {
				this->angle = M_PI + asin(cat1 / ip);
				this->horizontal = this->vertical = 1;
				if (this->speed == 0) {
					this->speed = 0.8f * ball->speed / c;
				}
				else {
					this->speed = 0.8f * ball->speed / c;
					this->angle = (this->angle + ball->angle) / 2;
				}
				if (this->angle > M_PI / 4.05f && this->angle < M_PI / 3.95f) {
					this->speed /= 4;
					ball->speed *= 4;
				}
				if (ball->x < this->x - error && ball->z < this->z - error) {
					ball->angle = this->angle - M_PI / 2;
				}
				ball->x = this->x - 2 * this->radius * sin(asin(cat1 / ip));
				ball->z = this->z - 2 * this->radius * cos(asin(cat1 / ip));
				this->frameMove = true;
			}
		}
	}
	else if (ball->speed == 0 && this->speed == 0) {
		float cat1 = abs(this->x - ball->x), cat2 = abs(this->z - ball->z), ip = 2 * this->radius, new_angle;
		if (cat1 * cat1 + cat2 * cat2 < ip * ip) {
			//Caz 1
			if (ball->x >= this->x && ball->z >= this->z) {
				this->x = ball->x - 2 * this->radius * sin(asin(cat1 / ip));
				this->z = ball->z - 2 * this->radius * cos(asin(cat1 / ip));
			}

			//Caz 2
			if (ball->x >= this->x && ball->z < this->z) {
				this->x = ball->x - 2 * this->radius * sin(asin(cat1 / ip));
				this->z = ball->z + 2 * this->radius * cos(asin(cat1 / ip));
			}

			//Caz3
			if (ball->x < this->x && ball->z < this->z) {
				this->x = ball->x + 2 * this->radius * sin(asin(cat1 / ip));
				this->z = ball->z + 2 * this->radius * cos(asin(cat1 / ip));
			}

			//Caz 4
			if (ball->x < this->x && ball->z >= this->z) {
				this->x = ball->x + 2 * this->radius * sin(asin(cat1 / ip));
				this->z = ball->z - 2 * this->radius * cos(asin(cat1 / ip));
			}
		}
	}
}

//This function checks if two stationary balls are colliding.
bool Ball::checkCollission(Ball * ball1, Ball * ball2)
{
	float cat1 = abs(ball1->x - ball2->x), cat2 = abs(ball1->z - ball2->z), ip = 2 * ball1->radius;
	if (cat1 * cat1 + cat2 * cat2 < ip * ip) {
		return true;
	}
	else {
		return false;
	}
}

//this function chancges the x and y coordinates of a ball according to it's speed and trajectory
void Ball::Move(float deltaTimeSeconds)
{
		float sp = this->speed * deltaTimeSeconds;
		if (sp > 2.0 / 3.0 * this->radius) {
			sp = 2.0 / 3.0 * this->radius;
		}

		if (this->speed != 0) {
			if (this->vertical == 1) {
				this->z -= this->vertical * sp * cos(this->angle);
			}
			else {
				this->z -= this->vertical * sp * cos(this->angle);
			}
			if (this->horizontal == 1) {
				this->x -= this->horizontal * sp * sin(this->angle);
			}
			else {
				this->x -= this->horizontal * sp * sin(this->angle);
			}
			this->speed -= sp;
			if (this->speed <= 0.05) {
				this->speed = 0;
			}
		}
}

//This function is used to simulate the friction between the ball and the table which will ultimately stop the ball
void Ball::Friction()
{
	if (this->speed > 0.0125f) {
		this->speed -= 0.0125f;
	}
	else {
		this->speed = 0.0f;
	}
}


/*This function checks if a the Ball entered one of the holes and if so it increments the number of balls
potted by a player, or it detects a foul.It also sets visible to false so the ball is no longer rendered*/
void Ball::inHole(float top, float bottom, float left_border, float right_border, Player* current, Player* other)
{
	float error = 0.05f;
	if ((this->z > top - this->radius - error / 2 && this->z < top + this->radius + error / 2) &&
		(this->x > right_border - this->radius - error || this->x < left_border + this->radius + error)) {
		this->visible = false;
		if (this->type.compare("White") != 0) {
			if (this->type.compare(current->type) == 0) {
				current->potted_balls++;
			}
			else {
				if (current->type.compare("Undecided") == 0) {
					current->potted_balls++;
					current->type = this->type;
					if (current->type.compare("Full") == 0) {
						other->type = "Halves";
					}
					else {
						other->type = "Full";
					}
				}
				else {
					other->potted_balls++;
				}
			}
		}
	}

	if ((this->z > bottom - this->radius - error / 2 && this->z < bottom + this->radius + error / 2) &&
		(this->x > right_border - this->radius - error || this->x < left_border + this->radius + error)) {
		this->visible = false;
		if (this->type.compare("White") != 0) {
			if (this->type.compare(current->type) == 0) {
				current->potted_balls++;
			}
			else {
				if (current->type.compare("Undecided") == 0) {
					current->potted_balls++;
					current->type = this->type;
					if (current->type.compare("Full") == 0) {
						other->type = "Halves";
					}
					else {
						other->type = "Full";
					}
				}
				else {
					other->potted_balls++;
				}
			}
		}
	}

	if ((this->z > -this->radius - error / 2 && this->z < this->radius + error / 2) && 
		(this->x > right_border - this->radius - error || this->x < left_border + this->radius + error)) {
		this->visible = false;
		if (this->type.compare("White") != 0) {
			if (this->type.compare(current->type) == 0) {
				current->potted_balls++;
			}
			else {
				if (current->type.compare("Undecided") == 0) {
					current->potted_balls++;
					current->type = this->type;
					if (current->type.compare("Full") == 0) {
						other->type = "Halves";
					}
					else {
						other->type = "Full";
					}
				}
				else {
					other->potted_balls++;
				}
			}
		}
	}

	if (this->type.compare("White") == 0 && this->visible == false) {
		this->x = 0;
		//this->y = 0.2 + this->radius / 2;
		this->z = 1.0f / 3.0f;
		this->visible = true;
		this->speed = 0;
		this->angle = 0;
		current->foul = true;
		current->change = true;
	}

	if (this->type.compare("Black") == 0 && this->visible == false) {
		current->potted_balls++;
		if (current->potted_balls == 8) {
			cout << "Player " << current->number << " has won the game!\n";
			current->win = true;
		}
		else {
			cout << "Player " << other->number <<" won the game!\n";
			other->win = true;
		}
	}
}

/*This function uses all the functions defined above together in order to simulate the movement
of a ball in relation to all the other balls and in relation to the table.*/
void Ball::MoveBall(Ball * balls[16], float deltaTimeSeconds, float top, float bottom,
	float left_border, float right_border, Player* current, Player* other)
{
	if (this->delay == 0) {
		glm::vec2 old_pos = glm::vec2(this->x, this->z);
		float old_speed = this->speed;
		this->Move(deltaTimeSeconds);
		this->inHole(top, bottom, left_border, right_border, current, other);
		this->Collission(top, bottom, left_border, right_border);
		int c = 0, cont, delay;
		for (int i = 15; i >= 0; i = i--) {
			if (balls[i]->visible == false) {
				continue;
			}
			if (this->checkCollission(this, balls[i]) && balls[i] != this) {
				c++;
				this->collissions++;
			}
		}
		cont = c;
		if (this->collissions > c) {
			delay = 0;
		}
		else {
			delay = 5;
		}

		for (int i = 15; i >= 0; i = i--) {
			if (balls[i]->visible == false) {
				continue;
			}
			if (cont > 0) {
				this->x = old_pos.x;
				this->z = old_pos.y;
				this->speed = old_speed;
				this->delay = 0;
				this->Move(deltaTimeSeconds);
			}
			if (this->checkCollission(this, balls[i]) && balls[i] != this && this->speed != 0) {
				if (c == 1 && this == balls[15]) {
					if (balls[i]->type.compare(current->type) != 0 &&
						current->type.compare("Undecided") != 0) {
						current->foul = true;
					}
				}
				cont--;

				balls[i]->BallsCollission(this, deltaTimeSeconds, c, balls, top, bottom, left_border, right_border, current);
				if (cont > 0) {
					this->speed = 0;
				}
				this->delay = delay;
			}
		}

		if (c > 0) {
			this->speed *= 0.2f;
		}
	}
	else {
		this->delay--;
	}

}
