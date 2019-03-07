#include "Pool.h"

#include <vector>
#include <string>
#include <iostream>
#include <stdio.h>

#include <Core/Engine.h>
#include "Transform3D.h"
#include "Ball.h"
#include "LabCamera.h"
#include "Player.h"

using namespace std;

//table dimenssions
float table_length = 2.0f, table_width = 1.0f, table_height = 0.2f;
//manta dimenssions
float manta_width = 0.1f, manta_height = 0.07f, manta_length = 2.0f;
//top, bottom, left and right size coordinates of the table
float top = -1.0f, bottom = 1.0f, left_border = -0.5f, right_border = 0.5f;
//white ball's starting position
float ball_x = 0, ball_z = bottom - 2.0f / 3 + 0.04f;
float ball_radius = 0.035f;
//the dimenssions of the cue
float cue_width = 0.015, cue_length = 1.2f, cue_x, cue_y, cue_z;
Ball *white_ball;
Ball *balls[16];
//check if the right mouse button has been pressed
bool draw = false, right_press = false;
bool balls_moving = false;
bool foul = true;
//variables which will count how many balls each playyer potted
int player1_balls = 0, player2_balls = 0;
//variables which will remember the type of ball each player has to pot
string player1_type = "Undecided", player2_type = "Undecided";
Player* player1;
Player* player2;
Player* currentPlayer, *otherPlayer;
bool endMove = false, space_press = false;
static int collissions = 0;

Pool::Pool()
{
}

Pool::~Pool()
{
}

void Pool::Init()
{
	polygonMode = GL_FILL;

	renderCameraTarget = false;

	camera = new Laborator::Camera();
	camera->Set(glm::vec3(0, 2, 3.5f), glm::vec3(ball_x, table_height / 2 + ball_radius / 2, ball_z), glm::vec3(0, 1, 0));

	projectionMatrix = glm::perspective(RADIANS(60), window->props.aspectRatio, 0.01f, 200.0f);

	//Player
	player1 = new Player(player1_balls, false, player1_type, 1);
	player2 = new Player(player2_balls, false, player2_type, 2);
	currentPlayer = player1;
	otherPlayer = player2;


	Mesh* mesh = new Mesh("box");
	mesh->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "box.obj");
	meshes[mesh->GetMeshID()] = mesh;

	Mesh* mesh1 = new Mesh("sphere");
	mesh1->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "sphere.obj");
	meshes[mesh1->GetMeshID()] = mesh1;


	//Initialize white ball
	white_ball = new Ball(ball_x, ball_z, ball_radius, "White");

	//Initialize other balls
	float error = 0.005f;
	float start_z = top + 2.0f / 3 - ball_radius, start_x = 0;
	balls[0] = new Ball(0, start_z, ball_radius, "Full");
	start_z -= 2 * ball_radius - ball_radius / 4 + error;
	start_x -= ball_radius + error / 2;
	int alternate[15];
	for (int i = 0; i < 15; i++) {
		alternate[i] = 1;
	}
	alternate[0] = alternate[2] = alternate[3] = alternate[7] = alternate[8] = alternate[10] = alternate[13] = -1;

	for (int i = 1; i < 3; i++) {
		if (alternate[i] == 1) {
			balls[i] = new Ball(start_x, start_z, ball_radius, "Halves");
		}
		else {
			balls[i] = new Ball(start_x, start_z, ball_radius, "Full");
		}
		start_x += 2 * ball_radius + error;
	}
	start_z -= 2 * ball_radius - ball_radius / 4 + error;;
	start_x = -2 * ball_radius - error;
	for (int i = 3; i < 6; i++) {
		if (i == 4) {
			balls[i] = new Ball(start_x, start_z, ball_radius, "Black");
			start_x += 2 * ball_radius + error;
			continue;
		}
		if (alternate[i] == 1) {
			balls[i] = new Ball(start_x, start_z, ball_radius, "Halves");
		}
		else {
			balls[i] = new Ball(start_x, start_z, ball_radius, "Full");
		}
		start_x += 2 * ball_radius + error;
	}
	start_z -= 2 * ball_radius - ball_radius / 4 + error;
	start_x = -3 * ball_radius - 3 * error / 2;
	for (int i = 6; i < 10; i++) {
		if (alternate[i] == 1) {
			balls[i] = new Ball(start_x, start_z, ball_radius, "Halves");
		}
		else {
			balls[i] = new Ball(start_x, start_z, ball_radius, "Full");
		}
		start_x += 2 * ball_radius + error;
	}
	start_z -= 2 * ball_radius - ball_radius / 4 + error;
	start_x = -4 * ball_radius - 2 * error;
	for (int i = 10; i < 15; i++) {
		if (alternate[i] == 1) {
			balls[i] = new Ball(start_x, start_z, ball_radius, "Halves");
		}
		else {
			balls[i] = new Ball(start_x, start_z, ball_radius, "Full");
		}
		start_x += 2 * ball_radius + error;
	}
	balls[15] = white_ball;


	cout << "Player 1 is shooting\n";
}

