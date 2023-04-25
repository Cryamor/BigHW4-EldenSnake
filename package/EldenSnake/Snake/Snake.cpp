#include "Snake.h"
#include "Game.h"
#include <iostream>
#include <conio.h>
#include <cmath>

IMAGE IM_LIFE1, IM_LIFE2;

//evil snkae
Snake::Snake(POINT p, int dire) :len(3), HP(16), dir(dire), life(0)
{
	loadimage(&IM_SNAKE, _T("bin/image/lightn.jpg"), 16, 16);
	loadimage(&IM_HEAD, _T("bin/image/lightn.jpg"), 16, 16);
	body.push_back(p);
	switch (dir) {
		case DIR_LEFT:
			for (int i = 0; i < 2; i++) {
				p.x++;
				body.push_back(p);
			}
			break;
		case DIR_RIGHT:
			for (int i = 0; i < 2; i++) {
				p.x--;
				body.push_back(p);
			}
			break;
		case DIR_UP:
			for (int i = 0; i < 2; i++) {
				p.y++;
				body.push_back(p);
			}
			break;
		case DIR_DOWN:
			for (int i = 0; i < 2; i++) {
				p.y--;
				body.push_back(p);
			}
			break;
	}
}

Snake::Snake(int l, int hp, int lf, int x, int y, int color)
{
	set(l, hp, lf, x, y);
	loadimage(&IM_LIFE1, _T("bin/image/life1.jpg"), 25, 25);
	loadimage(&IM_LIFE2, _T("bin/image/life2.jpg"), 25, 25);
	loadimage(&IM_SNAKE, _T("bin/image/body.jpg"), 16, 16);
	switch (color) {
		case 0:
			loadimage(&IM_HEAD, _T("bin/image/head.jpg"), 16, 16);
			break;
		case 1:
			loadimage(&IM_HEAD, _T("bin/image/head1.jpg"), 16, 16);
			break;
		case 2:
			loadimage(&IM_HEAD, _T("bin/image/head2.jpg"), 16, 16);
			break;
	}	
}

Snake::~Snake()
{
	clear();
}

void Snake::clear()
{
	body.clear();
}

void Snake::set(int l, int hp, int lf, int x, int y)
{
	len = l;
	HP = hp;
	life = lf;

	dir = rand() % 4;
	POINT p = { x,y };
	body.push_back(p);
	switch (dir) {
		case DIR_LEFT:
			for (int i = 0; i < l - 1; i++) {
				p.x++;
				body.push_back(p);
			}
			break;
		case DIR_RIGHT:
			for (int i = 0; i < l - 1; i++) {
				p.x--;
				body.push_back(p);
			}
			break;
		case DIR_UP:
			for (int i = 0; i < l - 1; i++) {
				p.y++;
				body.push_back(p);
			}
			break;
		case DIR_DOWN:
			for (int i = 0; i < l - 1; i++) {
				p.y--;
				body.push_back(p);
			}
			break;
	}

}

//init snake into map, pic into UI
void Snake::setInMap(Game& game)
{
	for (std::list<POINT>::iterator it = body.begin(); it != body.end(); it++) {
		setPoint(game, it->x, it->y);
	}
	drawSnake();
}

// 0 - hitwall
int Snake::move(Game& game)
{
	if (body.empty())
		return 0;
	POINT p = body.front(), p0 = body.front();
	switch (dir) {
		case DIR_UP:
			p.y--;
			break;
		case DIR_DOWN:
			p.y++;
			break;
		case DIR_LEFT:
			p.x--;
			break;
		case DIR_RIGHT:
			p.x++;
			break;
	}

	if (hitWall(game, p.x, p.y)||hitSelf(game, p.x, p.y)) {
		return 0;
	}
	//画蛇头
	body.push_front(p);
	setPoint(game, p.x, p.y);
	draw(p.x, p.y, 1);
	draw(p0.x, p0.y);  //原蛇头变为蛇身

	//清除蛇尾
	p = body.back();
	body.pop_back();
	clearPoint(game, p.x, p.y);
	draw(p.x, p.y, 1, 1);
	return 1;
}

bool Snake::eatFood(Food& food)
{
	int x = getHeadX(), y = getHeadY();
	std::deque<POINT>::iterator it = food.deq.begin();
	for (; it != food.deq.end(); it++) {
		if (it->x == x && it->y == y) {
			food.deq.erase(it);
			return true;
		}
	}
	return false;
}

bool Snake::eatSP(SP& sp)  
{
	int x = getHeadX(), y = getHeadY();
	if (x == sp.x && y == sp.y && sp.is_exist) {
		sp.is_exist = false;
		sp.exist_time = -10;
		return true;
	}
		
	return false;
}

bool Snake::hitBomb(Bomb& bomb)
{
	int x = getHeadX(), y = getHeadY();
	std::deque<POINT>::iterator it = bomb.deq.begin();
	for (; it != bomb.deq.end(); it++) {
		if (it->x == x && it->y == y) {
			bomb.deq.erase(it);
			return true;
		}
	}
	return false;
}

