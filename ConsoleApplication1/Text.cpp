#include "stdafx.h"
#include "Text.h"

Text::Text(SDL_Renderer *renderer,const std::string &message_text,SDL_Color color,TTF_Font *police):
_color(color), _police(police)
{
	_surface = TTF_RenderText_Blended(police, message_text.c_str(), color);
	_text_text = SDL_CreateTextureFromSurface(renderer, _surface);
	SDL_FreeSurface(_surface);

}

Text::~Text() 
{
	SDL_DestroyTexture(_text_text);
}

void Text::display(int x, int y, SDL_Renderer *renderer) const
{
	_text_rect.x = x;
	_text_rect.y = y;
	SDL_QueryTexture(_text_text, nullptr, nullptr, &_text_rect.w, &_text_rect.h);
	SDL_RenderCopy(renderer, _text_text, NULL, &_text_rect);
}

SDL_Texture *Text::loadFont(SDL_Renderer *renderer, const std::string &font_path, int font_size, const std::string &message_text, const SDL_Color &color)
{
	TTF_Font *font = TTF_OpenFont(font_path.c_str(), font_size);
	if (!font)
	{
		std::cerr << "failed to load font.\n";
	}
	auto text_surface = TTF_RenderText_Solid(font, message_text.c_str(), color);
	if (!text_surface)
	{
		std::cerr << "failed to create surface.\n";
	}
	auto text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);
	if (!text_texture) {
		std::cerr << "failed to create text texture.\n";
	}
	SDL_FreeSurface(text_surface);
	return text_texture;
}

void Text::change_text(std::string const&message_text,SDL_Renderer *renderer)
{
	_surface = TTF_RenderText_Blended(_police, message_text.c_str(), _color);
	_text_text = SDL_CreateTextureFromSurface(renderer, _surface);
	SDL_FreeSurface(_surface);
}
