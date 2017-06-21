#include <iostream>
#include "Drawer.h"

namespace PongEngine
{
	//Initialize the unique_ptr's deleters here
	std::unique_ptr<SDL_Window, void(*)(SDL_Window*)> Drawer::window
		= std::unique_ptr<SDL_Window, void(*)(SDL_Window*)>(nullptr, SDL_DestroyWindow);
	std::unique_ptr<SDL_Renderer, void(*)(SDL_Renderer*)> Drawer::renderer
		= std::unique_ptr<SDL_Renderer, void(*)(SDL_Renderer*)>(nullptr, SDL_DestroyRenderer);
	//Other static members
	SDL_Rect Drawer::windowRect;

	void Drawer::Init(std::string title, int xPos, int yPos, int width, int height, Uint32 wFlags, Uint32 rFlags)
	{
		if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
		{
			throw std::runtime_error("SDL Init Failed");
		}

		if (TTF_Init() != 0)
		{
			SDL_Quit();
		}

		CreateWindow(title, xPos, yPos, width, height, wFlags);

		CreateRenderer(window.get(), -1, rFlags);
	}

	void Drawer::CreateWindow(std::string title, int x, int y, int width, int height, Uint32 flags)
	{
		//Setup window size
		windowRect.x = x;
		windowRect.y = y;
		windowRect.w = width;
		windowRect.h = height;

		//Create the window
		window.reset(SDL_CreateWindow(title.c_str(), x, y, windowRect.w, windowRect.h, flags));

		//Check if window is created ok
		if (window == nullptr)
		{
			throw std::runtime_error("Failed to create window");
		}
	}

	void Drawer::CreateRenderer(SDL_Window *window, int index, Uint32 flags)
	{
		//Create the renderer
		renderer.reset(SDL_CreateRenderer(window, index, flags));

		//Check if renderer is created ok
		if (renderer == nullptr)
		{
			throw std::runtime_error("Failed to create renderer");
		}
	}

	void Drawer::Quit()
	{
		SDL_DestroyRenderer(renderer.get());
		SDL_DestroyWindow(window.get());
		TTF_Quit();
		IMG_Quit();
		SDL_Quit();
	}

	SDL_Texture* Drawer::LoadImage(const std::string &file)
	{
		SDL_Surface * surface = IMG_Load(file.c_str());
		SDL_Texture* tex = nullptr;

		if (surface != NULL)
		{
			SDL_SetColorKey(surface, SDL_TRUE, SDL_MapRGB(surface->format, 255, 0, 255));
		}
		tex = SDL_CreateTextureFromSurface(renderer.get(), surface);

		if (tex == nullptr)
			throw std::runtime_error("Failed to load image: " + file + IMG_GetError());

		return tex;
	}

	TTF_Font* Drawer::LoadFont(const std::string &fontFile, int fontSize)
	{
		//Open font
		TTF_Font *font = TTF_OpenFont(fontFile.c_str(), fontSize);
		if (font == NULL)
		{
			TTF_CloseFont(font);
			throw std::runtime_error("Failed to load font");
		}

		return font;
	}

	SDL_Texture* Drawer::GetRendererdFont(TTF_Font *font, const std::string &message, SDL_Color color)
	{
		SDL_Surface *surf = TTF_RenderText_Blended(font, message.c_str(), color);
		if (surf == NULL)
		{
			//TTF_CloseFont(font);
			throw std::runtime_error("Failed to create surface for font");
		}

		SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer.get(), surf);
		if (texture == NULL)
		{
			throw std::runtime_error("Failed to create texture for font");
		}

		SDL_FreeSurface(surf);
		//TTF_CloseFont(font);
		return texture;
	}

	void Drawer::Draw(SDL_Renderer *ren, SDL_Texture *tex, SDL_Rect &dstRect, SDL_Color color, SDL_Rect *clip, float angle, int xPivot, int yPivot, SDL_RendererFlip flip)
	{
		//SDL expect an SDL_Point as the pivot location
		SDL_Point pivot = { xPivot, yPivot };

		//Set color of texture if necessary
		SDL_SetTextureColorMod(tex, color.r, color.g, color.b);

		//Draw the texture
		SDL_RenderCopyEx(ren, tex, clip, &dstRect, angle, &pivot, flip);
	}

	void Drawer::RenderShape(SDL_Renderer *ren, SDL_Rect rect, SDL_Color color, bool fill)
	{
		SDL_SetRenderDrawColor(ren, color.r, color.g, color.b, color.a);
		if (fill)
		{
			SDL_RenderFillRect(ren, &rect);
		}
		else
		{
			SDL_RenderDrawRect(ren, &rect);
		}

		SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
	}

	void Drawer::ClearRenderer(SDL_Renderer *ren)
	{
		SDL_RenderClear(ren);
	}

	void Drawer::Present(SDL_Renderer *ren)
	{
		SDL_RenderPresent(ren);
	}

	SDL_Rect Drawer::wRect()
	{
		//update rectangle to match current window size
		SDL_GetWindowSize(window.get(), &windowRect.w, &windowRect.h);
		return windowRect;
	}

	SDL_Window* Drawer::GetWindow()
	{
		return window.get();
	}

	SDL_Renderer* Drawer::GetRenderer()
	{
		return renderer.get();
	}
}