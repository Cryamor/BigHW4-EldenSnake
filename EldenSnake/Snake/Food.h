#pragma once
#include <deque>
#include "Snake.h"

class Game;
class Snake;
class Food
{
private:
	std::deque<POINT> deq;
	int maxNum;
	IMAGE IM_FOOD;
public:
	Food(int max = 5);
	~Food();
	void clear();
	void create(Game& game);
	void draw(int x, int y, bool clear = false);
	void draw();
	int getNum() const;

	friend class Snake;
};

