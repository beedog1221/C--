#pragma once
#include "Drawer.h"
#include "GameObject.h"
#include <SDL_mixer.h>

class Ball :
	public PongEngine::GameObject
{
public:
	Ball() {};
	Ball(SDL_Rect rectangle, Velocity velo, SDL_Texture *tex, SDL_RendererFlip renFlip, SDL_Color objColor, Mix_Chunk *sound) : GameObject(rectangle, velo, tex, renFlip) 
	{
		color = objColor; objSound = sound;
	};

	/// <summary>
	/// Renders the object with the specified SDL_Renderer.
	/// </summary>
	/// <param name="ren">The SDL_Renderer.</param>
	void render(SDL_Renderer* ren);

	/// <summary>
	/// Updates the ball object.
	/// </summary>
	/// <param name="player1Obj">The player1 object.</param>
	/// <param name="player2Obj">The player2 object.</param>
	/// <param name="roundingObj">The rounding object.</param>
	std::string update(GameObject& player1, GameObject& player2, GameObject& rounding);

	/// <summary>
	/// Handles the collision.
	/// </summary>
	/// <param name="player1">The player1 sdl_rect.</param>
	/// <param name="player2">The player2 sdl_rect.</param>
	/// <param name="rounding">The rounding sdl_rect.</param>
	void handleCollision(SDL_Rect player1, SDL_Rect player2, SDL_Rect rounding);

	/// <summary>
	/// Gets the color of the object.
	/// </summary>
	/// <returns>Color of Object.</returns>
	SDL_Color GetColor()
	{
		return color;
	}

	/// <summary>
	/// Gets the sound of the object.
	/// </summary>
	/// <returns>Sound of Object.</returns>
	Mix_Chunk* GetSound()
	{
		return objSound;
	}

protected:
	/// <summary>
	/// Inverts the float.
	/// </summary>
	/// <param name="number">The number.</param>
	/// <returns></returns>
	float invertFloat(float number);

	/// <summary>
	/// Normalizes the direction.
	/// </summary>
	/// <param name="x">The x direction.</param>
	/// <param name="y">The y direction.</param>
	void vec2_norm(float& x, float &y);

	/// <summary>
	/// The color of the object.
	/// </summary>
	SDL_Color color;

	/// <summary>
	/// The object sound.
	/// </summary>
	Mix_Chunk *objSound;
};
