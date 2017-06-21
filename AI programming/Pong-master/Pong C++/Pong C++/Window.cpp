#include "Window.h"
#include <SDL_image.h>

//Initialize the unique_ptr's deleters here
std::unique_ptr<SDL_Window, void(*)(SDL_Window*)> Window::mWindow
= std::unique_ptr<SDL_Window, void(*)(SDL_Window*)>(nullptr, SDL_DestroyWindow);
std::unique_ptr<SDL_Renderer, void(*)(SDL_Renderer*)> Window::mRenderer
= std::unique_ptr<SDL_Renderer, void(*)(SDL_Renderer*)>(nullptr, SDL_DestroyRenderer);
//Other static members
SDL_Rect Window::mBox;

Window::Window()
{
}


Window::~Window()
{
}

void Window::Init(std::string title, int width, int height)
{
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		throw std::runtime_error("SDL Init Failed");
	}

	//Setup window size
	mBox.x = 0;
	mBox.y = 0;
	mBox.w = width;
	mBox.h = height;

	//Create the window
	mWindow.reset(SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, mBox.w, mBox.h, SDL_WINDOW_SHOWN));

	//Check if window is created ok
	if (mWindow == nullptr)
	{
		throw std::runtime_error("Failed to create window");
	}

	//Create the renderer
	mRenderer.reset(SDL_CreateRenderer(mWindow.get(), -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC));

	//Check if renderer is created ok
	if (mRenderer == nullptr)
	{
		throw std::runtime_error("Failed to create renderer");
	}
}

void Window::Quit()
{
	SDL_Quit();
}

SDL_Texture* Window::LoadImage(const std::string &file)
{
	SDL_Surface * surface = IMG_Load(file.c_str());
	SDL_Texture* tex = nullptr;

	if (surface != NULL)
	{
		SDL_SetColorKey(surface, SDL_TRUE, SDL_MapRGB(surface->format, 255, 0, 255));
	}
	tex = SDL_CreateTextureFromSurface(mRenderer.get(), surface);
	if (tex == nullptr)
		throw std::runtime_error("Failed to load image: " + file + IMG_GetError());
	return tex;	
}

void Window::Draw(SDL_Texture *tex, SDL_Rect &dstRect, SDL_Rect *clip, float angle, int xPivot, int yPivot, SDL_RendererFlip flip)
{
	//Convert pivot to center of texture
	xPivot += dstRect.w / 2;
	yPivot += dstRect.h / 2;

	//SDL expect an SDL_Point as the pivot location
	SDL_Point pivot = { xPivot, yPivot };

	//Draw the texture
	SDL_RenderCopyEx(mRenderer.get(), tex, clip, &dstRect, angle, &pivot, flip);
}

void Window::RenderShape(SDL_Rect rect, bool fill)
{
	SDL_SetRenderDrawColor(mRenderer.get(), 255, 255, 255, 255);
	if (fill)
	{
		SDL_RenderFillRect(mRenderer.get(), &rect);
	}
	else
	{
		SDL_RenderDrawRect(mRenderer.get(), &rect);
	}

	SDL_SetRenderDrawColor(mRenderer.get(), 0, 0, 0, 255);
}

void Window::Clear()
{
	
	SDL_RenderClear(mRenderer.get());
}

void Window::Present()
{
	SDL_RenderPresent(mRenderer.get());
}

SDL_Rect Window::Box()
{
	//update rect to match current window size
	SDL_GetWindowSize(mWindow.get(), &mBox.w, &mBox.h);
	return mBox;
}