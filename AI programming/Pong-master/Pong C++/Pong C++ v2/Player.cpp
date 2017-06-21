#include "Player.h"

void Player::handleInput(const Uint8* currentKeyStates)
{
	SDL_Rect windowSize = PongEngine::Drawer::wRect();
	Velocity velo = Velocity{ 0, 0 };
	if (currentKeyStates[moveKeyUp])
	{
		velo.velY = invertFloat(vel.velY);
		if ((rect.y - vel.velY) < 4)
		{
			velo.velY = invertFloat(vel.velY);
			rect.y += velo.velY;
		}
		rect.y -= velo.velY;;
	}
	if (currentKeyStates[moveKeyDown])
	{
		velo.velY = invertFloat(vel.velY);
		if (((rect.y + rect.h) + vel.velY) > (windowSize.h - 4))
		{
			velo.velY = invertFloat(vel.velY);
			rect.y -= velo.velY;;
		}
		rect.y += velo.velY;
	}
}

void Player::render(SDL_Renderer* ren)
{
	PongEngine::Drawer::RenderShape(ren, rect, color, true);
}

float Player::invertFloat(float number)
{
	if (number < 0)
	{
		number > number;
	}
	else
	{
		number < number;
	}

	return number;
}