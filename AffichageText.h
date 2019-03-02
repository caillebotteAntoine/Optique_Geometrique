#pragma once

#include "SDL.h"
#include "SDL_ttf.h"
#include "Vecteur2.h"

class Text
{
public:
	static bool Init();

	static SDL_Texture* CreateTextTexture(SDL_Renderer *renderer, const char *text, bool grand);

	static void DisplayText(SDL_Renderer *renderer, SDL_Texture* text_texture, Vec2 position);
	static void Clean();
};
