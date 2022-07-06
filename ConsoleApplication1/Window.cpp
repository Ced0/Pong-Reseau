#include "stdafx.h"
#include "Window.h"

SDL_Renderer *Window::renderer = nullptr;

Window::Window(const std::string &title, int width, int height) :
	_title(title), _width(width), _height(height)
{
	_closed = !init();
}

Window::~Window()
{
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(_window);


	/*if (_sound_effect.size()!= 0) {
		for (unsigned char i = 0; i < 3; i++)//_sound_effect.size(); i++)
		{
			Mix_FreeChunk(_sound_effect[i]);
		}
	}*/
	Mix_CloseAudio();
	if (_police)
	{
		TTF_CloseFont(_police);
	}
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}

bool Window::init()
{
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		std::cerr << "Failed to initialise SDL.\n";
		return 0;
	}

	if (IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG)
	{
		std::cerr << "Failed to initialise SDL_Image.\n";
		return 0;
	}

	if (TTF_Init() == -1) {
		std::cerr << "Failed to initialise SDL_ttf.\n";
		return 0;
	}

	if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) == -1)
	{
		return false;
	}

	_window = SDL_CreateWindow(
		_title.c_str(),
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		_width, _height,
		0
	);

	if (_window == nullptr)
	{
		std::cerr << "Failed to create window.\n";
		return 0;
	}

	renderer = SDL_CreateRenderer(_window, -1, SDL_RENDERER_PRESENTVSYNC);

	if (renderer == nullptr)
	{
		std::cerr << "Failed to create renderer.\n";
		return 0;
	}

	return true;
}

void Window::pollEvents(SDL_Event &event)
{

	switch (event.type) {
	case SDL_QUIT:
		_closed = true;
		break;

	case SDL_KEYDOWN:
		//std::cout << "a button was clicked on the keyboard.\n";
		switch (event.key.keysym.sym) {
		case SDLK_ESCAPE:
			_closed = true;
			break;
		}
		break;

	default:
		break;
	}
}

void Window::clear() const {
	SDL_RenderPresent(renderer);
	SDL_SetRenderDrawColor(renderer,0,0,200,255);
	SDL_RenderClear(renderer);
}