void Pool::FrameStart()
{
	// clears the color buffer (using the previously set color) and depth buffer
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::ivec2 resolution = window->GetResolution();
	// sets the screen area where to draw
	glViewport(0, 0, resolution.x, resolution.y);

	endMove = false;
	bool balls_moving_copy = balls_moving;
	balls_moving = false;
	if (white_ball->speed != 0) {
		balls_moving = true;
	}
	else {
		for (int i = 0; i < 16; i++) {
			//check if any ball is still moving
			if (balls[i]->speed != 0 && balls[i]->visible == true) {
				balls[i]->frameMove = false;
				balls_moving = true;
				break;
			}
		}
	}

	//check if this is the exact frame when all the balls stopped moving(the end of the Turn)
	if (balls_moving_copy == true && balls_moving == false) {
		endMove = true;
	}

	//if a player commited a foul increment the number of fouls commited by the player
	if (endMove == true) {
		if (currentPlayer->foul == true) {
			foul = true;
			space_press = false;
			currentPlayer->foul = false;
			currentPlayer->fouls++;
		}
	}

	//if it's the end of the turn let the other Player to shoot
	if (currentPlayer == player1 && endMove == true && currentPlayer->win == false && otherPlayer->win == false) {
		if (player1_balls == currentPlayer->potted_balls) {
			currentPlayer->change = true;
		}
		if (currentPlayer->change == true) {
			currentPlayer->change = false;
			currentPlayer = player2;
			if (currentPlayer->potted_balls != 0) {
				cout << "Player 2 is shooting and has potted " << currentPlayer->potted_balls << " " << currentPlayer->type << " balls until now.\n";
				cout << "Player 2 committed " << currentPlayer->fouls << " fouls.\n";
			}
			else {
				cout << "Player 2 is shooting.\n";
				cout << "Player 2 committed " << currentPlayer->fouls << " fouls.\n";
			}
		}
		else {
			cout << "Player " << currentPlayer->number << " is shooting and and has potted " << currentPlayer->potted_balls << " " << currentPlayer->type << " balls until now.\n";
			cout << "Player 1 committed " << currentPlayer->fouls << " fouls.\n";
		}
	} else if (currentPlayer == player2 && endMove == true && currentPlayer->win == false && otherPlayer->win == false) {
		if (player2_balls == currentPlayer->potted_balls) {
			currentPlayer->change = true;
		}
		if (currentPlayer->change == true) {
			currentPlayer->change = false;
			currentPlayer = player1;
			if (currentPlayer->potted_balls != 0) {
				cout << "Player 1 is shooting and has potted " << currentPlayer->potted_balls << " " << currentPlayer->type << " balls until now.\n";
				cout << "Player 1 committed " << currentPlayer->fouls << " fouls.\n";
			}
			else {
				cout << "Player 1 is shooting.\n";
				cout << "Player 1 committed " << currentPlayer->fouls << " fouls.\n";
			}
		} 
		else {
			cout << "Player " << currentPlayer->number << " is shooting and and has potted " << currentPlayer->potted_balls << " " << currentPlayer->type << " balls until now.\n";
			cout << "Player 2 committed " << currentPlayer->fouls << " fouls.\n";
		}
	}
}

