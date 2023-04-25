#pragma once

#include "Snake.h"

class Game;
class Snake;
class SP
{
private:
	int x;
	int y;
	time_t start_time;
	time_t exist_time; //<0表示当前不存在
	int type;
	bool is_exist;
	bool is_shown;
public:
	SP(int et = 15, int ty = 1);
	void clear();

	void updateTime(Game& game, int limit);
	void create(Game& game, int time);
	void draw(bool clear = false);
	void flash();
	bool isExist();

	friend class Snake;
};

