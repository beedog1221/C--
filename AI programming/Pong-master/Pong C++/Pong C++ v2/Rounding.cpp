#include "Rounding.h"

void Rounding::render(SDL_Renderer* ren)
{
	PongEngine::Drawer::RenderShape(ren, rect, color, false);
}