void Pool::Update(float deltaTimeSeconds)
{
	glLineWidth(3);
	glPointSize(5);
	glPolygonMode(GL_FRONT_AND_BACK, polygonMode);

	if (!window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT)) {
		//if the balls are moving show a top down view of the table else show a "cue" perspective
		if (balls_moving == true || foul == true && space_press == false) {
			camera->Set(glm::vec3(0, 1.5, 0), glm::vec3(0, 0, 0), glm::vec3(0, 0, -1));
			camera->RotateThirdPerson_OY(M_PI / 2);
		}
		else {
			camera->Set(glm::vec3(white_ball->x, table_height / 2 + ball_radius + 0.75, white_ball->z + sqrt(4 - 0.75 * 0.75)), glm::vec3(white_ball->x, table_height / 2 + ball_radius, white_ball->z), glm::vec3(0, 1, 0));
			camera->RotateThirdPerson_OY(white_ball->angle);
		}
	}


	//Draw Table
	modelMatrix = glm::mat4(1);
	modelMatrix *= Transform3D::Scale(1.2, 0.2, 2.2);
	RenderMesh(meshes["box"], shaders["Green"], modelMatrix);


	//Cue
	cue_x = white_ball->x;
	cue_y = table_height - white_ball->radius / 2;
	cue_z = white_ball->z + white_ball->radius + cue_length / 2;


	if (white_ball->speed == 0.0f && draw == false && currentPlayer->win == false && otherPlayer->win == false) {
		modelMatrix = glm::mat4(1);
		modelMatrix *= Transform3D::Translate(cue_x + white_ball->radius * sin(white_ball->angle) + cue_length / 2 * sin(white_ball->angle),
			cue_y, cue_z - (white_ball->radius - white_ball->radius * cos(white_ball->angle)) - (cue_length / 2 - cue_length / 2 * cos(white_ball->angle)));
		modelMatrix *= Transform3D::RotateOY(white_ball->angle);
		modelMatrix *= Transform3D::RotateOX(-M_PI / 48);
		modelMatrix *= Transform3D::Scale(cue_width, cue_width, cue_length);
		RenderMesh(meshes["box"], shaders["Cue"], modelMatrix);
	}

	//If the cue is drawn(left mouse button is pressed and the balls are stationary) start the Cue Animation 
	if (draw == true && currentPlayer->win == false && otherPlayer->win == false) {
		modelMatrix = glm::mat4(1);
		modelMatrix *= Transform3D::Translate(cue_x + white_ball->radius * sin(white_ball->angle) + cue_length / 2 * sin(white_ball->angle),
			cue_y, cue_z - (white_ball->radius - white_ball->radius * cos(white_ball->angle)) - (cue_length / 2 - cue_length / 2 * cos(white_ball->angle)));
		modelMatrix *= Transform3D::RotateOY(white_ball->angle);
		modelMatrix *= Transform3D::RotateOX(-M_PI / 48);
		modelMatrix *= Transform3D::Scale(cue_width, cue_width, cue_length);
		RenderMeshCue(meshes["box"], shaders["CueAnimation"], modelMatrix, white_ball->angle);
		white_ball->vertical = 1.0f;
		white_ball->horizontal = 1.0f;
		player1_balls = player1->potted_balls;
		player2_balls = player2->potted_balls;
		foul = false;
	}

	//Move the white ball on the table when a foul is being committed using the WASD keys.
	if (foul == true && balls_moving == false) {
		if (window->KeyHold(GLFW_KEY_D) == true && white_ball->z - ball_radius > top) {
			white_ball->z -= deltaTimeSeconds / 2;
			for (int i = 0; i < 15; i++) {
				if (white_ball->checkCollission(white_ball, balls[i])) {
					white_ball->z += deltaTimeSeconds / 2;
					break;
				}
			}
		}

		if (window->KeyHold(GLFW_KEY_A) == true && white_ball->z + ball_radius < bottom) {
			white_ball->z += deltaTimeSeconds / 2;
			for (int i = 0; i < 15; i++) {
				if (white_ball->checkCollission(white_ball, balls[i])) {
					white_ball->z -= deltaTimeSeconds / 2;
					break;
				}
			}
		}

		if (window->KeyHold(GLFW_KEY_W) == true && white_ball->x - ball_radius > left_border) {
			white_ball->x -= deltaTimeSeconds / 2;
			for (int i = 0; i < 15; i++) {
				if (white_ball->checkCollission(white_ball, balls[i])) {
					white_ball->x += deltaTimeSeconds / 2;
					break;
				}
			}
		}

		if (window->KeyHold(GLFW_KEY_S) == true && white_ball->x + ball_radius < right_border) {
			white_ball->x += deltaTimeSeconds / 2;
			for (int i = 0; i < 15; i++) {
				if (white_ball->checkCollission(white_ball, balls[i])) {
					white_ball->x -= deltaTimeSeconds / 2;
					break;
				}
			}
		}
	}


	manta_length = 2.0f;

	//Manta stanga
	modelMatrix = glm::mat4(1);
	modelMatrix *= Transform3D::Translate(- table_width / 2 - manta_width / 2, table_height / 2 + manta_height / 2, 0.0f);
	modelMatrix *= Transform3D::Scale(manta_width, manta_height, manta_length);
	RenderMesh(meshes["box"], shaders["Manta"], modelMatrix);

	//Manta dreapta
	modelMatrix = glm::mat4(1);
	modelMatrix *= Transform3D::Translate(table_width / 2 + manta_width / 2, table_height / 2 + manta_height / 2, 0.0f);
	modelMatrix *= Transform3D::Scale(manta_width, manta_height, manta_length);
	RenderMesh(meshes["box"], shaders["Manta"], modelMatrix);

	//Manta sus
	manta_length = 0.1 * 2 + 1;
	modelMatrix = glm::mat4(1);
	modelMatrix *= Transform3D::Translate(0.0f, table_height / 2 + manta_height / 2, - table_length / 2 - manta_width / 2);
	modelMatrix *= Transform3D::Scale(manta_length, manta_height, manta_width);
	RenderMesh(meshes["box"], shaders["Manta"], modelMatrix);

	//Manta jos
	manta_length = 0.1 * 2 + 1;
	modelMatrix = glm::mat4(1);
	modelMatrix *= Transform3D::Translate(0.0f, table_height / 2 + manta_height / 2, table_length / 2 + manta_width / 2);
	modelMatrix *= Transform3D::Scale(manta_length, manta_height, manta_width);
	RenderMesh(meshes["box"], shaders["Manta"], modelMatrix);


	//Gaura colt stanga sus
	modelMatrix = glm::mat4(1);
	modelMatrix *= Transform3D::Translate(- table_width / 2, table_height / 2, - table_length / 2);
	modelMatrix *= Transform3D::Scale(0.13, 0.13, 0.13);
	RenderMesh(meshes["sphere"], shaders["VertexColor"], modelMatrix);

	//Gaura colt dreapta sus
	modelMatrix = glm::mat4(1);
	modelMatrix *= Transform3D::Translate(table_width / 2, table_height / 2, - table_length / 2);
	modelMatrix *= Transform3D::Scale(0.13, 0.13, 0.13);
	RenderMesh(meshes["sphere"], shaders["VertexColor"], modelMatrix);

	//Gaura colt stanga mijloc
	modelMatrix = glm::mat4(1);
	modelMatrix *= Transform3D::Translate(-table_width / 2, table_height / 2, 0);
	modelMatrix *= Transform3D::Scale(0.13, 0.13, 0.13);
	RenderMesh(meshes["sphere"], shaders["VertexColor"], modelMatrix);

	//Gaura colt dreapta mijloc
	modelMatrix = glm::mat4(1);
	modelMatrix *= Transform3D::Translate(table_width / 2, table_height / 2, 0);
	modelMatrix *= Transform3D::Scale(0.13, 0.13, 0.13);
	RenderMesh(meshes["sphere"], shaders["VertexColor"], modelMatrix);

	//Gaura colt stanga jos
	modelMatrix = glm::mat4(1);
	modelMatrix *= Transform3D::Translate(-table_width / 2, table_height / 2, table_length / 2);
	modelMatrix *= Transform3D::Scale(0.13, 0.13, 0.13);
	RenderMesh(meshes["sphere"], shaders["VertexColor"], modelMatrix);

	//Gaura colt dreapta jos
	modelMatrix = glm::mat4(1);
	modelMatrix *= Transform3D::Translate(table_width / 2, table_height / 2, table_length / 2);
	modelMatrix *= Transform3D::Scale(0.13, 0.13, 0.13);
	RenderMesh(meshes["sphere"], shaders["VertexColor"], modelMatrix);

	for (int i = 15; i >= 0; i = i--) {
		if (balls[i]->visible == false) {
			continue;
		}
		balls[i]->MoveBall(balls, deltaTimeSeconds, top, bottom, left_border, right_border, currentPlayer, otherPlayer);
		modelMatrix = glm::mat4(1);
		modelMatrix *= Transform3D::Translate(balls[i]->x, table_height / 2 + 2 * ball_radius / 2, balls[i]->z);
		modelMatrix *= Transform3D::Scale(2 * ball_radius, 2 * ball_radius, 2 * ball_radius);
		RenderMesh(meshes["sphere"], shaders[balls[i]->type], modelMatrix);
	}
}

