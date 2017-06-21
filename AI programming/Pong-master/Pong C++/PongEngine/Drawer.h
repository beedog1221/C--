#pragma once
#include <SDL.h>
#include <SDL_TTF.h>
#include <SDL_image.h>
#include <string>
#include <memory>

#ifdef PONGENGINE_EXPORTS
#define PONGENGINE_API __declspec(dllexport)
#else
#define PONGENGINE_API __declspec(dllimport)
#endif

namespace PongEngine
{
	class Drawer
	{
	public:
		/// <summary>
		/// Initializes the window.
		/// </summary>
		/// <param name="title">The title of the window.</param>
		/// <param name="xPos">The x position of the window.</param>
		/// <param name="yPos">The y position of the window.</param>
		/// <param name="width">The width of the window.</param>
		/// <param name="height">The height of the window.</param>
		/// <param name="wFlags">The window flags.</param>
		/// <param name="rFlags">The render flags.</param>		
		/// \see CreateWindow
		/// \see CreateRenderer
		static PONGENGINE_API void Init(std::string title = "Window", int xPos = 0, int yPos = 0, int width = 640, int height = 480, Uint32 wFlags = SDL_WINDOW_SHOWN, Uint32 rFlags = SDL_RENDERER_ACCELERATED);

		/// <summary>
		/// Creates the window.
		/// </summary>
		/// <param name="title">The title of the window.</param>
		/// <param name="x">The x position of the window.</param>
		/// <param name="y">The y position of the window.</param>
		/// <param name="width">The width of the window.</param>
		/// <param name="height">The height of the window.</param>
		/// <param name="flags">The window flags.</param>
		static PONGENGINE_API void CreateWindow(std::string title, int x, int y, int width, int height, Uint32 flags);

		/// <summary>
		/// Creates the renderer.
		/// </summary>
		/// <param name="window">The window on which the renderer needs to be created on.</param>
		/// <param name="index">The index of rendering driver.</param>
		/// <param name="flags">The render flags.</param>
		static PONGENGINE_API void CreateRenderer(SDL_Window *window, int index, Uint32 flags);

		/// <summary>
		/// Quits everything.
		/// </summary>
		static PONGENGINE_API void Quit();

		/// <summary>
		/// Draws a texture.
		/// </summary>
		/// <param name="ren">The renderer to render to.</param>
		/// <param name="tex">The texture to render.</param>
		/// <param name="dstRect">The destination rect.</param>
		/// <param name="color">The color of the texture.</param>
		/// <param name="clip">The rect size of the original texture.</param>
		/// <param name="angle">The angle of the texture.</param>
		/// <param name="xPivot">The x pivot.</param>
		/// <param name="yPivot">The y pivot.</param>
		/// <param name="flip">The renderer flip.</param>
		static PONGENGINE_API void Draw(SDL_Renderer *ren, SDL_Texture *tex, SDL_Rect &dstRect, SDL_Color color, SDL_Rect *clip = NULL, float angle = 0.0, int xPivot = 0, int yPivot = 0, SDL_RendererFlip flip = SDL_FLIP_NONE);

		/// <summary>
		/// Gets a texture from a font, message and color.
		/// </summary>
		/// <param name="font">The font.</param>
		/// <param name="message">The message to print.</param>
		/// <param name="color">The color of the message.</param>
		/// <returns>Texture from font with message and color.</returns>
		static PONGENGINE_API SDL_Texture* GetRendererdFont(TTF_Font *font, const std::string &message, SDL_Color color);

		/// <summary>
		/// Loads a image.
		/// </summary>
		/// <param name="file">The image file location.</param>
		/// <returns>A SDL_Texture from the image.</returns>
		static PONGENGINE_API SDL_Texture* LoadImage(const std::string &file);

		/// <summary>
		/// Loads a font with a specified font size.
		/// </summary>
		/// <param name="fontFile">The font file location.</param>
		/// <param name="fontSize">The font size.</param>
		/// <returns>A font that is loaded with a specified font size.</returns>
		static PONGENGINE_API TTF_Font* LoadFont(const std::string &fontFile, int fontSize);

		/// <summary>
		/// Renders a rect.
		/// </summary>
		/// <param name="ren">The renderer to render to.</param>
		/// <param name="rect">The rect that needs to be rendered.</param>
		/// <param name="color">The color of the shape.</param>
		/// <param name="fill">if set to <c>true</c> [fill] the rect is rendered filled.</param>
		static PONGENGINE_API void RenderShape(SDL_Renderer *ren, SDL_Rect rect, SDL_Color color, bool fill);

		/// <summary>
		/// Clears the renderer.
		/// </summary>
		/// <param name="ren">The renderer to clear.</param>
		static PONGENGINE_API void ClearRenderer(SDL_Renderer *ren);

		/// <summary>
		/// Presents the specified renderer.
		/// </summary>
		/// <param name="ren">The renderer to present.</param>
		static PONGENGINE_API void Present(SDL_Renderer *ren);

		/// <summary>
		/// The window size and location in rect.
		/// </summary>
		/// <returns>The window size in rect.</returns>
		static PONGENGINE_API SDL_Rect wRect();

		/// <summary>
		/// Gets the <c>SDL_Window</c>.
		/// </summary>
		/// <returns>The window.</returns>
		static PONGENGINE_API SDL_Window* GetWindow();

		/// <summary>
		/// Gets the <c>SDL_Renderer</c>.
		/// </summary>
		/// <returns>The renderer</returns>
		static PONGENGINE_API SDL_Renderer* GetRenderer();
	private:
		static PONGENGINE_API std::unique_ptr<SDL_Window, void(*)(SDL_Window*)> window;
		static PONGENGINE_API std::unique_ptr<SDL_Renderer, void(*)(SDL_Renderer*)> renderer;
		static PONGENGINE_API SDL_Rect windowRect;
	};
}