#pragma once
#include "Drawer.h"
#include "GameObject.h"

class Rounding :
	public PongEngine::GameObject
{
public:
	Rounding() {};
	Rounding(SDL_Rect rectangle, Velocity velo, SDL_Texture *tex, SDL_RendererFlip renFlip, SDL_Color objColor) : GameObject(rectangle, velo, tex, renFlip) 
	{
		color = objColor; 
	};
	~Rounding() {};

	/// <summary>
	/// Renders the object with the specified SDL_Renderer.
	/// </summary>
	/// <param name="ren">The SDL_Renderer.</param>
	void render(SDL_Renderer* ren);
private:
	/// <summary>
	/// The color of the object.
	/// </summary>
	SDL_Color color;
};
