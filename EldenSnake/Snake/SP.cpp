#include "SP.h"
#include "Game.h"
#include <graphics.h>
#include <time.h>

IMAGE IM_SP;

SP::SP(int et, int ty) :x(0), y(0), exist_time(et), type(ty), start_time(0), is_exist(0), is_shown(0)
{
	loadimage(&IM_SP, _T("bin/image/sp.jpg"), 16, 16);
}

void SP::clear()
{
	is_shown = false;
	is_exist = false;
	start_time = 0;
	exist_time = -10;
	draw(1);
}

void SP::updateTime(Game& game, int limit)
{
	time_t t, delta;
	time(&t);
	delta = t - start_time;
	if (delta != 0) {
		start_time = t;
		if (is_exist) {
			exist_time -= delta;
			if (exist_time <= 0)
				clear();
		}
		else {
			exist_time += delta;
			if (exist_time >= 0)
				create(game, limit);
		}
	}
	if (is_exist)
		flash();
}

void SP::create(Game& game, int time)
{
	if (!is_exist) {
		while (true) {
			int n = rand() % (MAP_A * MAP_A);
			n += (n > MAP_MAX / 2 ? -20 : 20);
			int x1 = n % MAP_A, y1 = n / MAP_A;
			if (game.is_pointEmpty(x1, y1)) {
				game.setPoint(x1, y1, MAP_SP);
				this->x = x1;
				this->y = y1;
				draw();
				break;
			}
		}
		this->exist_time = time;
		this->start_time = game.start_time;
		is_exist = true;
		is_shown = true;
	}
}

void SP::draw(bool clear)
{
	int xPos = x * 16 + 20;
	int yPos = y * 16 + 20;

	if (clear) {
		setfillcolor(BLACK);
		fillrectangle(xPos, yPos, xPos + 16, yPos + 16);
	}
	else {
		putimage(xPos, yPos, &IM_SP);
	}
}

void SP::flash()
{
	if (is_shown) {
		draw(1);
		is_shown = false;
		Sleep(8);
	}
	else {
		draw();
		is_shown = true;
		Sleep(8);
	}
	
}

bool SP::isExist()
{
	return is_exist;
}

