#pragma once
#include "Snake.h"
#include <graphics.h>
const int MAP_A = 35; //边长 0<=x,y<=34
const int MAP_MAX = 35 * 35;
const double PI = 3.14159265359;

#define   BK_COLOR   TRANSPARENT

#define   DIF_IDIOT        1
#define   DIF_EASY         2
#define   DIF_NORMAL       3
#define   DIF_HARD         4
#define   DIF_HELL         5

#define   MAP_VOID         0
#define   MAP_SNAKE        1
#define   MAP_FOOD         2
#define   MAP_SP           3
#define   MAP_BOMB         4
#define   MAP_WALL         5
#define   MAP_HWALL        6
#define   MAP_SWALL        7
#define   MAP_LIGHT        5
#define   MAP_ENEMY        9

class Snake;

class Game
{
private:
	int* map;
	int dif;
	int score;
	double score_power; //分数倍率
	int lose_hp;
	int pause;
	time_t start_time;
	time_t last_time;
	char player[20];
public:
	Game(int);
	~Game();
	void clear();
	void initMap(int(*nmap)[3] = NULL);
	void clearMap();
	void adaptToDif(Snake& snake);

	void preTips();
	int start();
	int menu();
	int _menu();
	void menu_show_detail(LPCTSTR str, RECT* r);
	void settings();
	void setting_Dif();
	void setting_BGM();
	void setting_Name();	
	void localLog();

	int run();
	int standardGame(int mode);
	int gameOver(bool is_RPG = false);

	int doublePlayer();
	bool changeDir_double(Snake& snake1, Snake& snake2);
	int gameOver_double(int winnner);	
	void showScore_double(int score, int player);
	void showGameUI_double();
	void showItemRules_double();
	bool is_headToHead(Snake& snake1, Snake& snake2);

	int _menu_RPG();
	int RPG(int level);
	int RPG_G1();
	int RPG_G2();
	int RPG_G3();
	int RPG_G4();
	int RPG_G5();
	void showMsg(LPCTSTR str);
	void showDia(int speaker, LPCTSTR content, int yDia);
	void showGameUI_RPG(int level);
	void setGuard(int x, int y, int dir);
	void drawHP_RPG(double hp);
	void updateHP_RPG(int before, int after);
	void alarm(Snake& snake, Food& food, SP& sp, Bomb& bomb);

	bool is_mapFull();
	void updateScore(int type, bool clear = false);	
	void showScore(int add);
	void showGameUI(int mode);
	void showTime();
	void showItemRules();
	void showModeTitle(LPCTSTR title);
	void inputBox(RECT& r, char* str, int maxlen);

	void showPointWall(int x, int y);
	void showPointVoid(int x, int y);
	void showPointWeak(int x, int y);
	void showPointLight(int x, int y);
	bool is_pointEmpty(int x, int y) const;
	void setPoint(int x, int y, int type);
	void clearPoint(int x, int y);

	friend class Snake;
	friend class SP;
};