void Snake::clearPoint(Game& game, int x, int y)
{
	game.map[y * MAP_A + x] = MAP_VOID;
}

void Snake::setPoint(Game& game, int x, int y)
{
	game.map[y * MAP_A + x] = MAP_SNAKE;
}

/*0-normal(purple) 1-yellow 2-red*/
void Snake::changeHead(int type)
{
	switch (type) {
		case 0:
			loadimage(&IM_HEAD, _T("bin/image/head.jpg"), 16, 16);
			break;
		case 1:
			loadimage(&IM_HEAD, _T("bin/image/head1.jpg"), 16, 16);
			break;
		case 2:
			loadimage(&IM_HEAD, _T("bin/image/head2.jpg"), 16, 16);
			break;
	}
	draw(getHeadX(), getHeadY(), 1);
}

void Snake::longer(Game& game)
{
	POINT p1, p2;
	p1 = body.back();
	body.pop_back();
	p2 = body.back();  //倒数第二节
	body.push_back(p1);
	
	int d;
	if (p1.y == p2.y) {
		d = p1.x - p2.x;
		p1.x += d;
		if (!is_pointEmpty(game, p1.x, p1.y)) {
			p1.x -= d;
			if (is_pointEmpty(game, p1.x, p1.y + 1))
				p1.y++;
			else
				p1.y--;
		}		
	}
	else {
		d = p1.y - p2.y;
		p1.y += d;
		if (!is_pointEmpty(game, p1.x, p1.y)) {
			p1.y -= d;
			if (is_pointEmpty(game, p1.x + 1, p1.y))
				p1.x++;
			else
				p1.x--;
		}
	}
	setPoint(game, p1.x, p1.y);
	draw(p1.x, p1.y);
	body.push_back(p1);
	len++;
}

bool Snake::is_pointEmpty(Game& game, int x, int y)
{
	return game.map[y * MAP_A + x] == 0;
}

bool Snake::hitWall(Game& game, int x, int y)
{
	return game.map[y * MAP_A + x] == MAP_WALL;
}

bool Snake::hitSelf(Game& game, int x, int y)
{
	//前方一格为蛇尾，不算自撞
	if (getTailX() == x && getTailY() == y)
		return false;
	else
		return game.map[y * MAP_A + x] == MAP_SNAKE;
}

void Snake::revDir()
{
	if (body.size() < 2)
		return;
	POINT phead = body.front();
	body.pop_front();
	POINT p1 = body.front();
	switch (dir) {
		case DIR_UP:
			if (phead.y + 1 == p1.y) {
				dir = DIR_LEFT;
			}
			else
				dir = DIR_DOWN;
			break;
		case DIR_DOWN:
			if (phead.y - 1 == p1.y) {
				dir = DIR_LEFT;
			}
			else
				dir = DIR_UP;
			break;
		case DIR_LEFT:
			if (phead.x + 1 == p1.x)
				dir = DIR_UP;
			else
				dir = DIR_RIGHT;
			break;
		case DIR_RIGHT:
			if (phead.x - 1 == p1.x)
				dir = DIR_UP;
			else
				dir = DIR_LEFT;
			break;
	}
	body.push_front(phead);
}

/*ESC暂停 返回0 正常情况返回1 特殊情况返回2*/
int Snake::changeDir()
{
	int ch1, ch2;
	while (_kbhit()) {
		ch1 = _getch();
		switch (ch1) {
			case 224:
				ch2 = _getch();
				switch (ch2) {
					case 72:
						if (dir != DIR_DOWN)  //反方向不变
							dir = DIR_UP;
						break;
					case 80:
						if (dir != DIR_UP)
							dir = DIR_DOWN;
						break;
					case 75:
						if (dir != DIR_RIGHT)
							dir = DIR_LEFT;
						break;
					case 77:
						if (dir != DIR_LEFT)
							dir = DIR_RIGHT;
						break;
				}
				return 1;
			case 'W':
			case 'w':
				if (dir != DIR_DOWN)
					dir = DIR_UP;
				return 1;
			case 'A':
			case 'a':
				if (dir != DIR_RIGHT)
					dir = DIR_LEFT;
				return 1;
			case 'S':
			case 's':
				if (dir != DIR_UP)
					dir = DIR_DOWN;
				return 1;
			case 'D':
			case 'd':
				if (dir != DIR_LEFT)
					dir = DIR_RIGHT;
				return 1;
			case 'f':
			case 'F':
				return 2;
			case 27: //esc
				return 0;
		}
	}

	return 1;
}

bool Snake::is_dead()
{
	return HP == 0;
}

bool Snake::is_gameOver()
{
	return life == 0;
}

void Snake::halfCut(Game& game)
{
	int clen = this->len / 2;
	for (int i = 0; i < clen; i++) {
		POINT p = body.back();
		game.setPoint(p.x, p.y, MAP_VOID);
		game.showPointVoid(p.x, p.y);
		body.pop_back();
		this->len--;
	}
}

