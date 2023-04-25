#pragma once
#include <deque>
#include <graphics.h>

class Game;
class Snake;
class Bomb
{
private:
	std::deque<POINT> deq;
	int maxNum;
	IMAGE IM_BOMB;
public:
	Bomb(int max = 5);
	~Bomb();
	void clear();

	void create(Game& game);
	void draw(int x, int y, bool clear = false);
	void draw();
	int getNum() const;

	friend class Snake;
};