void Pool::FrameEnd()
{
	//DrawCoordinatSystem(camera->GetViewMatrix(), projectionMatrix);
}

void Pool::RenderMesh(Mesh * mesh, Shader * shader, const glm::mat4 & modelMatrix)
{
	if (!mesh || !shader || !shader->program)
		return;

	// render an object using the specified shader and the specified position
	shader->Use();
	glUniformMatrix4fv(shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));
	glUniformMatrix4fv(shader->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
	glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));
	float time = Engine::GetElapsedTime();
	int location = glGetUniformLocation(shader->GetProgramID(), "Time");
	glUniform1f(location, time);


	glm::vec3 eyePosition = camera->position;
	int eye_position = glGetUniformLocation(shader->program, "eye_position");
	glUniform3f(eye_position, eyePosition.x, eyePosition.y, eyePosition.z);

	int object_color = glGetUniformLocation(shader->program, "object_color");
	glUniform3f(object_color, 0, 1, 0);

	mesh->Render();
}

void Pool::RenderMeshCue(Mesh * mesh, Shader * shader, const glm::mat4 & modelMatrix, float angle)
{
	if (!mesh || !shader || !shader->program)
		return;

	// render an object using the specified shader and the specified position
	shader->Use();
	glUniformMatrix4fv(shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));
	glUniformMatrix4fv(shader->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
	glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));
	float time = Engine::GetElapsedTime();
	int location = glGetUniformLocation(shader->GetProgramID(), "Time");
	glUniform1f(location, time);

	location = glGetUniformLocation(shader->GetProgramID(), "Angle");
	glUniform1f(location, angle);

	glm::vec3 eyePosition = camera->position;
	int eye_position = glGetUniformLocation(shader->program, "eye_position");
	glUniform3f(eye_position, eyePosition.x, eyePosition.y, eyePosition.z);

	int object_color = glGetUniformLocation(shader->program, "object_color");
	glUniform3f(object_color, 0, 1, 0);

	mesh->Render();
}

