#include "Bomb.h"
#include "Game.h"



Bomb::Bomb(int max) :maxNum(max)
{
	loadimage(&IM_BOMB, _T("bin/image/dian.jpg"), 16, 16);
}

Bomb::~Bomb()
{
	clear();
}

void Bomb::clear()
{
	deq.clear();
}

void Bomb::create(Game& game)
{
	if (getNum() < maxNum) {
		while (true) {
			int n = rand() % MAP_MAX;
			n += (n > MAP_MAX / 2 ? -50 : 50);
			int x = n % MAP_A, y = n / MAP_A;
			if (game.is_pointEmpty(x, y)) {
				game.setPoint(x, y, MAP_BOMB);
				draw(x, y);
				POINT p = { x,y };
				deq.push_back(p);
				break;
			}
		}
	}
}

void Bomb::draw(int x, int y, bool clear)
{
	int xPos = x * 16 + 20;
	int yPos = y * 16 + 20;

	if (clear) {
		setfillcolor(BLACK);
		fillrectangle(xPos, yPos, xPos + 16, yPos + 16);
	}
	else {
		putimage(xPos, yPos, &IM_BOMB);
	}
}

void Bomb::draw()
{
	for (std::deque<POINT>::iterator it = deq.begin(); it != deq.end(); it++) {
		draw(it->x, it->y);
	}
}

int Bomb::getNum() const
{
	return (int)deq.size();
}
