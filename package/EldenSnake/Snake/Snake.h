#pragma once
#include <list>
#include <graphics.h>
#include "Food.h"
#include "SP.h"
#include "Bomb.h"

#define   DIR_UP           0
#define   DIR_DOWN         1
#define   DIR_LEFT         2
#define   DIR_RIGHT        3


class Game;
class Food;
class SP;

class Snake
{
private:
	int len;
	int HP;
	int dir;
	int life;
	std::list<POINT>body;
	IMAGE IM_HEAD;
	IMAGE IM_SNAKE;
public:
	Snake(POINT p, int dire);
	Snake(int l = 3, int hp = 16, int lf = 3, int x = 17, int y = 17, int color = 0);
	~Snake();
	void clear();
	void set(int l = 3, int hp = 16, int lf = 3, int x = 17, int y = 17);

	void setInMap(Game& game);
	int move(Game& game);
	void longer(Game& game);

	bool eatFood(Food& food);
	bool eatSP(SP& sp);
	bool hitBomb(Bomb& bomb);
	bool hitWall(Game& game, int x, int y);
	bool hitSelf(Game& game, int x, int y);
	void revDir();
	int changeDir();
	bool is_dead();
	bool is_gameOver();

	void halfCut(Game& game);
	void loseLife();
	void loseHP(int delta);
	void turnToWall(Game& game);
	void turnToFood(Game& game, Food& food);
	void turnToVoid(Game& game);
	void reborn(Game& game);

	bool is_pointEmpty(Game& game, int x, int y);
	void clearPoint(Game& game, int x, int y);
	void setPoint(Game& game, int x, int y);
	void changeHead(int type);

	int getHeadX() const;
	int getHeadY() const;
	int getTailX() const;
	int getTailY() const;

	void draw(int x, int y, bool is_head = 0, bool clear = 0);
	void drawSnake(bool clear = 0);
	void drawHP(double hp);
	void drawHP();
	void updateHP(int before, int after);
	void drawLife();
	void updateLife(int add);

	friend class Game;
};

