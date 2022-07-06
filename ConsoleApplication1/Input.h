#pragma once
#include "stdafx.h"
#include <SDL.h>

struct Keyboard{
	bool up = false; 
	bool down = false;
	bool right = false;
	bool left = false;
};

struct Mouse{
	unsigned int y = 0;
	unsigned int x = 0;
};

class InputHandler{
public:
	inline InputHandler(){}

	inline ~InputHandler(){}

	void pollEvents(SDL_Event event);

	inline struct Keyboard getKeyboard(){return keyboard;}

	inline struct Mouse getMouse(){return mouse;}

	inline void setInput(bool set){_input = set;}

private:
	struct Keyboard keyboard;
	struct Mouse mouse;
	bool _input = false;
};

