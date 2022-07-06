#pragma once
#include "stdafx.h"
#include "Window.h"
#include <string>
#include <vector>
#include <math.h>
#include <SDL_image.h>
#include <iostream>
#include "Input.h"



class Rect{
public:
	Rect(int w, int h, int x, int y, int r, int g, int b, int a);
	Rect(int w, int h, int x, int y, const std::string &image_path);
	
	~Rect();

	void draw() const;

	inline void set_x(int x) { _x2 += x - _x; _x = x; }
	inline void set_y(int y) { _y2 += y - _y; _y = y; }
	inline void set_pos(int x, int y) { set_x(x); set_y(y);}

	inline void set_size(int width, int height) { _w = width; _h = height; _x2 = _x + _w; _y2 = _y + _h; }

	inline int get_x() const { return _x; }
	inline int get_x2() const { return _x2; }
	inline int get_y() const { return _y; }
	inline int get_y2() const { return _y2; }

protected:
	int _w, _h;
	int _x, _y, _x2, _y2;
	int _r, _g, _b, _a;
	SDL_Texture *_texture = nullptr;
};



class Kinematic : public Rect {
public:

	Kinematic(int w, int h, int x, int y, int r, int g, int b, int a) : Rect(w, h, x, y, r, g, b, a)  {};

	~Kinematic() { SDL_DestroyTexture(_texture); }

	void pollEvents(SDL_Event &event);

	void keyboardEvents(unsigned char _key, bool press);

	void input_movement(std::vector<Rect> const&colliders_static, std::vector<Kinematic> const&colliders_kinematic, int moi, bool horizontal, bool vertical);
	void input_movement(std::vector<Kinematic> const&colliders_kinematic, int moi, bool horizontal, bool vertical);
	
	char move(std::vector<Rect> const&colliders_static, std::vector<Kinematic> const&colliders_kinematic, int moi, int dx, int dy);
	char move(std::vector<Kinematic> const&colliders_kinematic, int moi, int dx, int dy);
	inline void move(int dx, int dy){_x += dx; _y += dy; _x2 += dx; _y2 += dy;}


	char bounding_Box(Kinematic const&object, int &dx, int &dy);
	char bounding_Box(Rect const&object, int &dx, int &dy);

	inline void set_input(std::string input) { _input = input; };

	void collide_compare(char tmp_collide, char &collide);

private:
	struct Keyboard key;
	std::string _input = "Arrow";
};

