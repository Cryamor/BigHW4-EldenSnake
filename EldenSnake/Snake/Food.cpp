#include "Food.h"
#include "Game.h"



Food::Food(int max) :maxNum(max) 
{
	loadimage(&IM_FOOD, _T("bin/image/luen.png"), 16, 16);
}

Food::~Food()
{
	clear();
}

void Food::clear()
{
	deq.clear();
}

void Food::create(Game& game)
{
	if (getNum() < maxNum) {
		while (true) {
			int n = rand() % MAP_MAX;
			int x = n % MAP_A, y = n / MAP_A;
			if (game.is_pointEmpty(x, y)) {
				game.setPoint(x, y, MAP_FOOD);
				draw(x, y);
				POINT p = { x,y };
				deq.push_back(p);
				break;
			}
		}
	}
}

void Food::draw(int x, int y, bool clear)
{
	int xPos = x * 16 + 20;
	int yPos = y * 16 + 20;

	if (clear) {
		setfillcolor(BLACK);
		fillrectangle(xPos, yPos, xPos + 16, yPos + 16);
	}
	else {
		putimage(xPos, yPos, &IM_FOOD);
	}
}

void Food::draw()
{
	for (std::deque<POINT>::iterator it = deq.begin(); it != deq.end(); it++) {
		draw(it->x, it->y);
	}
}

int Food::getNum() const
{
	return (int)deq.size();
}
