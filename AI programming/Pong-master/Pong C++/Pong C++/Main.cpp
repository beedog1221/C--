#include <SDL.h>
#include <stdio.h>
#include <iostream>
#include "Window.h"
#include <SDL_image.h>

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;
const int MOVE_SPEED = 3;
float ball_dir_x = -1.0f;
float ball_dir_y = 0.0f;
float ballSpeed = 10.0f;
SDL_Rect ball;
SDL_Rect player1;
SDL_Rect player2;
SDL_Rect rounding;

void updateBall();
void vec2_norm(float& x, float &y);

int main(int argc, char* args[])
{

	//Init the window
	try
	{
		Window::Init("Ping Pong", SCREEN_WIDTH, SCREEN_HEIGHT);
	}
	catch (const std::runtime_error &e)
	{
		std::cout << e.what() << std::endl;
		Window::Quit();
		return -1;
	}

	//Player creation
	player1.w = 20;
	player1.h = 200;
	player1.x = 20;
	player1.y = (SCREEN_HEIGHT / 2) - player1.h / 2;

	//Player creation
	player2.w = 20;
	player2.h = 200;
	player2.x = SCREEN_WIDTH - player2.w - 20;
	player2.y = (SCREEN_HEIGHT / 2) - player2.h / 2;

	SDL_Texture *dot = Window::LoadImage("D:/Downloads/dot.bmp");
	ball.x = 100;
	ball.y = 100;
	ball.w = 20;
	ball.h = 20;

	rounding.x = 2;
	rounding.y = 2;
	rounding.w = SCREEN_WIDTH - 4;
	rounding.h = SCREEN_HEIGHT - 4;

	SDL_Event e;
	bool quit = false;

	//Main loop
	while (!quit)
	{
		//Handle events on queue
		while (SDL_PollEvent(&e) != 0)
		{
			//User requests quit
			if (e.type == SDL_QUIT)
			{
				quit = true;
			}
			if (e.key.keysym.sym == SDLK_ESCAPE)
			{
				quit = true;
			}
		}
		const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);
		if (currentKeyStates[SDL_SCANCODE_W])
		{
			player1.y -= MOVE_SPEED;
			if ((player1.y - MOVE_SPEED) < 0)
			{
				player1.y += MOVE_SPEED;
			}
		}
		if (currentKeyStates[SDL_SCANCODE_S])
		{
			player1.y += MOVE_SPEED;
			if ((player1.y + MOVE_SPEED) > (SCREEN_HEIGHT - player1.h))
			{
				player1.y -= MOVE_SPEED;
			}
		}
		if (currentKeyStates[SDL_SCANCODE_UP])
		{
			player2.y -= MOVE_SPEED;
			if ((player2.y - MOVE_SPEED) < 0)
			{
				player2.y += MOVE_SPEED;
			}
		}
		if (currentKeyStates[SDL_SCANCODE_DOWN])
		{
			player2.y += MOVE_SPEED;
			if ((player2.y + MOVE_SPEED) > (SCREEN_HEIGHT - player2.h))
			{
				player2.y -= MOVE_SPEED;
			}
		}

		updateBall();


		//Rendering
		Window::Clear();

		//Render outline
		Window::RenderShape(rounding, false);

		//Render player 1
		Window::RenderShape(player1, true);

		//REnder player 2
		Window::RenderShape(player2, true);

		Window::Draw(dot, ball);
		Window::Present();
	}

	//Quit everything after main loop
	Window::Quit();

	return 0;
}

void updateBall()
{
	//move ball
	ball.x += ball_dir_x * ballSpeed;
	ball.y += ball_dir_y * ballSpeed;

	//hit left racket
	if (ball.x < player1.x + player1.w &&
		ball.x > player1.x &&
		ball.y < player1.y + player1.h &&
		ball.y > player1.y)
	{
		// set fly direction depending on where it hit the racket
		// (t is 0.5 if hit at top, 0 at center, -0.5 at bottom)
		float t = ((ball.y - player1.y) / player1.h) - 0.5f;
		ball_dir_x = fabs(ball_dir_x); // force it to be positive
		ball_dir_y = t;
	}
	// hit by right racket?
	if ((ball.x + ball.w) > player2.x &&
		(ball.x + ball.w) < player2.x + player2.w &&
		ball.y < player2.y + player2.h &&
		ball.y > player2.y)
	{
		// set fly direction depending on where it hit the racket
		// (t is 0.5 if hit at top, 0 at center, -0.5 at bottom)
		float t = ((ball.y - player2.y) / player2.h) - 0.5f;
		ball_dir_x = -fabs(ball_dir_x); // force it to be negative
		ball_dir_y = t;
	}

	// hit left wall?
	if (ball.x < 4)
	{
	//	++score_right;
		ball.x = SCREEN_WIDTH / 2;
		ball.y = SCREEN_HEIGHT / 2;
		ball_dir_x = fabs(ball_dir_x); // force it to be positive
		ball_dir_y = 0;
	}

	// hit right wall?
	if (ball.x > (SCREEN_WIDTH - 4))
	{
		//++score_left;
		ball.x = SCREEN_WIDTH / 2;
		ball.y = SCREEN_HEIGHT / 2;
		ball_dir_x = -fabs(ball_dir_x); // force it to be negative
		ball_dir_y = 0;
	}

	// hit bottom wall?
	if (ball.y > (SCREEN_HEIGHT - 4))
	{
		ball_dir_y = -fabs(ball_dir_y); // force it to be negative
	}

	// hit top wall?
	if (ball.y < (SCREEN_HEIGHT - rounding.h))
	{
		ball_dir_y = fabs(ball_dir_y); // force it to be positive
	}

	// make sure that length of dir stays at 1
	vec2_norm(ball_dir_x, ball_dir_y);
}

void vec2_norm(float& x, float &y)
{
	// sets a vectors length to 1 (which means that x + y == 1)
	float length = sqrt((x * x) + (y * y));
	if (length != 0.0f)
	{
		length = 1.0f / length;
		x *= length;
		y *= length;
	}
}