#include <stdio.h>
#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "Ball.h"
#include "Player.h"
#include "Rounding.h"
#include "Background.h"
#include <SDL_mixer.h>
#include "Drawer.h"


const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;
const double MOVE_SPEED = 9;
const float ballSpeed = 11;

Background backgroundObj;
Ball ballObj;
Rounding roundingObj;
Player player1;
Player player2;

TTF_Font* font;

SDL_Texture* scoreTex;
SDL_Texture* startTex;

SDL_Color ballColor{ 0, 255, 0, 255 };
SDL_Color paddleColor{ 255, 0, 0, 255 };
SDL_Color roundingColor{ 0, 0, 0, 255 };
SDL_Color fontColor{ 255, 0, 255, 255 };
SDL_Color backgroundColor{ 55, 0, 255, 255 };

int scoreLeft = 0;
int scoreRight = 0;

int main(int argc, char* args[])
{
	//	Init the window
	try
	{
		//Initialize SDL_mixer
		Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
		PongEngine::Drawer::Init("Marco's Ping Pong", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	}
	catch (const std::runtime_error &e)
	{
		std::cout << e.what() << std::endl;
		PongEngine::Drawer::Quit();
		return -1;
	}
	font = PongEngine::Drawer::LoadFont("./Resources/Fonts/Roboto-Regular.ttf", 50);

	ballObj = Ball(SDL_Rect{ 100, 100, 20, 20 }, Velocity{ ballSpeed, ballSpeed }, PongEngine::Drawer::LoadImage("./Resources/Images/dot.bmp"), SDL_RendererFlip::SDL_FLIP_NONE, ballColor, Mix_LoadWAV("./Resources/Sounds/low.wav"));
	roundingObj = Rounding(SDL_Rect{ 2, 2, SCREEN_WIDTH - 4, SCREEN_HEIGHT - 4 }, Velocity{ 0, 0 }, NULL, SDL_RendererFlip::SDL_FLIP_NONE, roundingColor);
	player1 = Player(SDL_Rect{ 40, ((SCREEN_HEIGHT / 2) - 200), 20, 200 }, Velocity{ 0, MOVE_SPEED }, NULL, SDL_RendererFlip::SDL_FLIP_NONE, paddleColor);
	player2 = Player(SDL_Rect{ (SCREEN_WIDTH - 60), ((SCREEN_HEIGHT / 2) - 200), 20, 200 }, Velocity{ 0, MOVE_SPEED }, NULL, SDL_RendererFlip::SDL_FLIP_NONE, paddleColor, SDL_SCANCODE_UP, SDL_SCANCODE_DOWN);
	backgroundObj = Background(SDL_Rect{ 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT }, Velocity{ 0, 0 }, NULL, SDL_RendererFlip::SDL_FLIP_NONE, backgroundColor);

	SDL_Event e;
	bool quit = false;

	bool gamePaused = true;

	startTex = PongEngine::Drawer::GetRendererdFont(font, "Press enter to start!", fontColor);

	//Main loop
	while (!quit)
	{
		std::string resultScore1 = std::to_string(scoreLeft);
		std::string resultScore2 = std::to_string(scoreRight);
		scoreTex = PongEngine::Drawer::GetRendererdFont(font, resultScore1 + " - " + resultScore2, fontColor);

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

			if (gamePaused)
			{
				if (e.key.keysym.sym == SDLK_RETURN || e.key.keysym.sym == SDLK_RETURN2)
				{
					gamePaused = false;
				}
			}
			else
			{
				if (e.key.keysym.sym == SDLK_p)
				{
					gamePaused = true;
				}
				if (e.key.keysym.sym == SDLK_r)
				{
					gamePaused = true;
					scoreLeft = 0;
					scoreRight = 0;
					ballObj.SetPosition(100, 100);
					player1.SetPosition(40, ((SCREEN_HEIGHT / 2) - 200));
					player2.SetPosition((SCREEN_WIDTH - 60), ((SCREEN_HEIGHT / 2) - 200));
					ballObj.SetVelocity(ballSpeed, ballSpeed);
				}
			}
		}
		//Check if game is paused
		if (!gamePaused)
		{
			const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);

			//Handle Input
			player1.handleInput(currentKeyStates);
			player2.handleInput(currentKeyStates);

			//Update
			std::string scorer = ballObj.update(player1, player2, roundingObj);
			if (scorer == "LEFT")
			{
				scoreLeft++;
			}
			else if (scorer == "RIGHT")
			{
				scoreRight++;
			}

			//Rendering
			PongEngine::Drawer::ClearRenderer(PongEngine::Drawer::GetRenderer());

			backgroundObj.render(PongEngine::Drawer::GetRenderer());
			roundingObj.render(PongEngine::Drawer::GetRenderer());
			player1.render(PongEngine::Drawer::GetRenderer());
			player2.render(PongEngine::Drawer::GetRenderer());
			ballObj.render(PongEngine::Drawer::GetRenderer());

			int w, h;
			SDL_QueryTexture(scoreTex, NULL, NULL, &w, &h);
			int x = (SCREEN_WIDTH / 2) - (w / 2);
			SDL_Rect fontRect = { x, 20, w, h };

			SDL_RenderCopy(PongEngine::Drawer::GetRenderer(), scoreTex, NULL, &fontRect);

			PongEngine::Drawer::Present(PongEngine::Drawer::GetRenderer());
		}
		else
		{
			//Rendering
			PongEngine::Drawer::ClearRenderer(PongEngine::Drawer::GetRenderer());

			backgroundObj.render(PongEngine::Drawer::GetRenderer());
			roundingObj.render(PongEngine::Drawer::GetRenderer());
			ballObj.render(PongEngine::Drawer::GetRenderer());
			player1.render(PongEngine::Drawer::GetRenderer());
			player2.render(PongEngine::Drawer::GetRenderer());

			int w, h;
			SDL_QueryTexture(startTex, NULL, NULL, &w, &h);
			int x = (SCREEN_WIDTH / 2) - (w / 2);
			int y = (SCREEN_HEIGHT / 2) - (h / 2);
			SDL_Rect fontRect = { x, y, w, h };

			SDL_RenderCopy(PongEngine::Drawer::GetRenderer(), startTex, NULL, &fontRect);

			PongEngine::Drawer::Present(PongEngine::Drawer::GetRenderer());
		}

		
		SDL_DestroyTexture(scoreTex);
	}
	//Quit everything after main loop
	PongEngine::Drawer::Quit();

	return 0;
}