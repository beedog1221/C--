#pragma once
#include "Drawer.h"
#include "GameObject.h"

class Player : public PongEngine::GameObject
{
public:
	Player() {};
	Player(SDL_Rect rectangle, Velocity velo, SDL_Texture *tex, SDL_RendererFlip renFlip, SDL_Color objColor, SDL_Scancode moveObjUp = SDL_SCANCODE_W, SDL_Scancode moveObjDown = SDL_SCANCODE_S) : GameObject(rectangle, velo, tex, renFlip)
	{
		moveKeyUp = moveObjUp;
		moveKeyDown = moveObjDown;
		color = objColor;
	};
	~Player() {};

	/// <summary>
	/// Renders the object with the specified SDL_Renderer.
	/// </summary>
	/// <param name="ren">The SDL_Renderer.</param>
	void render(SDL_Renderer* ren);

	/// <summary>
	/// Handles the input.
	/// </summary>
	/// <param name="currentKeyStates">The current key states.</param>
	void handleInput(const Uint8* currentKeyStates);
private:
	/// <summary>
	/// The key to move the object up.
	/// </summary>
	SDL_Scancode moveKeyUp;

	/// <summary>
	/// The key to move the object down.
	/// </summary>
	SDL_Scancode moveKeyDown;

	/// <summary>
	/// The color of the object.
	/// </summary>
	SDL_Color color;

	/// <summary>
	/// Inverts the float.
	/// </summary>
	/// <param name="number">The number.</param>
	/// <returns></returns>
	float invertFloat(float number);

	/// <summary>
	/// Moves the object with the specified velocity.
	/// </summary>
	/// <param name="velo">The velocity.</param>
	/// <param name="deltaTime">The delta time.</param>
	void move(Velocity velo, float deltaTime);
};