void Pool::OnInputUpdate(float deltaTime, int mods)
{
	// move the camera only if MOUSE_RIGHT button is pressed
	if (!window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
	{
		float cameraSpeed = 0.25f;

		if (window->KeyHold(GLFW_KEY_W)) {
			// TODO : translate the camera forward
			camera->TranslateForward(cameraSpeed);
		}

		if (window->KeyHold(GLFW_KEY_A)) {
			// TODO : translate the camera to the left
			camera->TranslateRight(-cameraSpeed);
		}

		if (window->KeyHold(GLFW_KEY_S)) {
			// TODO : translate the camera backwards
			camera->TranslateForward(-cameraSpeed);
		}

		if (window->KeyHold(GLFW_KEY_D)) {
			// TODO : translate the camera to the right
			camera->TranslateRight(cameraSpeed);
		}

		if (window->KeyHold(GLFW_KEY_Q)) {
			// TODO : translate the camera down
			camera->TranslateUpword(-cameraSpeed);
		}

		if (window->KeyHold(GLFW_KEY_E)) {
			// TODO : translate the camera up
			camera->TranslateUpword(cameraSpeed);
		}
	}
}

void Pool::OnKeyPress(int key, int mods)
{
	// add key press event
	if (key == GLFW_KEY_SPACE) {
		space_press = true;
	}
}

void Pool::OnKeyRelease(int key, int mods)
{
	// add key release event
}

void Pool::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{

	if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
	{
		float sensivityOX = 0.001f;
		float sensivityOY = 0.001f;

		renderCameraTarget = true;
		camera->RotateThirdPerson_OY(-deltaX * sensivityOY);
		white_ball->angle -= deltaX * sensivityOX;

	}
}

void Pool::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button press event
	if (IS_BIT_SET(button, GLFW_MOUSE_BUTTON_LEFT)) {
		if (balls_moving == false) {
			draw = true;
		}
	}

	if (IS_BIT_SET(button, GLFW_MOUSE_BUTTON_RIGHT)) {
		right_press = true;
		foul = false;
	}
}

void Pool::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button release event
	if (IS_BIT_SET(button, GLFW_MOUSE_BUTTON_LEFT)) {
		if (balls_moving == false && currentPlayer->win == false && otherPlayer->win == false) {
			draw = false;
			white_ball->speed = 3.2f;
			float time = Engine::GetElapsedTime();
			float percentage = 1 - abs(cos(time));
			white_ball->speed *= percentage;
		}
	}

	if (IS_BIT_SET(button, GLFW_MOUSE_BUTTON_RIGHT)) {
		right_press = false;
	}
}

void Pool::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}

void Pool::OnWindowResize(int width, int height)
{
}
