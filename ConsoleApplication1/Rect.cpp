#include "stdafx.h"
#include "Rect.h"

Rect::Rect(int w, int h, int x, int y, int r, int g, int b, int a) :
_w(w), _h(h), _x(x), _y(y), _x2(x + w), _y2(y + h), _r(r), _g(g), _b(b), _a(a)
{}

Rect::Rect(int w, int h, int x, int y, const std::string &image_path) :
_w(w), _h(h), _x(x), _y(y), _x2(x + w), _y2(y + h)
{
	auto surface = IMG_Load(image_path.c_str());
	if (!surface)
	{
		std::cerr << "failed to create surface.\n";
	}
	_texture = SDL_CreateTextureFromSurface(Window::renderer, surface);

	if (!_texture)
	{
		std::cerr << "failed to create texture.\n";
	}
	SDL_FreeSurface(surface);
}

Rect::~Rect() 
{
	SDL_DestroyTexture(_texture);
}

void Rect::draw() const
{
	SDL_Rect rect = { _x, _y ,_w, _h };
	if (_texture)
	{
		SDL_RenderCopy(Window::renderer, _texture, nullptr, &rect);
	}else{
		
		SDL_SetRenderDrawColor(Window::renderer, _r, _g, _b, _a);
		SDL_RenderFillRect(Window::renderer, &rect);
	}
}


void Kinematic::pollEvents(SDL_Event &event)
{
	bool keyboard = false;
	bool type;
	if (event.type == SDL_KEYDOWN)
	{
		keyboard = true;
		type = true;

	}else if (event.type == SDL_KEYUP)
	{
		keyboard = true;
		type = false;
	}

	if (keyboard == true)
	{
		if (_input == "Arrow") {
			switch (event.key.keysym.sym)
			{
			case SDLK_LEFT:
				key.left = type;
				break;
			case SDLK_RIGHT:
				key.right = type;
				break;
			case SDLK_UP:
				key.up = type;
				break;
			case SDLK_DOWN:
				key.down = type;
				break;
			}
		}
		else if (_input == "AZERTY") {
			switch (event.key.keysym.sym)
			{
			case SDLK_q:
				key.left = type;
				break;
			case SDLK_d:
				key.right = type;
				break;
			case SDLK_z:
				key.up = type;
				break;
			case SDLK_s:
				key.down = type;
				break;
			}
		}

	}
}

void Kinematic::keyboardEvents(unsigned char _key, bool press)
{
	switch(_key)
	{
		case 1:
			key.up = press;
			break;
		case 2:
			key.down = press;
			break;
		case 3:
			key.right = press;
			break;
		case 4:
			key.left = press;
			break;
	}
}

void Kinematic::input_movement(std::vector<Rect> const&colliders_static, std::vector<Kinematic> const&colliders_kinematic, int moi, bool horizontal, bool vertical)
{
	//std::cout << "ok" << std::endl;
	int dx = 0;
	int dy = 0;
	if(horizontal == true)
	{
		if (key.right == true)
		{
			dx += 1;
		}

		if (key.left == true)
		{
			dx -= 1;
		}
	}

	if (vertical == true)
	{
		if (key.up == true)
		{
			dy -= 1;
		}

		if (key.down == true)
		{
			dy += 1;
		}
	}
	dx *= 5;
	dy *= 5;
	move(colliders_static, colliders_kinematic, moi, dx, dy);
}

void Kinematic::input_movement(std::vector<Kinematic> const&colliders_kinematic, int moi, bool horizontal, bool vertical)
{
	//std::cout << "ok" << std::endl;
	int dx = 0;
	int dy = 0;
	if (horizontal == true)
	{
		if (key.right == true)
		{
			dx += 1;
		}

		if (key.left == true)
		{
			dx -= 1;
		}
	}

	if (vertical == true)
	{
		if (key.up == true)
		{
			dy -= 1;
		}

		if (key.down == true)
		{
			dy += 1;
		}
	}
	dx *= 5;
	dy *= 5;
	move(colliders_kinematic, moi, dx, dy);
}

char Kinematic::move(std::vector<Rect> const&colliders_static, std::vector<Kinematic> const&colliders_kinematic, int moi, int dx, int dy)
{

	char collide = 0;
	char tmp_collide = 0;
	//Tests de collision
	if (colliders_static.size() != 0)
	{
		for (unsigned short i = 0; i < colliders_static.size(); i++)
		{
			tmp_collide = bounding_Box(colliders_static[i], dx, dy);
			collide_compare(tmp_collide, collide);
		}
	}

	if (colliders_kinematic.size() != 0)
	{
		for (unsigned short i = 0; i < colliders_kinematic.size(); i++)
		{
			if (i != moi)
			{
				tmp_collide = bounding_Box(colliders_kinematic[i], dx, dy);
				collide_compare(tmp_collide, collide);
			}
		}
	}

	_x += dx;
	_y += dy;
	_x2 += dx;
	_y2 += dy;

	//Si conditions collision detectee
	return collide;

}

