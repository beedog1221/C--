#include "Background.h"

void Background::render(SDL_Renderer* ren)
{
	PongEngine::Drawer::RenderShape(ren, rect, color, true);
}