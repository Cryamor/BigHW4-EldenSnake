#pragma once
#include <list>
#include <graphics.h>
#include <string.h>

using namespace std;

class Item {
public:
	int mode;  // 1-入门 2-进阶 3-高级
	int dif;
	int score;
	char player[20];
	time_t begin_time;
	time_t end_time;
	time_t last_time;
	Item() :mode(0), dif(0), score(0), begin_time(0), end_time(0), last_time(0)
	{
		strcpy_s(player, "PLAYER");
	}
	
	Item(int m, int d, char name[]) 
		:mode(m), dif(d), score(0), begin_time(0), end_time(0), last_time(0)
	{
		strcpy_s(player, name);
	}
};

class Log
{
private:	
	list<Item> item;
	int max[3];

public:
	Log(const char* file, const char* file_max);
	~Log();
	
	int clearAll(const char* file, const char* file_max);
	int readFromFile(const char* file);
	int writeToFile(const char* file);
	int addToFile(const char* file, Item& _item);
	int add(const char* file, Item& _item);

	void sortByScore(bool is_up = false);
	void sortByTime(bool is_up = false);
	void sortByMode(bool is_up = true);

	int readMaxScore(const char* file);
	int writeMaxScore(const char* file);

	void showMaxScore(int mode);
	void show();
	void showHeadTips(const int x[], const int y0 = 50, const int ye = 550);
	void showFocus(int focus, bool clear = false);
	void showData(int page, const int each_page = 20, char* str = NULL);
	void inputBox(RECT& r, char* str, int maxlen);
	int getPageLine(int page);

	int getMaxScore(int mode);
	int sum();

	friend class Game;
};

