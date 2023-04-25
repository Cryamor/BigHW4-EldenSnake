#include "Log.h"
#include <fstream>
#include <sstream>
#include <cstdio>
#include <time.h>
#include <conio.h>

extern IMAGE IM_BK;

Log::Log(const char* file, const char* file_max)
{
	readFromFile(file);
	readMaxScore(file_max);
}

Log::~Log()
{
	item.clear();
	max[0] = 0;
	max[1] = 0;
	max[2] = 0;
}

int Log::readFromFile(const char* file)
{
	ifstream in(file);
	if (!in.is_open())
		return -1;

	while (!in.eof()) {
		Item i;
		in >> i.mode >> i.dif >> i.score >> i.player;
		in >> i.begin_time >> i.end_time >> i.last_time;
		item.push_back(i);
	}

	item.pop_back(); //按以上读法最后会多出来一个空项
	in.close();
	return 0;
}

int Log::writeToFile(const char* file)
{
	ofstream out(file);
	if (!out.is_open())
		return -1;

	for (list<Item>::const_iterator it = item.begin(); it != item.end(); it++) {
		out << it->mode << " " << it->dif << " " << it->score << " " << it->player << " "
			<< it->begin_time << " " << it->end_time << " " << it->last_time << endl;
	}

	out.close();
	return 0;
}

int Log::addToFile(const char* file, Item& _item)
{
	ofstream out(file, ios::out | ios::app);
	if (!out.is_open())
		return -1;

	out << _item.mode << " " << _item.dif << " " << _item.score << " " << _item.player << " "
		<< _item.begin_time << " " << _item.end_time << " " << _item.last_time << endl;

	out.close();
	return 0;
}

int Log::add(const char* file, Item& _item)
{
	item.push_back(_item);
	if (_item.score > max[_item.mode - 1]) {
		max[_item.mode - 1] = _item.score;
	}
	if (addToFile(file, _item) < 0)
		return -1;
	else
		return 0;
}

int Log::clearAll(const char* file, const char* file_max)
{
	remove(file);
	remove(file_max);

	ofstream out(file);
	if (!out.is_open())
		return -1;
	out.close();

	ofstream _out(file_max);
	if (!_out.is_open())
		return -1;
	_out.close();

	return 0;
}

int Log::readMaxScore(const char* file)
{
	ifstream in(file);
	if (!in.is_open())
		return -1;

	in >> max[0] >> max[1] >> max[2];
	in.close();
	return 0;
}

int Log::writeMaxScore(const char* file)
{
	ofstream out(file);
	if (!out.is_open())
		return -1;

	out << max[0] << " " << max[1] << " " << max[2];
	out.close();
	return 0;
}

void Log::show()
{
	/*
	版本 用户名 得分 难度 起始时间 结束时间 游戏时间
	----------------------50
	Details
	----------------------550
	Tips
	*/
	const int y0 = 50, ye = 550, dy = 25;
	const int x[8] = { 30,110,280,360,430,630,830,930 };
	int page = 1;
	const int each_page = 20;  //每页20条
	int max_page = this->sum() / each_page + (this->sum() % each_page != 0);

	showData(page);

	int n;
	list<Item>::iterator itt = this->item.begin();
	RECT r;
	char ch[20] = { 0 }; //用于查找
	int focus = 0, page_line = getPageLine(page);
	showFocus(focus);
	while (true) {
		if (_kbhit()) {
			int ch1 = _getch(), ch2;
			switch (ch1) {
				case 224:
					ch2 = _getch();
					switch (ch2) {
						case 72:
							showFocus(focus, 1);
							focus == 0 ? focus += page_line-1 : focus--;
							showFocus(focus);
							break;
						case 80:
							showFocus(focus, 1);
							focus == page_line - 1 ? focus = 0 : focus++;
							showFocus(focus);
							break;
						case 75:
							if (page > 1) {
								page_line = getPageLine(--page);
								showData(page);
								focus = 0;
								showFocus(focus);
							}
							break;
						case 77:
							if (page < max_page) {
								page_line = getPageLine(++page);
								showData(page);
								focus = 0;
								showFocus(focus);
							}
							break;
					}
					break;
				case '\r':
					n = (page - 1) * each_page + focus;
					itt = this->item.begin();
					for (int i = 0; i < n; i++)
						itt++;
					r = { x[1],y0 + dy * focus,x[2],y0 + dy * (focus + 1) };
					inputBox(r, itt->player, 15);
					writeToFile("bin/log/1.log");
					showData(page);
					showFocus(focus);
					break;
				case 'q':
				case 'Q':
					r = { 750,ye,920,575 };					
					inputBox(r, ch, 15);
					showData(page, 20, ch);
					showFocus(focus);
					break;
				case 8: //backspace
					if (page_line == 1)
						continue; //只剩一条则跳过
					n = (page - 1) * each_page + focus;
					itt = this->item.begin();
					for (int i = 0; i < n; i++)
						itt++;
					r = { x[1],y0 + dy * focus,x[2],y0 + dy * (focus + 1) };
					itt = item.erase(itt);
					
					writeToFile("bin/log/1.log");
					showData(page);
					showFocus(focus);
					break;
				case 27: //esc
					return;
			}
		}
	}
}

