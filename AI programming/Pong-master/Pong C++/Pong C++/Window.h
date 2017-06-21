#include <SDL.h>
#include <string>
#include <memory>
class Window
{
public:
	Window();
	~Window();

	static void Init(std::string title = "Window", int width = 640, int height = 480);
	static void Quit();
	static void Draw(SDL_Texture *tex, SDL_Rect &dstRect, SDL_Rect *clip = NULL, float angle = 0.0, int xPivot = 0, int yPivot = 0, SDL_RendererFlip flip = SDL_FLIP_NONE);
	static SDL_Texture* LoadImage(const std::string &file);
	static void RenderShape(SDL_Rect rect, bool fill);
	static void Clear();
	static void Present();
	static SDL_Rect Box();

private:
	static std::unique_ptr<SDL_Window, void(*)(SDL_Window*)> mWindow;
	static std::unique_ptr<SDL_Renderer, void(*)(SDL_Renderer*)> mRenderer;
	static SDL_Rect mBox;
};

