#include "stdafx.h"
#include "Input.h"

void InputHandler::pollEvents(SDL_Event event)
{
	bool keyboardEvent = false;
	bool type;
	if (event.type == SDL_KEYDOWN)
	{
		keyboardEvent = true;
		type = true;

	}else if (event.type == SDL_KEYUP)
	{
		keyboardEvent = true;
		type = false;
	}

	if (keyboardEvent == true)
	{
		if (_input == true) {
			switch (event.key.keysym.sym)
			{
			case SDLK_LEFT:
				keyboard.left = type;
				break;
			case SDLK_RIGHT:
				keyboard.right = type;
				break;
			case SDLK_UP:
				keyboard.up = type;
				break;
			case SDLK_DOWN:
				keyboard.down = type;
				break;
			}
		}else if (_input == false) {

			switch (event.key.keysym.sym)
			{
			case SDLK_q:
				keyboard.left = type;
				break;
			case SDLK_d:
				keyboard.right = type;
				break;
			case SDLK_z:
				keyboard.up = type;
				break;
			case SDLK_s:
				keyboard.down = type;
				break;
			}
		}

	}else if(event.type == SDL_MOUSEMOTION){
		mouse.x = event.motion.x;
		mouse.y = event.motion.y;
	}
}