int Log::getMaxScore(int mode)
{
	return max[mode - 1];
}

int Log::sum()
{
	return item.size();
}

void Log::showMaxScore(int mode)
{
	LOGFONT f;
	gettextstyle(&f);
	settextcolor(LIGHTGRAY);
	_tcscpy_s(f.lfFaceName, _T("黑体"));
	f.lfItalic = false;
	f.lfHeight = 16;
	f.lfWidth = 8;
	f.lfWeight = 0;
	settextstyle(&f);

	RECT r = { 750,305,930,325 };
	setfillcolor(BLACK);
	setlinecolor(BLACK);
	fillrectangle(r.left, r.top, r.right, r.bottom);

	TCHAR tc[15];
	_stprintf_s(tc, _T("Record: %d"), max[mode - 1]);

	drawtext(tc, &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_NOCLIP);
}

void Log::showFocus(int focus, bool clear)
{
	IMAGE IM_ARR;
	loadimage(&IM_ARR, _T("bin/image/arrow.jpg"), 17, 4);
	int top = focus * 25 + 50;
	if (clear) {
		setfillcolor(BLACK);
		setlinecolor(BLACK);
		fillrectangle(10, top, 28, top + 25);
	}
	else {
		putimage(10, top + 10, &IM_ARR);
	}
}

void Log::showData(int page, const int each_page, char* str)
{
	const int y0 = 50, ye = 550, dy = 25;
	const int x[8] = { 30,110,280,360,430,630,830,930 };
	//显示表头和底部提示
	showHeadTips(x, y0, ye);

	int n = (page - 1) * each_page;
	list<Item>::iterator it = this->item.begin();
	for (int i = 0; i < n; i++)
		it++;  //设置迭代器位置

	for (int j = 0; j < each_page && it != this->item.end(); j++, it++) {
		int yPos = y0 + j * dy;
		RECT r = { x[0],yPos,x[1],yPos + dy };
		LPCTSTR lp = _T("");
		TCHAR tc[20];
		struct tm tm_t;
		//模式
		switch (it->mode) {
			case 1:
				lp = _T("入门版");
				break;
			case 2:
				lp = _T("进阶版");
				break;
			case 3:
				lp = _T("高级版");
				break;
		}
		drawtext(lp, &r, DT_VCENTER | DT_CENTER | DT_NOCLIP | DT_SINGLELINE);
		//用户名，str!=NULL为查找状态
		if (str) {
			if (strcmp(it->player, str) == 0)
				settextcolor(YELLOW);
			_stprintf_s(tc, _T("%hs"), it->player);
			r = { x[1],yPos,x[2],yPos + dy };
			drawtext(tc, &r, DT_VCENTER | DT_NOCLIP | DT_LEFT | DT_SINGLELINE);
			settextcolor(WHITE);
		}
		else {
			_stprintf_s(tc, _T("%hs"), it->player);
			r = { x[1],yPos,x[2],yPos + dy };
			drawtext(tc, &r, DT_VCENTER | DT_NOCLIP | DT_LEFT | DT_SINGLELINE);
		}
		//分数
		_stprintf_s(tc, _T("%d"), it->score);
		r = { x[2],yPos,x[3],yPos + dy };
		drawtext(tc, &r, DT_VCENTER | DT_CENTER | DT_NOCLIP | DT_SINGLELINE);
		//难度
		switch (it->dif) {
			case 1:
				lp = _T("IDIOT");
				break;
			case 2:
				lp = _T("EASY");
				break;
			case 3:
				lp = _T("NORMAL");
				break;
			case 4:
				lp = _T("HARD");
				break;
			case 5:
				lp = _T("HELL");
				break;
		}
		r = { x[3],yPos,x[4],yPos + dy };
		drawtext(lp, &r, DT_VCENTER | DT_CENTER | DT_NOCLIP | DT_SINGLELINE);
		//起始时间
		localtime_s(&tm_t, &it->begin_time);
		_stprintf_s(tc, _T("%d-%02d-%02d %02d:%02d:%02d")
			, tm_t.tm_year+1900, tm_t.tm_mon, tm_t.tm_mday, tm_t.tm_hour, tm_t.tm_min, tm_t.tm_sec);
		r = { x[4],yPos,x[5],yPos + dy };
		drawtext(tc, &r, DT_VCENTER | DT_CENTER | DT_NOCLIP | DT_SINGLELINE);
		//结束时间
		localtime_s(&tm_t, &it->end_time);
		_stprintf_s(tc, _T("%d-%02d-%02d %02d:%02d:%02d")
			, tm_t.tm_year+1900, tm_t.tm_mon, tm_t.tm_mday, tm_t.tm_hour, tm_t.tm_min, tm_t.tm_sec);
		r = { x[5],yPos,x[6],yPos + dy };
		drawtext(tc, &r, DT_VCENTER | DT_CENTER | DT_NOCLIP | DT_SINGLELINE);
		//耗时
		time_t min, hour, sec;
		sec = it->last_time % 60;
		min = it->last_time / 60;
		hour = min / 60;
		min = min % 60;
		_stprintf_s(tc, _T("%02lld:%02lld:%02lld"), hour, min, sec);
		r = { x[6],yPos,x[7],yPos + dy };
		drawtext(tc, &r, DT_VCENTER | DT_CENTER | DT_NOCLIP | DT_SINGLELINE);

	}
}

