#pragma once

#include <graphics.h>

class Button
{
private:
	int left;
	int top;
	int right;
	int bottom;	
	int condition;  //0 -正常 1 -选中 2 -确认
	LPCTSTR tag;
public:
	Button(LPCTSTR str, int l, int t, int r, int b);
	void set(LPCTSTR str, int l, int t, int r, int b);
	void set(LPCTSTR str);
	void setLF();

	int getCond();
	int changeCond(int dst);
	int hasChanged();
	int getMes();

	void draw();

};

