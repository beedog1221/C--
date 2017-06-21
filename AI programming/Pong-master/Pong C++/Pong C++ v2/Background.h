#pragma once
#include "Drawer.h"
#include "GameObject.h"

class Background :
	public PongEngine::GameObject
{
public:
	Background() {};
	Background(SDL_Rect rectangle, Velocity velo, SDL_Texture *tex, SDL_RendererFlip renFlip, SDL_Color objectColor) : GameObject(rectangle, velo, tex, renFlip)
	{
		color = objectColor;
	};
	~Background() {};

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
