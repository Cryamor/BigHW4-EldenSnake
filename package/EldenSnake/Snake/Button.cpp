#include "Button.h"
#include <iostream>
#include <graphics.h>

LOGFONT LF_BUTTON;
IMAGE IM_ARROW;

Button::Button(LPCTSTR str, int l, int t, int r, int b)
	:left(l), right(r), top(t), bottom(b), condition(0)
{
	set(str);
	setLF();
}

void Button::set(LPCTSTR str, int l, int t, int r, int b)
{
	left = l;
	top = t;
	right = r;
	bottom = b;
	set(str);
}

void Button::set(LPCTSTR str)
{
	tag = str;
}

void Button::setLF()
{
	gettextstyle(&LF_BUTTON);
	LF_BUTTON.lfQuality = ANTIALIASED_QUALITY;
	LF_BUTTON.lfHeight = 25;
	LF_BUTTON.lfWidth = 12;
	_tcscpy_s(LF_BUTTON.lfFaceName, _T("¿¬Ìå"));

	settextstyle(&LF_BUTTON);

	loadimage(&IM_ARROW, _T("bin/image/arrow.jpg"),35,8); //140*32->35*8
}

int Button::getCond()
{
	return condition;
}

int Button::changeCond(int dst)
{
	condition = dst;
	draw();
	return dst;
}

/*-1 -nochange*/
int Button::hasChanged()
{
	int before = condition;
	int ret = getMes();
	if (ret != before)
		return ret;

	return -1;
}

/* 0/1/2 -condition */
int Button::getMes()
{
	ExMessage em;
	getmessage(&em);
	flushmessage();
	if (em.x >= this->left && em.x <= this->right && em.y >= this->top && em.y <= this->bottom) {
		if(em.message== WM_LBUTTONDOWN|| em.message == WM_LBUTTONUP|| em.message == WM_LBUTTONDBLCLK)
			return changeCond(2);
		else 
			return changeCond(1);
	}

	return changeCond(0);
}

void Button::draw()
{		

	RECT r = { left,top,right,bottom };
	int h = bottom - top;
	RECT r_ar = { left - 45,top + h / 2 - 4,left,bottom - h / 2 + 4 };
	
	switch (condition) {
		case 0:
			setcolor(BLACK);
			setfillcolor(BLACK);
			fillrectangle(r_ar.left, r_ar.top, r_ar.right, r_ar.bottom);
			break;
		case 1:
		case 2:
			putimage(r_ar.left, r_ar.top, &IM_ARROW);
			break;
	}

	settextcolor(WHITE);
	settextstyle(&LF_BUTTON);

	drawtext(tag, &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_NOCLIP);
}