void Snake::loseLife()
{
	int hp = HP;
	//--life;
	HP = 0;
	updateHP(hp, HP);
	updateLife(-1);
}

void Snake::loseHP(int delta)
{
	int hp_before = this->HP;
	this->HP -= delta;
	if (HP < 0)
		HP = 0;
	updateHP(hp_before, HP);
}

void Snake::turnToWall(Game& game)
{
	while (!body.empty()) {
		POINT p = body.back();
		body.pop_back();
		game.setPoint(p.x, p.y, MAP_WALL);
		game.showPointWall(p.x, p.y);
	}
	reborn(game);
}

void Snake::turnToFood(Game& game, Food& food)
{
	while (!body.empty()) {
		POINT p = body.back();
		body.pop_back();
		game.setPoint(p.x, p.y, MAP_FOOD);
		food.deq.push_back(p);
		food.draw(p.x, p.y);
	}
	reborn(game);
}

void Snake::turnToVoid(Game& game)
{
	while (!body.empty()) {
		POINT p = body.back();
		body.pop_back();
		game.setPoint(p.x, p.y, MAP_VOID);
		game.showPointVoid(p.x, p.y);
	}
	//reborn(game);
}

void Snake::reborn(Game& game)
{
	set(3, 16, life);
	for (std::list<POINT>::iterator it = body.begin(); it != body.end(); it++) {
		setPoint(game, it->x, it->y);
	}
	drawSnake();
}

int Snake::getHeadX() const
{
	return body.front().x;
}

int Snake::getHeadY() const
{
	return body.front().y;
}

int Snake::getTailX() const
{
	return body.back().x;
}

int Snake::getTailY() const
{
	return body.back().y;
}

void Snake::draw(int x, int y, bool is_head, bool clear)
{
	int xPos = x * 16 + 20;
	int yPos = y * 16 + 20;

	setfillcolor(BK_COLOR);
	setlinecolor(BK_COLOR);

	if (clear) {
		fillrectangle(xPos, yPos, xPos + 16, yPos + 16);
	}
	else {
		if (is_head) {
			IMAGE IM_RO;
			switch (dir) {
				case DIR_UP:
					rotateimage(&IM_RO, &IM_HEAD, 0);
					break;
				case DIR_DOWN:
					rotateimage(&IM_RO, &IM_HEAD, PI);
					break;
				case DIR_LEFT:
					rotateimage(&IM_RO, &IM_HEAD, PI / 2);
					break;
				case DIR_RIGHT:
					rotateimage(&IM_RO, &IM_HEAD, -PI / 2);
					break;
			}
			putimage(xPos, yPos, &IM_RO);
		}
		else
			putimage(xPos, yPos, &IM_SNAKE);
	}
}

void Snake::drawSnake(bool clear)
{
	std::list<POINT>::iterator it = body.begin();
	draw(it->x, it->y, 1);

	for (++it; it != body.end(); it++) {
		draw(it->x, it->y);
	}
}

void Snake::drawHP(double hp)
{
	setfillcolor(RGB(116, 49, 33));
	setlinecolor(BLACK);

	int max_len = 880 - 640 - 4;
	int now_len = int(hp * max_len / 16);

	fillrectangle(642, 162, 642 + now_len, 178);

	setfillcolor(BLACK);
	fillrectangle(642 + now_len, 162, 878, 178);
}

void Snake::drawHP()
{
	setfillcolor(BLACK);
	setlinecolor(DARKGRAY);
	fillrectangle(640, 160, 880, 180);

	setfillcolor(RGB(116, 49, 33));
	setlinecolor(BLACK);

	int max_len = 880 - 640 - 4;
	int now_len = HP * max_len / 16;

	fillrectangle(642, 162, 642 + now_len, 178);

	setfillcolor(BLACK);
	fillrectangle(642 + now_len, 162, 878, 178);
}

void Snake::updateHP(int before, int after)
{
	double hp = before;
	if (hp >= after) {
		while (fabs(hp-after) > 1e-6) {
			drawHP(hp);
			hp -= 0.25;
			Sleep(2);
		}
	}
	else {
		while (fabs(hp - after) > 1e-6) {
			drawHP(hp);
			hp += 0.25;
			Sleep(2);
		}
	}
}

void Snake::drawLife()
{
	const int d = 35;
	const int y = 120;
	for (int i = 0; i < this->life; i++) {
		int xPos = 640 + i * d;
		putimage(xPos, y, &IM_LIFE1);
	}
}

/*进行生命值的变化（加减）并绘制*/
void Snake::updateLife(int add)
{
	const int d = 35;
	const int y = 120;

	if (add > 0) {
		for (int i = 0; i < add; i++) {
			int xPos = 640 + this->life * d;
			putimage(xPos, y, &IM_LIFE1);
			this->life++;
		}
	}
	else {
		for (int i = 0; i < -add; i++) {
			int xPos = 640 + (this->life - 1) * d;
			putimage(xPos, y, &IM_LIFE2);
			this->life--;
		}
	}
}
