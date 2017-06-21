#pragma once
#include <SDL.h>
#include "Drawer.h"
#include <vector>

#ifdef PONGENGINE_EXPORTS
#define PONGENGINE_API __declspec(dllexport)
#else
#define PONGENGINE_API __declspec(dllimport)
#endif

struct Velocity
{
	float velX;
	float velY;
};
namespace PongEngine
{
	class GameObject
	{
	public:
		GameObject()
		{
			rect = SDL_Rect{ 0, 0, 10, 10 };
			vel = Velocity{ 2, 2 };
			texture = NULL;
			flip = SDL_RendererFlip::SDL_FLIP_NONE;
		}

		GameObject(SDL_Rect rectangle, Velocity velo, SDL_Texture *tex, SDL_RendererFlip renFlip)
		{
			rect = rectangle;
			vel = velo;
			texture = tex;
			flip = renFlip;
		}

		/// <summary>
		/// Gets the rect.
		/// </summary>
		/// <returns>Rect of Object</returns>
		SDL_Rect GetRect()
		{
			return rect;
		}

		/// <summary>
		/// Gets the velocity.
		/// </summary>
		/// <returns>The velocity.</returns>
		Velocity GetVelocity()
		{
			return vel;
		}

		/// <summary>
		/// Gets the texture.
		/// </summary>
		/// <returns>Texture of object.</returns>
		SDL_Texture* GetTexture();

		/// <summary>
		/// Sets the position.
		/// </summary>
		/// <param name="x">The x position.</param>
		/// <param name="y">The y position.</param>
		void SetPosition(int x, int y)
		{
			rect.x = x;
			rect.y = y;
		}

		/// <summary>
		/// Sets the velocity.
		/// </summary>
		/// <param name="velX">The x velocity.</param>
		/// <param name="velY">The y velocity.</param>
		void SetVelocity(int velX, int velY)
		{
			vel = Velocity{ velX, velY };
		}

	protected:
		/// <summary>
		/// The rectangle of the game object
		/// </summary>
		SDL_Rect rect;

		/// <summary>
		/// The velocity of the game object
		/// </summary>
		Velocity vel;

		/// <summary>
		/// The texture of the game object
		/// </summary>
		SDL_Texture *texture;

		/// <summary>
		/// The renderer flip
		/// </summary>
		SDL_RendererFlip flip;

		/// <summary>
		/// Renders the object with the specified SDL_Renderer.
		/// </summary>
		/// <param name="ren">The SDL_Renderer.</param>
		virtual void render(SDL_Renderer* ren) = 0
		{
			Drawer::RenderShape(ren, rect, SDL_Color{ 255, 255, 255, 255 }, true);
		}
	};
}