void Log::inputBox(RECT& r, char* str, int maxlen)
{
	setlinecolor(DARKGRAY);
	setfillcolor(BLACK);
	fillrectangle(r.left, r.top, r.right, r.bottom);

	settextcolor(WHITE);
	LOGFONT f;
	gettextstyle(&f);
	f.lfHeight = r.bottom - r.top - 2;
	f.lfWidth = f.lfHeight / 2;
	f.lfWeight = 0;
	settextstyle(&f);


	TCHAR temp[20] = { 0 };
	//闪烁
	while (!_kbhit()) {
		temp[0] = '_';
		outtextxy(r.left + 1, r.top + 1, temp);
		Sleep(200);
		fillrectangle(r.left, r.top, r.right, r.bottom);
		Sleep(200);
	}
	fillrectangle(r.left, r.top, r.right, r.bottom);

	int n = 0;
	while (true) {
		int ch = _getch();
		if (ch == '\r') {
			int i = 0;
			for (; temp[i] != '\0'; i++)
				str[i] = (char)temp[i];
			str[i] = '\0';
			break;
		}
		else if (ch == 27) {
			break;
		}
		else if (ch == '\b') {
			if (n > 0) {
				--n;
				temp[n] = '\0';
				fillrectangle(r.left, r.top, r.right, r.bottom);
				outtextxy(r.left + 1, r.top + 1, temp);
			}
		}
		else if (ch > 32 && ch < 127) {  //可见字符
			if (n < maxlen) {
				temp[n] = char(ch);
				outtextxy(r.left + 1, r.top + 1, temp);
				++n;
			}
		}
	}
}

int Log::getPageLine(int page)
{
	if (sum() >= page * 20)
		return 20;
	else {
		return sum() % 20;
	}
}

void Log::showHeadTips(const int x[], const int y0, const int ye)
{
	cleardevice();
	putimage(0, 0, &IM_BK);
	//设置字体
	{
		LOGFONT f;
		gettextstyle(&f);
		f.lfHeight = 20;
		f.lfWidth = 10;
		f.lfWeight = 0;
		_tcscpy_s(f.lfFaceName, _T("楷体"));
		settextstyle(&f);
		settextcolor(WHITE);
	}
	//表头
	{
		RECT r = { x[0],0,x[1],y0 };
		drawtext(_T("版本"), &r, DT_VCENTER | DT_CENTER | DT_NOCLIP | DT_SINGLELINE);
		r = { x[1],0,x[2],y0 };
		drawtext(_T("用户名"), &r, DT_VCENTER | DT_NOCLIP | DT_LEFT | DT_SINGLELINE);
		r = { x[2],0,x[3],y0 };
		drawtext(_T("得分"), &r, DT_VCENTER | DT_CENTER | DT_NOCLIP | DT_SINGLELINE);
		r = { x[3],0,x[4],y0 };
		drawtext(_T("难度"), &r, DT_VCENTER | DT_CENTER | DT_NOCLIP | DT_SINGLELINE);
		r = { x[4],0,x[5],y0 };
		drawtext(_T("起始时间"), &r, DT_VCENTER | DT_CENTER | DT_NOCLIP | DT_SINGLELINE);
		r = { x[5],0,x[6],y0 };
		drawtext(_T("结束时间"), &r, DT_VCENTER | DT_CENTER | DT_NOCLIP | DT_SINGLELINE);
		r = { x[6],0,x[7],y0 };
		drawtext(_T("游戏时长"), &r, DT_VCENTER | DT_CENTER | DT_NOCLIP | DT_SINGLELINE);
	}
	//底部提示
	{
		RECT r = { x[0],ye,x[5],600 };
		drawtext(_T("↑↓选择 ←→翻页 ENTER修改当前项用户名 ESC退出 Backspace删除当前项 Q查找用户"), &r, DT_VCENTER | DT_NOCLIP | DT_LEFT | DT_WORDBREAK);
	}
}

