#include <SDL.h>

class RenderComponent
{
public:
	/// <summary>
	/// Renders the object with the specified SDL_Renderer.
	/// </summary>
	/// <param name="ren">The SDL_Renderer.</param>
	virtual void render(SDL_Renderer* ren) = 0;
};