char Kinematic::move(std::vector<Kinematic> const&colliders_kinematic, int moi, int dx, int dy)
{

	char collide = 0;
	char tmp_collide = 0;
	//Tests de collision
	if (colliders_kinematic.size() != 0)
	{
		for (unsigned short i = 0; i < colliders_kinematic.size(); i++)
		{
			if (i != moi)
			{
				tmp_collide = bounding_Box(colliders_kinematic[i], dx, dy);
				collide_compare(tmp_collide, collide);
			}
		}
	}

	_x += dx;
	_y += dy;
	_x2 += dx;
	_y2 += dy;

	//Si conditions collision detectee
	return collide;

}

void Kinematic::collide_compare(char tmp_collide, char &collide)
{
	switch (tmp_collide)
	{
	case 0:
		return;
	case 'x':
		switch (collide)
		{
		case 0:
			collide = 'x';
			return;
		case 'x':
			return;
		case 'y':
			collide = 2;
			return;
		case 2:
			return;
		}
		break;
	case 'y':
		switch (collide)
		{
		case 0:
			collide = 'y';
			return;
		case 'x':
			collide = 2;
			return;
		case 'y':
			return;
		case 2:
			return;
		}
		break;
	case 2:
		return;
	}
}

char Kinematic::bounding_Box(Rect const&object, int &dx, int &dy)
{
	if (_x2 + dx <= object.get_x())
	{
		return 0;
	}

	if (_x + dx >= object.get_x2())
	{
		return 0;
	}

	if (_y2 + dy <= object.get_y())
	{
		return 0;
	}

	if (_y + dy >= object.get_y2())
	{
		return 0;
	}



	int tmp_dx = dx;
	int tmp_dy = dy;
	if (dx != 0)
	{
		if (dx > 0)
		{
			tmp_dx = _x2 + dx - object.get_x();
		}
		else {
			tmp_dx = _x + dx - object.get_x2();
		}
	}

	if (dy != 0)
	{
		if (dy > 0)
		{
			tmp_dy = _y2 + dy - object.get_y();
		}
		else {
			tmp_dy = _y + dy - object.get_y2();
		}
	}

	if (dx != 0 && dy != 0)
	{
		if (abs(tmp_dy) < abs(tmp_dx))
		{
			dy -= tmp_dy;
			return 'y';
		}
		else if (abs(tmp_dx) < abs(tmp_dy))
		{
			dx -= tmp_dx;
			return 'x';
		}
		else {
			dx -= tmp_dx;
			dy -= tmp_dy;
			return 2;
		}
	}
	else {
		if (dx != 0)
		{
			dx -= tmp_dx;
			return 'x';
		}
		else if (dy != 0)
		{
			dy -= tmp_dy;
			return 'y';
		}
	}

	return 0;
}

char Kinematic::bounding_Box(Kinematic const&object, int &dx, int &dy)
{
	if (_x2 + dx <= object.get_x())
	{
		return 0;
	}

	if (_x + dx >= object.get_x2())
	{
		return 0;
	}

	if (_y2 + dy <= object.get_y())
	{
		return 0;
	}

	if (_y + dy >= object.get_y2())
	{
		return 0;
	}



	int tmp_dx = dx;
	int tmp_dy = dy;
	if (dx != 0)
	{
		if (dx > 0)
		{
			tmp_dx = _x2 + dx - object.get_x();
		}
		else {
			tmp_dx = _x + dx - object.get_x2();
		}
	}

	if (dy != 0)
	{
		if (dy > 0)
		{
			tmp_dy = _y2 + dy - object.get_y();
		}
		else {
			tmp_dy = _y + dy - object.get_y2();
		}
	}

	if (dx != 0 && dy != 0)
	{
		if (abs(tmp_dy) < abs(tmp_dx))
		{
			dy -= tmp_dy;
			return 'y';
		}
		else if (abs(tmp_dx) < abs(tmp_dy))
		{
			dx -= tmp_dx;
			return 'x';
		}
		else {
			dx -= tmp_dx;
			dy -= tmp_dy;
			return 2;
		}
	}
	else {
		if (dx != 0)
		{
			dx -= tmp_dx;
			return 'x';
		}
		else if (dy != 0)
		{
			dy -= tmp_dy;
			return 'y';
		}
	}

	return 0;
}

