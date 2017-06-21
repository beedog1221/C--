#include <SDL.h>

class CollisionComponent
{
public:
	/// <summary>
	/// Handles the collision.
	/// </summary>
	/// <param name="player1">The player1 rect.</param>
	/// <param name="player2">The player2 rect.</param>
	/// <param name="rounding">The rounding rect.</param>
	virtual void handleCollision(SDL_Rect player1, SDL_Rect player2, SDL_Rect rounding) = 0;
};