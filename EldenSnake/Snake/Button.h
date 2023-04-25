#pragma once

#include <graphics.h>

class Button
{
private:
	int left;
	int top;
	int right;
	int bottom;	
	int condition;  //0 -���� 1 -ѡ�� 2 -ȷ��
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

