#pragma once
#include "stdafx.h"
#include <SDL_ttf.h>
#include <SDL.h>
#include <string>
#include <iostream>


class Text
{
public:
	Text(SDL_Renderer *renderer,const std::string &message_text, SDL_Color _color, TTF_Font *police);

	~Text();

	void display(int x, int y, SDL_Renderer *renderer) const;

	static SDL_Texture *loadFont(SDL_Renderer *renderer, const std::string &font_path, int font_size, const std::string &message_text, const SDL_Color &color);

	void change_text(std::string const&message_text, SDL_Renderer *renderer);

private:
	mutable SDL_Rect _text_rect;
	SDL_Surface *_surface;
	SDL_Color _color;
	TTF_Font *_police;
	SDL_Texture *_text_text;
};
