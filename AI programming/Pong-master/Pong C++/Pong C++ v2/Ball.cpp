#include "Ball.h"

void Ball::render(SDL_Renderer* ren)
{
	//Set pivot to center of object
	double xPivot = rect.w / 2;
	double yPivot = rect.h / 2;

	//Render the game object
	SDL_Rect *clip = NULL;
	PongEngine::Drawer::Draw(ren, texture, rect, color, clip, 0.0F, xPivot, yPivot, flip);
}

std::string Ball::update(GameObject& player1, GameObject& player2, GameObject& rounding)
{
	std::string side = "";

	SDL_Rect windowSize = PongEngine::Drawer::wRect();
	
	
	//move ball
	rect.x += vel.velX;
	rect.y += vel.velY;

	if (rect.x < (player1.GetRect().x + player1.GetRect().w) && rect.x > player1.GetRect().x && rect.y < (player1.GetRect().y + player1.GetRect().h) && rect.y > player1.GetRect().y)
	{
		//Hit PLAYER 1
		vel.velX = invertFloat(vel.velX);
		Mix_PlayChannel(-1, objSound, 0);
	}

	if ((rect.x + rect.w) > player2.GetRect().x && (rect.x + rect.w) < player2.GetRect().x + player2.GetRect().w && rect.y < player2.GetRect().y + player2.GetRect().h && rect.y > player2.GetRect().y)
	{
		//HIT PLAYER 2
		vel.velX = invertFloat(vel.velX );
		Mix_PlayChannel(-1, objSound, 0);
	}

	if (rect.x < 4)
	{
		// hit left wall
		//SCORE RIGHT +1;
		side = "RIGHT";
		rect.x = windowSize.w / 2;
		rect.y = windowSize.h / 2;
		vel.velX = fabs(vel.velX); // force it to be negative
	}

	if (rect.x > (windowSize.w - 4))
	{
		//hit right wall
		//SCORE LEFT +1;
		side = "LEFT";
		rect.x = windowSize.w / 2;
		rect.y = windowSize.h / 2;
		vel.velX = -fabs(vel.velX); // force it to be positive
	}

	if (rect.y < (windowSize.h - rounding.GetRect().h))
	{
		//hit top wall
		vel.velY = fabs(vel.velY);
		Mix_PlayChannel(-1, objSound, 0);
	}

	if ((rect.y + rect.h) > windowSize.h - 4)
	{
		//hit bottom wall
		vel.velY = -fabs(vel.velY);
		Mix_PlayChannel(-1, objSound, 0);
	}

	return side;
}

void Ball::handleCollision(SDL_Rect player1, SDL_Rect player2, SDL_Rect rounding) {}

void Ball::vec2_norm(float& x, float &y)
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

float Ball::invertFloat(float number)
{
	if (number < 0)
	{
		number = +fabs(number);
	}
	else
	{
		number = -fabs(number);
	}

	return number;
}