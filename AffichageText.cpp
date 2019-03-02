#pragma once

#include "AffichageText.h"

SDL_Color textColor = { 255, 255, 255, 255 }; // white
TTF_Font* font_9 = nullptr;
TTF_Font* font_12 = nullptr;

bool Text::Init()
{
	// Load font
	font_9 = TTF_OpenFont("font.ttf", 9);
	if (font_9 == nullptr)
	{
		std::cout << " Failed to load font : " << SDL_GetError() << std::endl;
		return false;
	}
	// Load font
	font_12 = TTF_OpenFont("font.ttf", 12);
	if (font_12 == nullptr)
	{
		std::cout << " Failed to load font : " << SDL_GetError() << std::endl;
		return false;
	}
	return true;
}

SDL_Texture* Text::CreateTextTexture(SDL_Renderer *renderer, const char *text, bool grand)
{
	SDL_Surface* text_surface = text_surface = TTF_RenderText_Solid(font_9, text, textColor);

	if (grand)
	{
		SDL_FreeSurface(text_surface);
		text_surface = TTF_RenderText_Solid(font_12, text, textColor);
	}

	if (!text_surface)
		std::cout << "failed to create text surface" << std::endl;

	SDL_Texture* text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);
	if (!text_texture)
		std::cout << "failed to create text texture" << std::endl;

	SDL_FreeSurface(text_surface);
	text_surface = 0;

	return text_texture;
}

void Text::DisplayText(SDL_Renderer *renderer, SDL_Texture* text_texture, Vec2 position)
{
	SDL_Rect text_rect;

	SDL_QueryTexture(text_texture, nullptr, nullptr, &text_rect.w, &text_rect.h);

	text_rect.x = position.xint();
	text_rect.y = position.yint();
	SDL_RenderCopy(renderer, text_texture, nullptr, &text_rect);
}

void Text::Clean()
{
	TTF_CloseFont(font_9);
	TTF_CloseFont(font_12);
}