#include <iostream>
#include <time.h>
#include <Windows.h>
#include <conio.h>
#include <graphics.h>
#include <string.h>
#include <string>
#include "Game.h"
#include "Snake.h"
#include "Button.h"
#include "Log.h"
#include <mmsystem.h>
#pragma comment(lib,"winmm.lib")

/*
(0,0)------------------------------------(34,0)
|
|
|
|
|
|
|
(0,34)-----------------------------------(34,34)
*/

bool BGM_ON = true;

IMAGE IM_START;
IMAGE IM_BK; //BK_IMAGE
IMAGE IM_WALL; 

//{x,y,MAP_TYPE}

int MAP_0[][3] = { {1,1,MAP_WALL},{33,1,MAP_WALL},{1,33,MAP_WALL},{33,33,MAP_WALL},{-1,-1,0} };
int MAP_DOUBLE[][3] = { {17,17,MAP_WALL},{17,18,MAP_WALL},{17,16,MAP_WALL},{16,17,MAP_WALL},{18,17,MAP_WALL}, {-1,-1,0} };
int MAP_G1[][3] = { {15,15,MAP_WALL},{16,15,MAP_WALL},{17,15,MAP_WALL},{18,15,MAP_WALL},{19,15,MAP_WALL},
{15,16,MAP_WALL},{19,16,MAP_WALL},{15,17,MAP_WALL},{19,17,MAP_WALL},{15,18,MAP_WALL},{19,18,MAP_WALL},
{15,19,MAP_WALL},{16,19,MAP_WALL},{17,19,MAP_WALL},{18,19,MAP_WALL},{19,19,MAP_WALL},{-1,-1,0} };
int MAP_G2[][3] = {
{15,15,MAP_WALL},{16,15,MAP_WALL},{17,15,MAP_WALL},{18,15,MAP_WALL},{19,15,MAP_WALL},
{15,16,MAP_WALL},{16,16,MAP_WALL},{17,16,MAP_WALL},{18,16,MAP_WALL},{19,16,MAP_WALL},
{15,17,MAP_WALL},{16,17,MAP_WALL},{17,17,MAP_WALL},{18,17,MAP_WALL},{19,17,MAP_WALL},
{15,18,MAP_WALL},{16,18,MAP_WALL},{17,18,MAP_WALL},{18,18,MAP_WALL},{19,18,MAP_WALL},
{15,19,MAP_WALL},{16,19,MAP_WALL},{17,19,MAP_WALL},{18,19,MAP_WALL},{19,19,MAP_WALL},
{7,7,MAP_WALL},	{7,8,MAP_WALL},	{8,7,MAP_WALL},	{8,8,MAP_WALL},
{7,26,MAP_WALL},{7,27,MAP_WALL},{8,26,MAP_WALL},{8,27,MAP_WALL},
{26,26,MAP_WALL},{26,27,MAP_WALL},{27,26,MAP_WALL},{27,27,MAP_WALL},
{26,7,MAP_WALL},{26,8,MAP_WALL},{27,7,MAP_WALL},{27,8,MAP_WALL},
{-1,-1,0} };
int MAP_G3[][3] = {
{15,15,MAP_WALL},{16,15,MAP_WALL},{17,15,MAP_WALL},{18,15,MAP_WALL},{19,15,MAP_WALL},
{15,16,MAP_WALL},{16,16,MAP_WALL},{17,16,MAP_WALL},{18,16,MAP_WALL},{19,16,MAP_WALL},
{15,17,MAP_WALL},{16,17,MAP_WALL},{17,17,MAP_WALL},{18,17,MAP_WALL},{19,17,MAP_WALL},
{15,18,MAP_WALL},{16,18,MAP_WALL},{17,18,MAP_WALL},{18,18,MAP_WALL},{19,18,MAP_WALL},
{15,19,MAP_WALL},{16,19,MAP_WALL},{17,19,MAP_WALL},{18,19,MAP_WALL},{19,19,MAP_WALL},
{-1,-1,0} };
int MAP_G5[][3]={
{14,14,MAP_WALL},{15,14,MAP_WALL},{16,14,MAP_WALL},{17,14,MAP_WALL},{18,14,MAP_WALL},{19,14,MAP_WALL},{20,14,MAP_WALL},
{14,15,MAP_WALL},{15,15,MAP_WALL},{16,15,MAP_WALL},{17,15,MAP_WALL},{18,15,MAP_WALL},{19,15,MAP_WALL},{20,15,MAP_WALL},
{14,16,MAP_WALL},{15,16,MAP_WALL},{16,16,MAP_WALL},{17,16,MAP_WALL},{18,16,MAP_WALL},{19,16,MAP_WALL},{20,16,MAP_WALL},
{14,17,MAP_WALL},{15,17,MAP_WALL},{16,17,MAP_WALL},{17,17,MAP_WALL},{18,17,MAP_WALL},{19,17,MAP_WALL},{20,17,MAP_WALL},
{14,18,MAP_WALL},{15,18,MAP_WALL},{16,18,MAP_WALL},{17,18,MAP_WALL},{18,18,MAP_WALL},{19,18,MAP_WALL},{20,18,MAP_WALL},
{14,19,MAP_WALL},{15,19,MAP_WALL},{16,19,MAP_WALL},{17,19,MAP_WALL},{18,19,MAP_WALL},{19,19,MAP_WALL},{20,19,MAP_WALL},
{14,20,MAP_WALL},{15,20,MAP_WALL},{16,20,MAP_WALL},{17,20,MAP_WALL},{18,20,MAP_WALL},{19,20,MAP_WALL},{20,20,MAP_WALL},
{-1,-1,0} }; 


Game::Game(int d)
	:score(0), dif(d), score_power(1.0), pause(80), last_time(0), start_time(0), lose_hp(8)
{
	strcpy_s(player, "PLAYER");
	map = new(std::nothrow)int[MAP_A * MAP_A];
}

Game::~Game()
{
	clear();
}

void Game::clear()
{
	if (map)
		delete[] map;
	map = NULL;

	start_time = 0;
	last_time = 0;
	score = 0;
}

//读取存储地图信息的数组
void Game::initMap(int(*nmap)[3])
{
	memset(map, 0, MAP_MAX * sizeof(map));
	//init Wall
	if (1) {
		for (int i = 0; i < MAP_A; i++) {
			map[i] = MAP_WALL;
			map[MAP_A * (MAP_A - 1) + i] = MAP_WALL;
		}

		for (int j = 1; j < MAP_A; j++) {
			map[j * MAP_A] = MAP_WALL;
			map[(j + 1) * MAP_A - 1] = MAP_WALL;
		}

		if (nmap) {
			int(*p)[3] = nmap;
			for (;; p++) {
				int x = (*p)[0];
				int y = (*p)[1];
				if (x < 0 || y < 0)
					break;
				map[y * MAP_A + x] = (*p)[2];
			}
		}
	}
	last_time = 0;
	time(&start_time);
}

void Game::clearMap()
{
	memset(map, 0, MAP_MAX * sizeof(map));
	score = 0;
	last_time = 0;
	start_time = 0;
}

//根据选择的难度调整分数倍率、蛇的速度（停顿）、生命数、发癫扣血量
void Game::adaptToDif(Snake& snake)
{
	switch (dif) {
		case DIF_IDIOT:
			score_power = 0.2;
			pause = 150;
			snake.life = 5;
			lose_hp = 0;
			break;
		case DIF_EASY:
			score_power = 0.5;
			pause = 120;
			snake.life = 4;
			lose_hp = 4;
			break;
		case DIF_NORMAL:
			score_power = 1;
			pause = 90;
			snake.life = 3;
			lose_hp = 8;
			break;
		case DIF_HARD:
			score_power = 2;
			pause = 60;
			snake.life = 2;
			lose_hp = 16;
			break;
		case DIF_HELL:
			score_power = 10;
			pause = 25;
			snake.life = 1;
			lose_hp = 16;
			break;
	}
}

//最开始的提示部分
void Game::preTips()
{
	setbkcolor(BLACK);
	cleardevice();
	RECT r = { 100,50,860,550 };
	LOGFONT f;
	gettextstyle(&f);
	f.lfHeight = 30;
	f.lfWidth = 15;
	f.lfWeight = 500;
	_tcscpy_s(f.lfFaceName, _T("楷体"));
	f.lfQuality = ANTIALIASED_QUALITY;
	settextstyle(&f);
	int color = 255;
	while (color > 0) {
		settextcolor(RGB(color, color, color));
		drawtext(_T("注意:\n1.本游戏含背景音乐，请注意调整音量以免对听觉器官造成不必要的损害，您也可以在设置中选择关闭背景音乐。\n2.为了确保键盘输入正确，在游戏开始时会将您的输入法改为英文，游戏中需要使用的按键包括Esc、W、A、S、D、F、Q、Backspace、回车及方向键。\n3.菜单界面均采用鼠标选择方式，回车和Esc分别为确认与退出，鼠标左键点不动时可以尝试多点几下或按回车键。\n4.祝您游戏愉快!"),
			&r, DT_VCENTER | DT_LEFT | DT_WORDBREAK);
		Sleep(50);
		color -= 3;
	}
	Sleep(100);
}

//开始菜单及加载
int Game::start()
{
	/*调整输入法*/
	TCHAR _szKBNAME[MAX_PATH];
	GetKeyboardLayoutName(_szKBNAME);
	HKL hkl = LoadKeyboardLayout(_T("00000409"), KLF_ACTIVATE); //设置为英语美式键盘
	HWND TheWindow = GetForegroundWindow();
	SendMessage(TheWindow, WM_IME_NOTIFY, 1, 0);
	SendMessage(TheWindow, WM_INPUTLANGCHANGEREQUEST, 1, (LPARAM)hkl);
	SendMessage(TheWindow, WM_INPUTLANGCHANGE, 1, (LPARAM)hkl);
	ActivateKeyboardLayout(hkl, KLF_SETFORPROCESS);

	initgraph(960, 600);
	loadimage(&IM_START, _T("bin/image/start.jpg"), 960, 600);
	loadimage(&IM_BK, _T("bin/image/bk.jpg"), 960, 600);
	loadimage(&IM_WALL, _T("bin/image/block.jpg"), 14, 14);
	
	preTips();
	
	setbkmode(TRANSPARENT);
	setbkcolor(BK_COLOR);
	putimage(0, 0, &IM_START);
	//bgm
	if (BGM_ON) {
		mciSendString(_T("open bin/music/start.mp3 alias STARTBGM"), NULL, 0, NULL);
		mciSendString(_T("play STARTBGM repeat"), NULL, 0, NULL);
	}

	int R = 255, G = 240, B = 190;
	settextcolor(RGB(R, G, B));
	RECT r = { 0,400,960,600 };
	LOGFONT f;
	gettextstyle(&f);				
	f.lfHeight = 25;
	f.lfWidth = 12;
	f.lfWeight = 500;
	_tcscpy_s(f.lfFaceName, _T("Constantia"));		
	f.lfQuality = ANTIALIASED_QUALITY;		
	settextstyle(&f);

	int alpha = 99, minus = 1;  //渐变效果

	while (!_kbhit()) {
		if (alpha == 0 || alpha == 100)
			minus = -minus;
		alpha -= minus;

		settextcolor(RGB(R * alpha / 100, G * alpha / 100, B * alpha / 100));
		drawtext(_T("Press  Any  Key  To  Continue . . ."), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_NOCLIP);
		Sleep(50);
	}
	//接收任意键输入
	int ch = _getch();

	return 0;
}

/*开始界面后的一级菜单 */
int Game::menu()
{
	cleardevice();
	putimage(0, 0, &IM_START);

	Button bu[6] = {
		{_T("标准模式"),150,270,240,315},
		{_T("故事模式"),150,315,240,360},
		{_T("双人对决"),150,360,240,405},
		{_T("设 置"),150,405,240,450},
		{_T("本地记录"),150,450,240,495},
		{_T("退 出"),150,495,240,540}
	};
	//details
	/*LPCTSTR str[6] = {
		_T("吃掉食物但不要撞到自己或墙壁！除画面外原汁原味的贪吃蛇体验，或是增加一些有趣的小规则"),
		_T("带谁也看不懂的剧情的RPG模式，体验闯关与多种玩法的缝合！"),
		_T("玩游戏就是为了赢！与好兄弟/好姐妹/NPY各持键盘的一端，一决高下"),
		_T("设置难度、BGM开关以及用户名"),
		_T("查看标准模式的本地文件，记录了难度、得分、起始时间、游戏时间等数据，可以修改用户名"),
		_T("哈人游戏，润了润了")
	};
	RECT r_detail = { 740,280,840,540 };*/

	int ret = menu_button(bu, 6);
	if (ret == 6)
		ret = 0;
	return ret;
}

/*选择三种模式的二级菜单 0 -quit 1 -menu*/
int Game::_menu()
{
	cleardevice();
	putimage(0, 0, &IM_START);

	Button bu[4] = {
		{_T("正常模式"),150,315,240,360},
		{_T("后人受苦"),150,360,240,405},
		{_T("后人乘凉"),150,405,240,450},
		{_T("后 退"),150,495,240,540}
	};

	//details
	LPCTSTR str[3] = {
		_T("入门版：WASD/方向键控制蛇移动，当血量归零时蛇死亡。若有剩余生命则尸体消失并复活，否则游戏结束"),
		_T("进阶版：蛇死亡后，尸体变为墙壁"),
		_T("高级版：蛇死亡后，尸体变为食物")
	};
	RECT r_detail = { 740,320,840,540 };

	Sleep(200);

	int ret = 0;
	bool flag = 1;
	int focus = 0;
	//show buttons
	bu[0].changeCond(1);

	for (int i = 0; i < 4; i++) {
		bu[i].draw();
	}

	while (flag) {
		for (int i = 0; i < 4 && flag; i++) {
			if (_kbhit()) {
				int ch = _getch();
				if (ch == '\r') {
					bu[focus].changeCond(2);
					Sleep(500);
					if (focus != 3) {
						if (BGM_ON) {
							mciSendString(_T("stop STARTBGM"), NULL, 0, NULL);
							mciSendString(_T("close STARTBGM"), NULL, 0, NULL);
						}
						while (true) {
							int temp1 = standardGame(focus + 1);
							if (temp1 == 1) //replay
								continue;
							else if (temp1 == 2) {
								ret = 0;
								flag = 0;
								break;
							}
							else {  //0-menu
								ret = 1;
								flag = 0;
								break;
							}
						}
					}
					else { //Quit
						ret = 1;
						flag = 0;
						break;
					}
				}
				else if (ch == 27) {
					bu[focus].changeCond(0);
					bu[3].changeCond(2);
					ret = 1;
					flag = 0;
					break;
				}
			}

			int temp = bu[i].hasChanged();
			if (temp == 0)
				bu[i].draw();
			else if (temp == 1) {
				bu[i].draw();
				focus = i;
			}
			else if (temp == 2) {
				bu[i].draw();
				Sleep(500);
				if (i != 3) {
					if (BGM_ON) {
						mciSendString(_T("stop STARTBGM"), NULL, 0, NULL);
						mciSendString(_T("close STARTBGM"), NULL, 0, NULL);
					}
					while (true) {
						int temp1 = standardGame(i + 1);
						if (temp1 == 1) //replay
							continue;
						else if (temp1 == 2) {
							ret = 0;
							flag = 0;
							break;
						}
						else {  //0-menu
							ret = 1;
							flag = 0;
							break;
						}
					}
				}
				else {
					ret = 1;
					flag = 0;
					break;
				}
			}
		}
		if (focus != 3)
			menu_show_detail(str[focus], &r_detail);
	}

	return ret;
}

void Game::menu_show_detail(LPCTSTR str, RECT* r)
{
	setlinecolor(BLACK);
	setfillcolor(BLACK);
	fillrectangle(r->left, r->top, r->right, r->bottom);
	
	settextcolor(LIGHTGRAY);
	LOGFONT f;
	gettextstyle(&f);						
	f.lfHeight = 17;
	f.lfWidth = 8;
	f.lfWeight = FW_DONTCARE;
	_tcscpy_s(f.lfFaceName, _T("楷体"));
	f.lfQuality = ANTIALIASED_QUALITY;		
	settextstyle(&f);					

	drawtext(str, r, DT_TOP | DT_WORDBREAK | DT_WORD_ELLIPSIS | DT_LEFT | DT_NOCLIP);
}

int Game::menu_button(Button* bu, int num, int preFocus, LPCTSTR* detail)
{
	Sleep(200);
	int ret = 0;
	int focus = preFocus;
	//show buttons
	bu[0].changeCond(1);
	for (int i = 0; i < num; i++) {
		bu[i].draw();
	}
	while (true) {
		for (int i = 0; i < num; i++) {
			if (_kbhit()) {
				int ch = _getch();
				if (ch == '\r') {
					bu[focus].changeCond(2);
					bu[focus].draw();
					Sleep(500);
					return focus + 1;
				}
				else if (ch == 27) {
					bu[focus].changeCond(0);
					bu[focus].draw();
					bu[num-1].changeCond(2);
					bu[num-1].draw();
					Sleep(500);
					return 0;
				}
			}

			int temp = bu[i].hasChanged();
			if (temp == 0)
				bu[i].draw();
			else if (temp == 1) {
				bu[i].draw();
				focus = i;
			}
			else if (temp == 2) {
				bu[i].draw();
				Sleep(500);
				return i + 1;
			}
		}
	}
	return ret;
}

/*设置菜单 0 -quit 1 -menu*/
void Game::settings()
{
	cleardevice();
	putimage(0, 0, &IM_BK);

	Button bu[5] = {
		{_T("难 度"),150,270,240,315},
		{_T("背景音乐"),150,315,240,360},
		{_T("用户名"),150,360,240,405},
		{_T("分辨率(锁定)"),150,405,240,450},
		{_T("退 出"),150,495,240,540},
	};

	int focus = 0;
	//show buttons
	bu[0].changeCond(1);

	for (int i = 0; i < 5; i++) {
		bu[i].draw();
	}
	while (true) {
		for (int i = 0; i < 5; i++) {
			if (_kbhit()) {
				int ch = _getch();
				if (ch == '\r') {
					bu[focus].changeCond(2);
					Sleep(500);
					switch (focus) {
						case 0:
							setting_Dif();
							break;
						case 1:
							setting_BGM();
							break;
						case 2:
							setting_Name();
							break;
						case 4:
							return;
					}
				}
				else if (ch == 27) {
					bu[focus].changeCond(0);
					bu[focus].draw();
					bu[4].changeCond(2);
					Sleep(500);
					return;
				}
			}

			int temp = bu[i].hasChanged();
			if (temp == 0)
				bu[i].draw();
			else if (temp == 1) {
				bu[i].draw();
				focus = i;
			}
			else if (temp == 2) {
				bu[i].draw();
				Sleep(500);
				switch (i) {
					case 0:
						setting_Dif();
						break;
					case 1:
						setting_BGM();
						break;
					case 2:
						setting_Name();
						break;
					case 4:
						return;
				}
			}
		}
	}
}

void Game::setting_Dif()
{
	setfillcolor(BLACK);
	setlinecolor(BLACK);
	fillrectangle(750, 270, 840, 495);

	Button bu[5] = {
		{_T("Idiot"),750,270,840,315},
		{_T("Easy"),750,315,840,360},
		{_T("Normal"),750,360,840,405},
		{_T("Hard"),750,405,840,450},
		{_T("Hell"),750,450,840,495}
	};

	int ret = menu_button(bu, 5, this->dif - 1);
	if (ret) 
		this->dif = DIF_IDIOT + ret-1;
	
	setfillcolor(BLACK);
	setlinecolor(BLACK);
	fillrectangle(700, 270, 840, 495);
}

void Game::setting_BGM()
{
	setfillcolor(BLACK);
	setlinecolor(BLACK);
	fillrectangle(750, 270, 840, 360);

	Button bu[2] = {
		{_T("ON  "),750,270,840,315},
		{_T("OFF  "),750,315,840,360}
	};

	int ret = menu_button(bu, 2, (BGM_ON ? 0 : 1));
	setfillcolor(BLACK);
	setlinecolor(BLACK);
	fillrectangle(700, 270, 840, 360);
	//若之前BGM为开且此处选择关，则关掉
	if (BGM_ON && ret == 2) {
		mciSendString(_T("stop STARTBGM"), NULL, 0, NULL);
		mciSendString(_T("close STARTBGM"), NULL, 0, NULL);
		BGM_ON = false;
	}
	else if (ret == 1)
		BGM_ON = true;
}

void Game::setting_Name()
{
	setfillcolor(BLACK);
	setlinecolor(BLACK);
	fillrectangle(750, 270, 930, 495);

	settextcolor(WHITE);
	LOGFONT f;
	gettextstyle(&f);
	f.lfHeight = 14;
	f.lfWidth = 7;
	f.lfWeight = 0;
	settextstyle(&f);

	RECT r_t = { 750,390,910,440 };
	drawtext(_T("最多15字符，不接受中文，回车确定，ESC退出，默认PLAYER"), &r_t, DT_LEFT | DT_VCENTER | DT_WORDBREAK);

	RECT r = { 750,360,910,382 };
	inputBox(r, this->player, 15);

	setfillcolor(BLACK);
	setlinecolor(BLACK);
	fillrectangle(750, 270, 930, 495);
}

void Game::localLog()
{
	cleardevice();
	putimage(0, 0, &IM_BK);
	if (BGM_ON) {
		mciSendString(_T("stop STARTBGM"), NULL, 0, NULL);
		mciSendString(_T("close STARTBGM"), NULL, 0, NULL);
	}
	Log* log = new(std::nothrow) Log("bin/log/1.log", "bin/log/2.log");
	if (log == NULL) {
		outtextxy(477, 300, _T("ERROR!"));
		_getch();
		return;
	}
	
	log->show();

	delete log;
}

/*-1 -error  0 -menu 1 -replay 2-quit*/
int Game::standardGame(int mode)
{
	if (BGM_ON) {
		mciSendString(_T("open bin/music/bk_nor.mp3 alias BGM"), NULL, 0, NULL);
		mciSendString(_T("play BGM repeat"), NULL, 0, NULL);
	}
	Snake* snake = new(std::nothrow) Snake();
	if (snake == NULL)
		return -1;
	Food* food = new(std::nothrow) Food();
	if (food == NULL)
		return -1;
	Bomb* bomb = new(std::nothrow) Bomb();
	if (bomb == NULL)
		return -1;
	SP* sp = new(std::nothrow) SP();
	if (sp == NULL)
		return -1;
	Log* log = new(std::nothrow) Log("bin/log/1.log", "bin/log/2.log");
	if (log == NULL)
		return -1;

	Item m_i(mode, this->dif, this->player);

	initMap();
	adaptToDif(*snake);
	showGameUI(mode);
	snake->setInMap(*this);
	snake->drawHP();
	snake->drawLife();
	int loop_times = 0;

	getchar();
	last_time = 0;
	time(&start_time);
	m_i.begin_time = this->start_time;  //记录起始时间至文件
	log->showMaxScore(mode); //显示最高分

	//先生成一次
	if (this->dif != DIF_IDIOT)
		bomb->create(*this);
	food->create(*this);
	sp->create(*this, 15);
	while (!snake->is_gameOver() && (!is_mapFull())) {
		if (!snake->changeDir()) {
			//ESC暂停时
			showTime();  //刷新计时
			getchar();

			time(&start_time); //重置计时起点
		}

		if (!snake->move(*this)) {
			snake->loseLife();
			updateScore(0);
			if (snake->is_gameOver())
				break;

			if (mode == 1) {
				snake->turnToVoid(*this);
				snake->reborn(*this);
			}
			else if(mode==2)
				snake->turnToWall(*this);
			else if(mode==3)
				snake->turnToFood(*this, *food);

			showTime();  //刷新计时
			getchar();
			snake->updateHP(0, 16);	
			time(&start_time); //重置计时起点
		}

		if (snake->eatFood(*food)) {
			snake->longer(*this);
			updateScore(1);
		}

		if (snake->eatSP(*sp)) {
			snake->longer(*this);
			if (dif < DIF_HELL) {
				if (snake->life < 5)
					snake->updateLife(1);
			}
			updateScore(2);
		}

		if (snake->hitBomb(*bomb)) {
			snake->loseHP(lose_hp);
			updateScore(-1);
			if (snake->is_dead()) {
				snake->loseLife();
				if (snake->is_gameOver())
					break;

				if (mode == 1)
					snake->turnToVoid(*this);
				else if (mode == 2)
					snake->turnToWall(*this);
				else if (mode == 3)
					snake->turnToFood(*this, *food);

				showTime();  //刷新计时
				getchar();
				snake->updateHP(0, 16);
				time(&start_time); //重置计时起点
			}
		}

		//生成食物
		if (loop_times == 20) {
			food->create(*this);
			if (this->dif != DIF_IDIOT)
				bomb->create(*this);
			loop_times = 0;
		}

		showTime();
		if (loop_times % 5)  //闪烁效果实现
			sp->updateTime(*this, 30);

		Sleep(pause);
		loop_times++;
	}

	m_i.score = this->score;
	time(&start_time);
	m_i.end_time = this->start_time;
	m_i.last_time = this->last_time;
	log->add("bin/log/1.log", m_i);
	log->writeMaxScore("bin/log/2.log");

	delete snake;
	delete food;
	delete bomb;
	delete sp;
	delete log;

	if (BGM_ON) {
		mciSendString(_T("stop BGM"), NULL, 0, NULL);
		mciSendString(_T("close BGM"), NULL, 0, NULL);
	}
	int temp = this->gameOver();

	clearMap();
	return temp;
}

/*选择多人模式的二级菜单 0 -quit 1 -menu*/
int Game::menu_double()
{
	cleardevice();
	putimage(0, 0, &IM_START);
	Button bu[3] = {
		{_T("P V P"),150,315,240,360},
		{_T("P V E"),150,360,240,405},
		{_T("后 退"),150,495,240,540}
	};

	int ret = menu_button(bu, 3);
	int _ret;

	while (true) {
		switch (ret) {
			case 0:
			case 3:
				return 1;
			case 1: {
				_ret = doublePlayer();
				if (_ret == 1)
					break;
				else if (_ret == 2)
					return 0;
				else if (_ret == 0)
					return 1;
			}
			case 2: {
				_ret = doublePlayer_PVE();
				if (_ret == 1)
					break;
				else if (_ret == 2)
					return 0;
				else if (_ret == 0)
					return 1;
			}
		}
	}

	return ret;
}

/*-1 -error  0 -menu 1 -replay 2-quit*/
int Game::doublePlayer()
{
	if (BGM_ON) {
		mciSendString(_T("stop STARTBGM"), NULL, 0, NULL);
		mciSendString(_T("close STARTBGM"), NULL, 0, NULL);
		mciSendString(_T("open bin/music/bk_nor.mp3 alias BGM"), NULL, 0, NULL);
		mciSendString(_T("play BGM repeat"), NULL, 0, NULL);
	}
	Snake* snake1 = new(std::nothrow) Snake(3, 16, 1, 8, 8, 1);
	if (snake1 == NULL)
		return -1;
	Snake* snake2 = new(std::nothrow) Snake(3, 16, 1, 26, 26, 2);
	if (snake2 == NULL)
		return -1;
	Food* food = new(std::nothrow) Food();
	if (food == NULL)
		return -1;

	initMap(MAP_DOUBLE);
	showGameUI_double();
	//adapt dif
	this->pause = 120;
	this->score_power = 1.0;

	snake1->setInMap(*this);
	snake2->setInMap(*this);
	int loop_times = 0, score1 = 0, score2 = 0;

	showScore_double(score1, 1);
	showScore_double(score2, 2);

	getchar();
	last_time = 0;
	time(&start_time);

	int winner = 1;  // 0-draw
	food->create(*this);
	while (true) {
		//地图满时，平局
		if (is_mapFull()) {
			winner = 0;
			break;
		}

		if (!changeDir_double(*snake1, *snake2)) {
			//ESC暂停时
			showTime();  //刷新计时
			getchar();
			time(&start_time); //重置计时起点
		}

		if (!snake1->move(*this)) {
			if (is_headToHead(*snake1, *snake2)) {
				if (score1 == score2)
					winner = 0;
				else if (score1 > score2)
					winner = 1;
				else
					winner = 2;
			}
			else
				winner = 2;
			break;
		}

		if (!snake2->move(*this)) {
			if (is_headToHead(*snake1, *snake2)) {
				if (score1 == score2)
					winner = 0;
				else if (score1 > score2)
					winner = 1;
				else
					winner = 2;
			}
			else
				winner = 1;
			break;
		}

		if (snake1->eatFood(*food)) {
			snake1->longer(*this);
			score1++;
			showScore_double(score1, 1);
		}
		if (snake2->eatFood(*food)) {
			snake2->longer(*this);
			score2++;
			showScore_double(score2, 2);
		}


		if (loop_times == 15) {
			food->create(*this);
			loop_times = 0;
		}

		showTime();

		Sleep(pause);
		loop_times++;
	}

	delete snake1;
	delete snake2;
	delete food;

	if (BGM_ON) {
		mciSendString(_T("stop BGM"), NULL, 0, NULL);
		mciSendString(_T("close BGM"), NULL, 0, NULL);
	}

	int temp = this->gameOver_double(winner);

	clearMap();
	return temp;
}

int Game::doublePlayer_PVE()
{
	if (BGM_ON) {
		mciSendString(_T("stop STARTBGM"), NULL, 0, NULL);
		mciSendString(_T("close STARTBGM"), NULL, 0, NULL);
		mciSendString(_T("open bin/music/bk_nor.mp3 alias BGM"), NULL, 0, NULL);
		mciSendString(_T("play BGM repeat"), NULL, 0, NULL);
	}
	Snake* snake1 = new(std::nothrow) Snake(3, 16, 1, 8, 8, 1);
	if (snake1 == NULL)
		return -1;
	Snake* snake2 = new(std::nothrow) Snake(3, 16, 1, 26, 26, 2);
	if (snake2 == NULL)
		return -1;
	Food* food = new(std::nothrow) Food();
	if (food == NULL)
		return -1;

	initMap();
	showGameUI_double();
	//adapt dif
	this->pause = 120;
	this->score_power = 1.0;

	snake1->setInMap(*this);
	snake2->setInMap(*this);
	int loop_times = 0, score1 = 0, score2 = 0;

	showScore_double(score1, 1);
	showScore_double(score2, 2);

	getchar();
	last_time = 0;
	time(&start_time);

	int winner = 1;  // 0-draw
	food->create(*this);
	while (true) {
		//地图满时，平局
		if (is_mapFull()) {
			winner = 0;
			break;
		}

		if (!changeDir_double(*snake1, *snake2)) {
			//ESC暂停时
			showTime();  //刷新计时
			getchar();
			time(&start_time); //重置计时起点
		}

		if (!snake1->move(*this)) {
			if (is_headToHead(*snake1, *snake2)) {
				if (score1 == score2)
					winner = 0;
				else if (score1 > score2)
					winner = 1;
				else
					winner = 2;
			}
			else
				winner = 2;
			break;
		}

		if (!snake2->autoMove(*this,*food)) {
			if (is_headToHead(*snake1, *snake2)) {
				if (score1 == score2)
					winner = 0;
				else if (score1 > score2)
					winner = 1;
				else
					winner = 2;
			}
			else
				winner = 1;
			break;
		}

		if (snake1->eatFood(*food)) {
			snake1->longer(*this);
			score1++;
			showScore_double(score1, 1);
		}
		if (snake2->eatFood(*food)) {
			snake2->longer(*this);
			score2++;
			showScore_double(score2, 2);
		}


		if (loop_times == 15) {
			food->create(*this);
			loop_times = 0;
		}

		showTime();

		Sleep(pause);
		loop_times++;
	}

	delete snake1;
	delete snake2;
	delete food;

	if (BGM_ON) {
		mciSendString(_T("stop BGM"), NULL, 0, NULL);
		mciSendString(_T("close BGM"), NULL, 0, NULL);
	}

	int temp = this->gameOver_double(winner);

	clearMap();
	return temp;
}

bool Game::changeDir_double(Snake& snake1, Snake& snake2)
{
	int ch1, ch2;
	while (_kbhit()) {
		ch1 = _getch();
		switch (ch1) {
			case 224:
				ch2 = _getch();
				switch (ch2) {
					case 72:
						if (snake2.dir != DIR_DOWN)  //反方向不变
							snake2.dir = DIR_UP;
						break;
					case 80:
						if (snake2.dir != DIR_UP)
							snake2.dir = DIR_DOWN;
						break;
					case 75:
						if (snake2.dir != DIR_RIGHT)
							snake2.dir = DIR_LEFT;
						break;
					case 77:
						if (snake2.dir != DIR_LEFT)
							snake2.dir = DIR_RIGHT;
						break;
				}
				return true;
			case 'W':
			case 'w':
				if (snake1.dir != DIR_DOWN)
					snake1.dir = DIR_UP;
				return true;
			case 'A':
			case 'a':
				if (snake1.dir != DIR_RIGHT)
					snake1.dir = DIR_LEFT;
				return true;
			case 'S':
			case 's':
				if (snake1.dir != DIR_UP)
					snake1.dir = DIR_DOWN;
				return true;
			case 'D':
			case 'd':
				if (snake1.dir != DIR_LEFT)
					snake1.dir = DIR_RIGHT;
				return true;
			case 27: //esc
				return false;
		}
	}


	return true;
}

int Game::run()
{
	start(); //开始界面

	while (true) {
		int choice = menu(); 
		int temp = 1;
		//开始界面BGM未停止
		switch (choice) {
			case 1:
				temp = _menu();
				break;
			case 2:
				temp = _menu_RPG();
				break;
			case 3:
				temp = menu_double();
				break;
			case 4:
				settings();
				temp = 1;
				break;
			case 5:
				localLog();
				temp = 1;
				break;
			case 0:
				return 0;  //quit

		}
		
		switch (temp) {
			case -1:
			case 0:
				return temp;
			case 1: {
				cleardevice();
				if (BGM_ON) {
					mciSendString(_T("open bin/music/start.mp3 alias STARTBGM"), NULL, 0, NULL);
					mciSendString(_T("play STARTBGM repeat"), NULL, 0, NULL);
				}
				break;
			}
		}
	}
	return 0;
}

//0 - menu 1- replay 2 - quit
int Game::gameOver(bool is_RPG)
{
	clearMap();
	setfillcolor(BLACK);
	setlinecolor(BLACK);
	fillrectangle(36, 36, 564, 564);

	IMAGE END;
	loadimage(&END, (is_RPG ? _T("bin/image/3win.jpg") : _T("bin/image/die.jpg")), 520);
	putimage(40, 225, &END);

	Button bu[3] = {
		{_T("返回菜单"),255,375,345,420},
		{_T("再来一局"),255,420,345,465},
		{(is_RPG ? _T("继续前进") : _T("润了润了")),255,465,345,510},
	};

	int ret = menu_button(bu, 3);
	if (ret == 0 || ret == 3)
		ret = 2;
	else
		ret--;
	return ret;
}

//0 - menu 1- replay 2 - quit
int Game::gameOver_double(int winner)
{
	clearMap();
	setfillcolor(BLACK);
	setlinecolor(BLACK);
	fillrectangle(36, 36, 564, 564);

	IMAGE END;
	if (winner == 1) 
		loadimage(&END, _T("bin/image/1win.jpg"), 520);
	
	else if (winner == 2) 
		loadimage(&END, _T("bin/image/2win.jpg"), 520);
	
	else 
		loadimage(&END, _T("bin/image/draw.jpg"), 520);
	
	putimage(40, 225, &END);

	Button bu[3] = {
		{_T("返回菜单"),255,375,345,420},
		{_T("再来一局"),255,420,345,465},
		{_T("润了润了"),255,465,345,510},
	};

	int ret = menu_button(bu, 3);
	if (ret == 0 || ret == 3)
		ret = 2;
	else
		ret--;

	return ret;
}

/*显示对话框 speaker 0-tips 1-snake 2-tanished 3-jar 4-rykard*/
void Game::showDia(int speaker, LPCTSTR content, int yDia)
{
	const int y0 = 40, x0 = 40, left = 100, right = 500, d = 50;

	IMAGE SPE;
	switch (speaker) {
		case 0:
			loadimage(&SPE, _T("bin/image/null.jpg"), d, d);
			break;
		case 1:
			loadimage(&SPE, _T("bin/image/head.jpg"), d, d);
			break;
		case 2:
			loadimage(&SPE, _T("bin/image/tanished.jpg"), d, d);
			break;
		case 3:
			loadimage(&SPE, _T("bin/image/jar.jpg"), d, d);
			break;
		case 4:
			loadimage(&SPE, _T("bin/image/rykard.jpg"), d, d);
			break;
	}

	int yPos = yDia * d + y0;
	putimage(x0, yPos, &SPE);
	RECT r = { left,yPos,right,yPos + d };
	setlinecolor(DARKGRAY);
	setfillcolor(TRANSPARENT);
	fillrectangle(r.left, r.top, r.right, r.bottom);

	settextcolor(WHITE);
	LOGFONT f;
	gettextstyle(&f);
	f.lfHeight = 20;
	f.lfWidth = 10;
	f.lfWeight = 0;
	settextstyle(&f);
	drawtext(content, &r, DT_WORDBREAK | DT_NOCLIP | DT_LEFT | DT_VCENTER);
	getchar();
}

void Game::showGameUI_RPG(int level)
{
	cleardevice();
	putimage(0, 0, &IM_BK);
	for (int i = 0; i < MAP_A * MAP_A; i++) {
		if (map[i] == MAP_WALL)
			showPointWall(i % MAP_A, i / MAP_A);
		else if (map[i] == MAP_VOID)
			showPointVoid(i % MAP_A, i / MAP_A);
	}
	setlinecolor(DARKGRAY);
	rectangle(620, 20, 940, 580);
	switch (level) {
		case 1:
			showModeTitle(_T("故事模式-1.二次呼吸"));
			break;
		case 2:
			showModeTitle(_T("故事模式-2.靶向药物"));
			break;
		case 3:
			showModeTitle(_T("故事模式-3.苦难摇篮"));
			break;
		case 4:
			showModeTitle(_T("故事模式-4.怒号光明"));
			break;
		case 5:
			showModeTitle(_T("故事模式-5.破碎日冕"));
			break;
	}

	showTime();

}

void Game::setGuard(int x, int y, int dir)
{
	IMAGE IM_GUARD;
	loadimage(&IM_GUARD, _T("bin/image/enemy.png"), 16, 16);
	IMAGE IM_RO;
	int i, j, x1, y1;
	switch (dir) {
		case DIR_UP:
			rotateimage(&IM_RO, &IM_GUARD, -PI / 2);
			for (i = 0; i < 4; i++) {
				for (j = -1; j < 2; j++) {
					x1 = x + j;
					y1 = y - i;
					if (x1 >= 0 && x1 <= 34 && y1 >= 0 && y1 <= 34) {
						setPoint(x1, y1, MAP_LIGHT); //light和wall是一样的
						showPointLight(x1, y1);
					}
				}
			}
			break;
		case DIR_DOWN:
			rotateimage(&IM_RO, &IM_GUARD, PI / 2);
			for (i = 0; i < 4; i++) {
				for (j = -1; j < 2; j++) {
					x1 = x + j;
					y1 = y + i;
					if (x1 >= 0 && x1 <= 34 && y1 >= 0 && y1 <= 34) {
						setPoint(x1, y1, MAP_LIGHT); 
						showPointLight(x1, y1);
					}
				}
			}
			break;
		case DIR_LEFT:
			rotateimage(&IM_RO, &IM_GUARD, 0);
			for (i = 0; i < 4; i++) {
				for (j = -1; j < 2; j++) {
					x1 = x - i;
					y1 = y + j;
					if (x1 >= 0 && x1 <= 34 && y1 >= 0 && y1 <= 34) {
						setPoint(x1, y1, MAP_LIGHT); 
						showPointLight(x1, y1);
					}
				}
			}
			break;
		case DIR_RIGHT:
			rotateimage(&IM_RO, &IM_GUARD, PI);
			for (i = 0; i < 4; i++) {
				for (j = -1; j < 2; j++) {
					x1 = x + i;
					y1 = y + j;
					if (x1 >= 0 && x1 <= 34 && y1 >= 0 && y1 <= 34) {
						setPoint(x1, y1, MAP_LIGHT);
						showPointLight(x1, y1);
					}
				}
			}
			break;
	}
	putimage(x * 16 + 20, y * 16 + 20, &IM_RO);
}

void Game::drawHP_RPG(double hp)
{
	setfillcolor(RGB(116, 49, 33));
	setlinecolor(BLACK);

	int max_len = 880 - 640 - 4;
	int now_len = int(hp * max_len / 10);

	fillrectangle(642, 252, 642 + now_len, 268);

	setfillcolor(BLACK);
	fillrectangle(642 + now_len, 252, 878, 268);
}

void Game::updateHP_RPG(int before, int after)
{
	double hp = before;
	if (hp >= after) {
		while (fabs(hp - after) > 1e-6) {
			drawHP_RPG(hp);
			hp -= 0.2;
			Sleep(2);
		}
	}
	else {
		while (fabs(hp - after) > 1e-6) {
			drawHP_RPG(hp);
			hp += 0.2;
			Sleep(2);
		}
	}
}

void Game::alarm(Snake& snake, Food& food, SP& sp, Bomb& bomb)
{
	RECT r[4] = { {1,16,13,18},{21,16,33,18},{16,1,18,13},{16,21,18,33} };
	setfillcolor(RED);
	for (int i = 0; i < 4; i++) {
		fillrectangle(r[i].left*16+20, r[i].top * 16 + 20, r[i].right * 16 + 20, r[i].bottom * 16 + 20);
	}
	Sleep(500);
	for (int i = 0; i < 4; i++) {
		int le = r[i].left, ri = r[i].right, to = r[i].top, bo = r[i].bottom;
		for (int x = le; x <= ri; x++) {
			for (int y = to; y <= bo; y++) {
				showPointVoid(x, y);
			}
		}
	}
	snake.drawSnake();
	food.draw();
	bomb.draw();
	sp.draw();
}

bool Game::is_mapFull()
{
	for (int n = 0; n < MAP_MAX; n++) {
		if (map[n] == MAP_VOID)
			return false;
	}
	return true;
}

//type=1 normal food  type=2 SP type=-1 bomb type=0 hitwall
void Game::updateScore(int type, bool clear)
{
	if (clear) {
		score = 0;
		showScore(0);
	}
	else { //normal - 10
		int add = 0;
		switch (type) {
			case 1:
				add = 10;
				break;
			case 2:
				add = 100;
				break;
			case -1:
				add = -30;
				break;
			case 0:
				add = -50;
				break;
		}
		//int add = (type == 1 ? 10 : 100);
		score += int( add * score_power);
		showScore(add);
	}
}

void Game::showScore(int add)
{
	LOGFONT f;
	gettextstyle(&f);						
	f.lfHeight = 45;
	f.lfWidth = 22;
	f.lfItalic = false;
	_tcscpy_s(f.lfFaceName, _T("Constantia"));		
	f.lfQuality = ANTIALIASED_QUALITY;		
	settextstyle(&f);	
	settextcolor(RGB(255, 202, 24));

	RECT r_s = { 620,200,800,245 };
	drawtext(_T("Score:"), &r_s, DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_NOCLIP);

	setfillcolor(BLACK);
	setlinecolor(BLACK);
	fillrectangle(630, 240, 800, 300);
	//总分
	RECT r_d = { 620,225,800,300 };
	TCHAR s[20];
	_stprintf_s(s, _T("%d"), score);
	f.lfItalic = true;
	f.lfHeight = 75;
	f.lfWidth = 30;
	settextstyle(&f);
	drawtext(s, &r_d, DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_NOCLIP);

	//倍率
	if (add) {
		RECT r_p = { 850,260,930,280 };
		setfillcolor(BLACK);
		setlinecolor(BLACK);
		fillrectangle(r_p.left, r_p.top, r_p.right, r_p.bottom);

		TCHAR s2[30];
		if (add > 0)
			_stprintf_s(s2, _T("+%d*%.1lf"), add, score_power);
		else
			_stprintf_s(s2, _T("%d*%.1lf"), add, score_power);
		f.lfHeight = 20;
		f.lfWidth = 10;
		f.lfItalic = false;
		settextstyle(&f);
		
		drawtext(s2, &r_p, DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_NOCLIP);
	}
}

/*标准模式UI, mode=1,2,3*/
void Game::showGameUI(int mode)
{
	cleardevice();
	putimage(0, 0, &IM_BK);
	for (int i = 0; i < MAP_A * MAP_A; i++) {
		if (map[i] == MAP_WALL)
			showPointWall(i % MAP_A, i / MAP_A);
		else if(map[i]==MAP_VOID)
			showPointVoid(i % MAP_A, i / MAP_A);
	}


	setlinecolor(DARKGRAY);
	rectangle(620, 20, 940, 580);
	
	//show title
	switch (mode) {
		case 1:
			showModeTitle(_T("标准模式-正常模式"));
			break;
		case 2:
			showModeTitle(_T("标准模式-后人受苦"));
			break;
		case 3:
			showModeTitle(_T("标准模式-后人乘凉"));
			break;
	}

	IMAGE IM_FADIAN;
	loadimage(&IM_FADIAN, _T("bin/image/fadian.jpg"), 100, 117);
	putimage(830, 30, &IM_FADIAN);

	showItemRules();

	showTime();
	showScore(0);
}

void Game::showScore_double(int score, int player)
{
	LOGFONT f;
	gettextstyle(&f);
	f.lfHeight = 45;
	f.lfWidth = 22;
	f.lfItalic = false;
	_tcscpy_s(f.lfFaceName, _T("Constantia"));
	f.lfQuality = ANTIALIASED_QUALITY;
	settextstyle(&f);

	RECT r_s, r_d;
	COLORREF color = BLACK;

	if (player == 1) {
		color = RGB(255, 202, 24);
		r_s = { 620,100,800,145 };
		r_d = { 620,125,800,200 };
	}
	else {
		color = RGB(138, 45, 40);
		r_s = { 620,200,800,245 };
		r_d = { 620,225,800,300 };
	}

	setcolor(color);
	drawtext(_T("Score:"), &r_s, DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_NOCLIP);

	setfillcolor(BLACK);
	setlinecolor(BLACK);
	fillrectangle(630, r_d.top + 15, 800, r_d.bottom);

	TCHAR s[20];
	_stprintf_s(s, _T("%d"), score * 100);
	f.lfItalic = true;
	f.lfHeight = 75;
	f.lfWidth = 30;
	settextstyle(&f);
	drawtext(s, &r_d, DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_NOCLIP);

}

void Game::showGameUI_double()
{
	cleardevice();
	putimage(0, 0, &IM_BK);
	for (int i = 0; i < MAP_A * MAP_A; i++) {
		if (map[i] == MAP_WALL)
			showPointWall(i % MAP_A, i / MAP_A);
		else if (map[i] == MAP_VOID)
			showPointVoid(i % MAP_A, i / MAP_A);
	}
	setlinecolor(DARKGRAY);
	setfillcolor(BLACK);
	rectangle(620, 20, 940, 580);

	IMAGE DSNAKE;
	loadimage(&DSNAKE, _T("bin/image/doublesnake.jpg"), 120, 180);
	putimage(810, 50, &DSNAKE);

	showModeTitle(_T("双人对决"));
	showItemRules_double();
	showTime();
}

void Game::showItemRules_double()
{
	IMAGE _IM_FOOD, _IM_SNAKE1, _IM_SNAKE2, _IM_BEL;
	RECT r_i[4] = {
		{640,325,900,385},  //60 each
		{640,385,900,445},
		{640,445,900,505},
		{640,505,900,565}
	};

	loadimage(&_IM_FOOD, _T("bin/image/luen.png"), 50, 50);  //50*50 each
	loadimage(&_IM_SNAKE1, _T("bin/image/head1.jpg"), 50, 50);
	loadimage(&_IM_SNAKE2, _T("bin/image/head2.jpg"), 50, 50);
	loadimage(&_IM_BEL, _T("bin/image/bel.jpg"), 50, 60);

	putimage(r_i[0].left, r_i[0].top + 5, &_IM_FOOD);
	RECT r_temp = { r_i[0].left + 60,r_i[0].top + 5,r_i[0].right,r_i[0].bottom - 5 };
	menu_show_detail(_T("黄金卢恩，为人所渴望的力量，使你变得更加强大(指变长)与富有(指加分)"), &r_temp);

	putimage(r_i[1].left, r_i[1].top + 5, &_IM_SNAKE1);
	r_temp = { r_i[1].left + 60,r_i[1].top + 5,r_i[1].right,r_i[1].bottom - 5 };
	menu_show_detail(_T("黄色方(Player1)，用WASD控制方向移动"), &r_temp);

	putimage(r_i[2].left, r_i[2].top + 5, &_IM_SNAKE2);
	r_temp = { r_i[2].left + 60,r_i[2].top + 5,r_i[2].right,r_i[2].bottom - 5 };
	menu_show_detail(_T("红色方(Player2)，用↑←↓→控制方向移动"), &r_temp);

	putimage(r_i[3].left, r_i[3].top + 5, &_IM_BEL);
	r_temp = { r_i[3].left + 60,r_i[3].top + 5,r_i[3].right,r_i[3].bottom - 5 };
	menu_show_detail(_T("ESC暂停，ENTER继续；游戏开始时自动暂停；先创墙/蛇而死的为败方，头对头互创则比较分数"), &r_temp);
}

bool Game::is_headToHead(Snake& snake1, Snake& snake2)
{
	int x1 = snake1.getHeadX(), y1 = snake1.getHeadY();
	int x2 = snake2.getHeadX(), y2 = snake2.getHeadY();

	if (x1 - x2 == 1 && y1 == y2 && snake1.dir == DIR_LEFT && snake2.dir == DIR_RIGHT)
		return true;
	else if (x2 - x1 == 1 && y1 == y2 && snake2.dir == DIR_LEFT && snake1.dir == DIR_RIGHT)
		return true;
	else if (y1 - y2 == 1 && x1 == x2 && snake1.dir == DIR_DOWN && snake2.dir == DIR_UP)
		return true;
	else if (y2 - y1 == 1 && x1 == x2 && snake2.dir == DIR_DOWN && snake1.dir == DIR_UP)
		return true;
	else
		return false;
}

/*选择关卡的二级菜单 0 -quit 1 -menu*/
int Game::_menu_RPG()
{
	cleardevice();
	putimage(0, 0, &IM_START);

	Button bu[6] = {
		{_T("1.二次呼吸"),150,270,240,315},
		{_T("2.靶向药物"),150,315,240,360},
		{_T("3.苦难摇篮"),150,360,240,405},
		{_T("4.怒号光明"),150,405,240,450},
		{_T("5.破碎日冕"),150,450,240,495},
		{_T("退 出"),150,495,240,540}
	};

	//details
	LPCTSTR str[5] = {
		_T("你复活于封印监牢旁……"),
		_T("收集黄金卢恩……重获力量"),
		_T("在大赐福的圆桌厅堂中潜行与窃取……"),
		_T("尝试战斗！"),
		_T("一库走，Snake!向“亵渎君王”拉卡德展现你的力量！")
	};
	RECT r_detail = { 740,320,840,540 };

	Sleep(200);

	int ret = 0;
	bool flag = 1;
	int focus = 0;
	//show buttons
	bu[0].changeCond(1);

	for (int i = 0; i < 6; i++) {
		bu[i].draw();
	}

	while (flag) {
		for (int i = 0; i < 6 && flag; i++) {
			if (_kbhit()) {
				int ch = _getch();
				if (ch == '\r') {
					bu[focus].changeCond(2);
					Sleep(500);
					if (focus != 5) {
						if (BGM_ON) {
							mciSendString(_T("stop STARTBGM"), NULL, 0, NULL);
							mciSendString(_T("close STARTBGM"), NULL, 0, NULL);
						}
						while (true) {
							int temp1 = RPG(focus + 1);
							if (temp1 == 1) //replay
								continue;
							else if (temp1 == 2) {
								ret = 0;
								flag = 0;
								break;
							}
							else {  //0-menu
								ret = 1;
								flag = 0;
								break;
							}
						}
					}
					else { //Quit
						ret = 1;
						flag = 0;
						break;
					}
				}
				else if (ch == 27) {
					bu[focus].changeCond(0);
					bu[5].changeCond(2);
					ret = 1;
					flag = 0;
					break;
				}
			}

			int temp = bu[i].hasChanged();
			if (temp == 0)
				bu[i].draw();
			else if (temp == 1) {
				bu[i].draw();
				focus = i;
			}
			else if (temp == 2) {
				bu[i].draw();
				Sleep(500);
				if (i != 5) {
					if (BGM_ON) {
						mciSendString(_T("stop STARTBGM"), NULL, 0, NULL);
						mciSendString(_T("close STARTBGM"), NULL, 0, NULL);
					}
					while (true) {
						int temp1 = RPG(i + 1);
						if (temp1 == 1) //replay
							continue;
						else if (temp1 == 2) {
							ret = 0;
							flag = 0;
							break;
						}
						else {  //0-menu
							ret = 1;
							flag = 0;
							break;
						}
					}
				}
				else {
					ret = 1;
					flag = 0;
					break;
				}
			}
		}
		if (focus != 5)
			menu_show_detail(str[focus], &r_detail);
	}

	return ret;
}

/*-1 -error  0 -menu 1 -replay 2-quit*/
int Game::RPG(int level)
{
	int temp = 0;
	switch (level) {
		case 1:
			while (true) {
				temp = RPG_G1();
				if (temp == -1 || temp == 0 || temp == 2)
					return temp;
				else if (temp == 1)
					continue;
				else
					break;
			}
			//case后不break,便于直接进入下一关
		case 2:
			while (true) {
				temp = RPG_G2();
				if (temp == -1 || temp == 0 || temp == 2)
					return temp;
				else if (temp == 1)
					continue;
				else
					break;
			}
		case 3:
			while (true) {
				temp = RPG_G3();
				if (temp == -1 || temp == 0 || temp == 2)
					return temp;
				else if (temp == 1)
					continue;
				else
					break;
			}
		case 4:
			while (true) {
				temp = RPG_G4();
				if (temp == -1 || temp == 0 || temp == 2)
					return temp;
				else if (temp == 1)
					continue;
				else
					break;
			}
		case 5:
			while (true) {
				temp = RPG_G5();
				if (temp == -1 || temp == 0 || temp == 2)
					return temp;
				else if (temp == 1)
					continue;
				else {
					cleardevice();
					putimage(0, 0, &IM_BK);
					RECT rr = { 0,0,960,600 };
					drawtext(_T("即将返回菜单..."), &rr, DT_CENTER | DT_VCENTER);
					Sleep(500);
					break;
				}
			}
	}
	return 0;
}

/*-1-error 0-menu 1-replay 2-quit 3 -next*/
int Game::RPG_G1()
{
	if (BGM_ON) {
		mciSendString(_T("open bin/music/bk_nor.mp3 alias BGM"), NULL, 0, NULL);
		mciSendString(_T("play BGM repeat"), NULL, 0, NULL);
	}

	cleardevice();
	putimage(0, 0, &IM_BK);
	showDia(0, _T("按回车进入下一段对话..."), 0);
	showDia(0, _T("封印监牢旁。"), 0);
	showDia(1, _T("……什么动静？哦，是褪色者(Tanished)在封印监牢里与熔炉骑士战斗。"), 1);
	showDia(1, _T("……听起来褪色者好像又寄了。还是赶紧离开吧，免得一会他复活了来拿我撒气。"), 2);
	showDia(2, _T("****，就差一刀，找个石头蛇撒撒气……嗯？跑哪去了？让我找找。"), 3);
	showDia(0, _T("WASD改变移动方向，按回车开始游戏。目标：到达地图最右侧缺口处。"), 4);

	initMap(MAP_G1);
	showGameUI_RPG(1);
	showMsg(_T("WASD改变移动方向，按回车开始游戏。目标：到达地图最右侧缺口处。"));

	Snake* snake = new(std::nothrow) Snake(3, 16, 1, 8, 8);
	if (snake == NULL)
		return -1;
	snake->setInMap(*this);
	snake->drawHP();
	snake->drawLife();

	//右侧缺口（map中仍记为墙）
	showPointVoid(34, 16);
	showPointVoid(34, 17);
	showPointVoid(34, 18);

	getchar();
	last_time = 0;
	time(&start_time);
	
	bool success = true;
	while (true) {
		if (!snake->changeDir()) {
			//ESC暂停时
			showTime();  //刷新计时
			getchar();
			time(&start_time); //重置计时起点
		}

		if (!snake->move(*this)) {
			//右侧缺口
			if (snake->getHeadX() == 33 && snake->getHeadY() >= 16 && snake->getHeadY() <= 18) {
				success = true;
			}
			else {
				snake->loseLife();
				success = false;
			}
			break;
		}

		showTime();
		Sleep(100);
	}
	
	delete snake;

	int ret = 0;
	if (success) {
		ret = gameOver(1);
		if (ret == 2)
			ret = 3; //next level
	}
	else {
		ret = gameOver();
	}

	
	clearMap();
	if (BGM_ON) {
		mciSendString(_T("stop BGM"), NULL, 0, NULL);
		mciSendString(_T("close BGM"), NULL, 0, NULL);
	}
	return ret;
}

int Game::RPG_G2()
{
	if (BGM_ON) {
		mciSendString(_T("open bin/music/bk_nor.mp3 alias BGM"), NULL, 0, NULL);
		mciSendString(_T("play BGM repeat"), NULL, 0, NULL);
	}

	cleardevice();
	putimage(0, 0, &IM_BK);
	showDia(0, _T("按回车进入下一段对话..."), 0);
	showDia(0, _T("LIMGRAVE一处，满地都是褪色者死亡留下的血迹。"), 0);
	showDia(1, _T("太弱小了，没有力量，迟早被路过的褪色者送走。"), 1);
	showDia(1, _T("这帮铸币褪色者怎么在新手村都能死这么多次，满地都是黄金卢恩。"), 2);
	showDia(1, _T("把这些收集起来，我应该也能变强……吧？不管了先做了再说。"), 3);
	showDia(1, _T("那边那道墙看起来倒是不怎么坚固的样子，应该可以撞开吧。"), 4);
	showDia(0, _T("按回车开始游戏。目标：收集全部黄金卢恩并从右侧撞软墙离开(收集够之后出现软墙)。"), 5);

	initMap(MAP_G2);
	showGameUI_RPG(2);
	showMsg(_T("按回车开始游戏。目标：收集全部黄金卢恩并从右侧撞软墙离开(收集够之后出现软墙)。"));
	int foodNum = 5;

	Snake* snake = new(std::nothrow) Snake(3, 16, 1, 8, 17);
	if (snake == NULL)
		return -1;
	Food* food = new(std::nothrow) Food(foodNum);
	if (food == NULL)
		return -1;

	snake->setInMap(*this);
	snake->drawHP();
	snake->drawLife();

	for (int i = 0; i < foodNum; i++)
		food->create(*this);

	getchar();
	last_time = 0;
	time(&start_time);

	bool success = true;
	bool actiWall = false;
	while (true) {
		if (!snake->changeDir()) {
			//ESC暂停时
			showTime();  //刷新计时
			getchar();
			time(&start_time); //重置计时起点
		}

		if (!snake->move(*this)) {
			//右侧缺口
			if (actiWall && snake->getHeadX() == 33 
				&& snake->getHeadY() >= 16 && snake->getHeadY() <= 18) {
				snake->halfCut(*this);
				success = true;
			}
			else {
				snake->loseLife();
				success = false;
			}
			break;
		}

		if (snake->eatFood(*food)) {
			snake->longer(*this);
			if (--foodNum == 0) {
				showPointWeak(34, 16);
				showPointWeak(34, 17);
				showPointWeak(34, 18);
				actiWall = true;
			}
		}

		showTime();
		Sleep(100);
	}

	delete snake;
	delete food;

	int ret = 0;
	if (success) {
		Sleep(500);
		ret = gameOver(1);
		if (ret == 2)
			ret = 3; //next level
	}
	else {
		ret = gameOver();
	}

	clearMap();
	if (BGM_ON) {
		mciSendString(_T("stop BGM"), NULL, 0, NULL);
		mciSendString(_T("close BGM"), NULL, 0, NULL);
	}
	return ret;
}

int Game::RPG_G3()
{
	if (BGM_ON) {
		mciSendString(_T("open bin/music/bk_nor.mp3 alias BGM"), NULL, 0, NULL);
		mciSendString(_T("play BGM repeat"), NULL, 0, NULL);
	}

	cleardevice();
	putimage(0, 0, &IM_BK);
	showDia(0, _T("按回车进入下一段对话..."), 0);
	showDia(0, _T("未知地点。"), 0);
	showDia(1, _T("撞墙还是疼啊，直接给我撞掉一半，但总归是逃出来了。诶，这地方怎么有点眼熟？"), 1);
	showDia(1, _T("**……怎么误打误撞跑到圆桌了，这可是褪色者的大本营啊…"), 2);
	showDia(0, _T("大赐福-圆桌厅堂。"), 0);
	showDia(1, _T("不过这帮被揍得失了智的家伙在这里应该不会太警觉，正好让我趁机偷走他们的力量(卢恩)。"), 3);
	showDia(0, _T("按回车开始游戏。目标：在不被发现的情况先收集所有卢恩，随后撞软墙逃离"), 4);

	initMap(MAP_G3);
	showGameUI_RPG(3);
	showMsg(_T("按回车开始游戏。目标：在不被发现的情况先收集所有卢恩，随后撞软墙逃离(收集够之后出现软墙)。"));
	int foodNum = 6;

	Snake* snake = new(std::nothrow) Snake(4, 16, 1, 8, 8);
	if (snake == NULL)
		return -1;
	Food* food = new(std::nothrow) Food(foodNum);
	if (food == NULL)
		return -1;

	snake->setInMap(*this);
	snake->drawHP();
	snake->drawLife();

	setGuard(8, 14, DIR_LEFT);
	setGuard(33, 12, DIR_LEFT);
	setGuard(32, 33, DIR_UP);
	setGuard(17, 11, DIR_DOWN);
	setGuard(14, 20, DIR_DOWN);
	setGuard(10, 5, DIR_RIGHT);
	setGuard(14, 5, DIR_LEFT);
	setGuard(4, 33, DIR_UP);
	setGuard(5, 27, DIR_DOWN);
	setGuard(20, 17, DIR_RIGHT);

	for (int i = 0; i < foodNum; i++)
		food->create(*this);

	getchar();
	last_time = 0;
	time(&start_time);

	bool success = true;
	bool actiWall = false;
	while (true) {
		if (!snake->changeDir()) {
			//ESC暂停时
			showTime();  //刷新计时
			getchar();
			time(&start_time); //重置计时起点
		}

		if (!snake->move(*this)) {
			//右侧缺口
			if (actiWall && snake->getHeadX() == 33
				&& snake->getHeadY() >= 16 && snake->getHeadY() <= 18) {
				snake->halfCut(*this);
				success = true;
			}
			else {
				snake->loseLife();
				success = false;
			}
			break;
		}

		if (snake->eatFood(*food)) {
			snake->longer(*this);
			if (--foodNum == 0) {
				showPointWeak(34, 16);
				showPointWeak(34, 17);
				showPointWeak(34, 18);
				actiWall = true;
			}
		}

		showTime();
		Sleep(100);
	}

	delete snake;
	delete food;

	int ret = 0;
	if (success) {
		Sleep(500);
		ret = gameOver(1);
		if (ret == 2)
			ret = 3; //next level
	}
	else {
		ret = gameOver();
	}

	clearMap();
	if (BGM_ON) {
		mciSendString(_T("stop BGM"), NULL, 0, NULL);
		mciSendString(_T("close BGM"), NULL, 0, NULL);
	}
	return ret;
}

int Game::RPG_G4()
{
	if (BGM_ON) {
		mciSendString(_T("open bin/music/bk_nor.mp3 alias BGM"), NULL, 0, NULL);
		mciSendString(_T("play BGM repeat"), NULL, 0, NULL);
	}

	cleardevice();
	putimage(0, 0, &IM_BK);
	showDia(0, _T("按回车进入下一段对话..."), 0);
	showDia(0, _T("圆桌外。两名褪色者路过。"), 0);
	showDia(2, _T("褪色者甲：什么狗屁艾尔登之王，爷不当了，让那帮大龙虾还有熔炉骑士什么的去当吧。"), 1);
	showDia(2, _T("褪色者乙：就是就是，也没人说过只有褪色者才能成为艾尔登之王吧，我**谁啊还当。"), 2);
	showDia(2, _T("褪色者甲：只可惜这一身白狼战鬼，但凡我有巴格莱姆前辈十分之一厉害也不至于这么菜。"), 3);
	showDia(2, _T("褪色者乙：听说最后的大卢恩的所有者\"亵渎君王\"拉卡德是个…人蛇杂交还是人蛇合体？"), 4);
	showDia(2, _T("褪色者甲：Who cares?那照你这么说也许一只蛇能轻轻松松干掉他，同性相斥嘛。"), 5);
	showDia(2, _T("褪色者乙：哈哈，我倒真希望看到……"), 6);
	showDia(0, _T("二人走远了。"), 7);
	showDia(1, _T("……"), 8);

	cleardevice();
	putimage(0, 0, &IM_BK);
	showDia(0, _T("LIMGRAVE一处平地。"), 0);
	showDia(3, _T("不是我说，你练功就安安静静练，非要叫上我干嘛？"), 1);
	showDia(1, _T("没有对象没有靶子能练出来个啥，还是说我去找个褪色者送死？"), 2);
	showDia(1, _T("反正你自我修复能力那么强，我又这么菜，对你也没什么威胁不是，战士壶亚历山大？"), 3);
	showDia(3, _T("……随你怎么说吧，赶紧的，我一会儿还要去火山那边锻炼抗火性能。"), 4);
	showDia(1, _T("好嘞，我绝对控制好度。正好我也要去火山，一会儿也一起走吧。"), 5);
	showDia(3, _T("赶紧赶紧，时间要是太久了我就直接动手了"), 6);
	showDia(0, _T("解锁了能力：攻击！收集散落在地上的卢恩获得充能，按F进行攻击，最多充能2次。"), 7);
	showDia(0, _T("按回车键继续游戏。目标：在一分钟内对战士壶进行不少于十次攻击。"), 8);

	initMap(MAP_G3);
	showGameUI_RPG(4);
	showMsg(_T("按回车键继续游戏。目标：在一分钟内对战士壶进行不少于十次攻击。(收集散落在地上的卢恩获得充能，按F进行攻击，最多充能2次)。"));
	IMAGE IM_JAR, IM_JAR1, IM_JAR2;
	loadimage(&IM_JAR, _T("bin/image/jar.jpg"), 80, 80);
	putimage(15 * 16 + 20, 15 * 16 + 20, &IM_JAR);
	loadimage(&IM_JAR1, _T("bin/image/jar.jpg"), 60, 60);
	loadimage(&IM_JAR2, _T("bin/image/jarhit.jpg"), 80, 80);
	putimage(642, 185, &IM_JAR1);
	setfillcolor(BLACK);
	setlinecolor(DARKGRAY);
	fillrectangle(640, 250, 880, 270);
	drawHP_RPG(10);

	Snake* snake = new(std::nothrow) Snake(4, 16, 1, 8, 8);
	if (snake == NULL)
		return -1;
	Food* food = new(std::nothrow) Food(1);
	if (food == NULL)
		return -1;

	snake->setInMap(*this);
	snake->drawHP();
	snake->drawLife();

	getchar();
	last_time = 0;
	time(&start_time);

	bool success = true;
	int charge = 0;
	int hit = 10;
	food->create(*this);
	while (true) {

		int temp = snake->changeDir();
		if (!temp) {
			//ESC暂停时
			showTime();  //刷新计时
			getchar();
			time(&start_time); //重置计时起点
		}
		else if (temp == 2) {
			if (charge > 0) {
				snake->changeHead(--charge);
				updateHP_RPG(hit, hit-1);

				setlinecolor(RED);
				setlinestyle(PS_SOLID| PS_ENDCAP_ROUND, 2);
				int x = snake->getHeadX(), y = snake->getHeadY();
				line(x*16+20, y * 16 + 20, 17 * 16 + 20, 17 * 16 + 20);
				putimage(15 * 16 + 20, 15 * 16 + 20, &IM_JAR2);
				Sleep(500);
				setlinecolor(BLACK);
				setlinestyle(PS_SOLID| PS_ENDCAP_ROUND, 3);
				line(x * 16 + 20, y * 16 + 20, 17 * 16 + 20, 17 * 16 + 20);
				putimage(15 * 16 + 20, 15 * 16 + 20, &IM_JAR);

				if (--hit == 0) {
					success = true;
					break;
				}
			}
		}

		//超时
		if (last_time >= 60) {
			success = false;
			break;
		}

		if (!snake->move(*this)) {		
			snake->loseLife();
			success = false;		
			break;
		}

		if (snake->eatFood(*food)) {
			snake->longer(*this);
			if (charge == 0 || charge == 1) {
				snake->changeHead(++charge);
			}
			if (food->getNum() == 0) {
				food->create(*this);
			}
		}

		showTime();
		Sleep(100);
	}

	delete snake;
	delete food;

	int ret = 0;
	if (success) {
		Sleep(500);
		ret = gameOver(1);
		if (ret == 2)
			ret = 3; //next level
	}
	else {
		ret = gameOver();
	}

	clearMap();
	if (BGM_ON) {
		mciSendString(_T("stop BGM"), NULL, 0, NULL);
		mciSendString(_T("close BGM"), NULL, 0, NULL);
	}
	return ret;
}

int Game::RPG_G5()
{
	cleardevice();
	putimage(0, 0, &IM_BK);
	showDia(0, _T("按回车进入下一段对话..."), 0);
	showDia(0, _T("火山官邸底部，觐见厅。"), 0);
	showDia(4, _T("来者何人？"), 1);
	showDia(4, _T("哦？餐后甜点么？还从来没有你这种生物来过这里。"), 2);
	showDia(4, _T("不过我倒是不挑食，来，让我品品。"), 3);
	showDia(0, _T("BIU~~HONG!"), 4);
	showDia(4, _T("啊呀…还敢还手？了不起，不过是临死前的挣扎罢了。"), 5);
	showDia(1, _T("攻高血薄的纸老虎罢了，我能战胜他的"), 6);
	showDia(0, _T("按回车继续游戏。目标：击败\"亵渎君王\"拉卡德"), 8);
	cleardevice();
	IMAGE BKIMG;

	if (BGM_ON) {
		mciSendString(_T("open bin/music/bk.mp3 alias BGM"), NULL, 0, NULL);
		mciSendString(_T("play BGM repeat"), NULL, 0, NULL);
	}

	loadimage(&BKIMG, _T("bin/image/scene.jpg"), 900, 500);
	putimage(30, 50, &BKIMG);
	Sleep(1000);

	initMap(MAP_G5);
	showGameUI_RPG(5);
	showMsg(_T("按回车继续游戏。目标：击败\"亵渎君王\"拉卡德(收集散落在地上的卢恩获得充能，按F进行攻击，最多充能2次)。"));
	IMAGE IM_RY, IM_RY1, IM_RY2;
	loadimage(&IM_RY, _T("bin/image/rykard.jpg"), 112, 112);
	loadimage(&IM_RY2, _T("bin/image/rykardhit.jpg"), 112, 112);
	putimage(14 * 16 + 20, 14 * 16 + 20, &IM_RY);
	loadimage(&IM_RY1, _T("bin/image/rykard.jpg"), 60, 60);
	putimage(642, 185, &IM_RY1);
	setfillcolor(BLACK);
	setlinecolor(DARKGRAY);
	fillrectangle(640, 250, 880, 270);
	drawHP_RPG(10);

	Snake* snake = new(std::nothrow) Snake(3, 16, 1, 8, 8);
	if (snake == NULL)
		return -1;
	Food* food = new(std::nothrow) Food(2);
	if (food == NULL)
		return -1;
	SP* sp = new(std::nothrow) SP();
	if (sp == NULL)
		return -1;
	Bomb* bomb = new(std::nothrow) Bomb(1);
	if (bomb == NULL)
		return -1;
	Snake* esnake = NULL;
	Snake* _esnake = NULL;

	snake->setInMap(*this);
	snake->drawHP();
	snake->drawLife();

	getchar();
	last_time = 0;
	time(&start_time);

	bool success = true;
	int loop_times = 0;
	int charge = 0;
	int hit = 10;
	food->create(*this);
	bomb->create(*this);
	sp->create(*this, 300); //infinite
	while (true) {
		/* rykard's attack*/
		if (loop_times == 40) {
			_esnake = new(std::nothrow) Snake[8]{
			{POINT{14,10},DIR_UP},{POINT{20,10},DIR_UP},
			{POINT{14,24},DIR_DOWN},{POINT{20,24},DIR_DOWN},
			{POINT{10,14},DIR_LEFT},{POINT{10,20},DIR_LEFT},
			{POINT{24,14},DIR_RIGHT},{POINT{24,20},DIR_RIGHT}
			};
			if (_esnake == NULL)
				return -1;
			for (int i = 0; i < 8; i++)
				_esnake[i].setInMap(*this);
		}
		else if (loop_times == 49) {
			for (int i = 0; i < 8; i++) {
				_esnake[i].turnToVoid(*this);
			}
			delete[] _esnake;
			_esnake = NULL;
		}
		else if (loop_times == 80) {
			esnake = new(std::nothrow) Snake[12]{
			{POINT{16,10},DIR_UP},{POINT{17,10},DIR_UP},{POINT{18,10},DIR_UP},
			{POINT{16,24},DIR_DOWN},{POINT{17,24},DIR_DOWN},{POINT{18,24},DIR_DOWN},
			{POINT{10,16},DIR_LEFT},{POINT{10,17},DIR_LEFT},{POINT{10,18},DIR_LEFT},
			{POINT{24,16},DIR_RIGHT},{POINT{24,17},DIR_RIGHT},{POINT{24,18},DIR_RIGHT}
			};
			if (esnake == NULL)
				return -1;
			for (int i = 0; i < 12; i++)
				esnake[i].setInMap(*this);
		}
		else if (loop_times == 89) {
			for (int i = 0; i < 12; i++) {
				esnake[i].turnToVoid(*this);
			}
			delete[] esnake;
			esnake = NULL;
			loop_times = 0;
		}

		if (esnake) {
			for (int i = 0; i < 12; i++) {
				if (esnake[i].body.empty())
					continue;
				if (esnake[i].eatFood(*food))
					food->create(*this);
				if (esnake[i].eatSP(*sp))
					sp->create(*this, 300);
				if (esnake[i].hitBomb(*bomb))
					bomb->create(*this);
				if (!esnake[i].move(*this)) {
					esnake[i].turnToVoid(*this);
					snake->loseHP(8);
					if (snake->is_dead()) {
						snake->loseLife();
						if (snake->is_gameOver()) {
							success = false;
							break;
						}
						snake->updateHP(0, 16);
					}					
				}
				
			}
		}
		if (snake->is_gameOver()) {
			success = false;
			break;
		}

		if (_esnake) {
			for (int i = 0; i < 8; i++) {
				if (_esnake[i].body.empty())
					continue;
				if (_esnake[i].eatFood(*food))
					food->create(*this);
				if (_esnake[i].eatSP(*sp))
					sp->create(*this, 300);
				if (_esnake[i].hitBomb(*bomb))
					bomb->create(*this);
				if (!_esnake[i].move(*this)) {
					_esnake[i].turnToVoid(*this);
					snake->loseHP(8);
					if (snake->is_dead()) {
						snake->loseLife();
						if (snake->is_gameOver()) {
							success = false;
							break;
						}
						snake->updateHP(0, 16);
					}
				}

			}
		}
		if (snake->is_gameOver()) {
			success = false;
			break;
		}

		int temp = snake->changeDir();
		if (!temp) {
			//ESC暂停时
			showTime();  //刷新计时
			getchar();
			time(&start_time); //重置计时起点
		}
		else if (temp == 2) {
			if (charge > 0) {
				snake->changeHead(--charge);
				updateHP_RPG(hit, hit - 1);

				setlinecolor(RED);
				setlinestyle(PS_SOLID | PS_ENDCAP_ROUND, 2);
				int x = snake->getHeadX(), y = snake->getHeadY();
				line(x * 16 + 20, y * 16 + 20, 17 * 16 + 20, 17 * 16 + 20);
				putimage(14 * 16 + 20, 14 * 16 + 20, &IM_RY2);
				Sleep(500);
				setlinecolor(BLACK);
				setlinestyle(PS_SOLID | PS_ENDCAP_ROUND, 3);
				line(x * 16 + 20, y * 16 + 20, 17 * 16 + 20, 17 * 16 + 20);
				putimage(14 * 16 + 20, 14 * 16 + 20, &IM_RY);

				if (--hit == 0) {
					success = true;
					break;
				}
			}
		}

		//超时
		if (last_time >= 300) {
			success = false;
			break;
		}

		if (!snake->move(*this)) {
			snake->loseLife();
			//改变头方向
			snake->revDir();
		}

		if (snake->is_gameOver()) {
			success = false;
			break;
		}

		if (snake->hitBomb(*bomb)) {
			snake->loseHP(4);
			if (snake->is_dead()) {
				snake->loseLife();
				if (snake->is_gameOver()) {
					success = false;
					break;
				}
				snake->updateHP(0, 16);
			}
		}

		if (snake->eatFood(*food)) {
			snake->longer(*this);
			if (charge == 0 || charge == 1) {
				snake->changeHead(++charge);
			}
		}

		if (snake->eatSP(*sp)) {			
			if (snake->life < 5)
				snake->updateLife(1);
		}		

		if (!sp->isExist())
			sp->create(*this, 300);
		if (food->getNum() < 2) 
			food->create(*this);
		if (bomb->getNum() == 0)
			bomb->create(*this);
		
		showTime();
		Sleep(100);
		loop_times++;
	}

	delete snake;
	delete food;
	delete sp;
	delete bomb;

	int ret = 0;
	if (success) {
		Sleep(500);
		ret = gameOver(1);
		if (ret == 2)
			ret = 3; //next level
	}
	else {
		ret = gameOver();
	}

	clearMap();
	if (BGM_ON) {
		mciSendString(_T("stop BGM"), NULL, 0, NULL);
		mciSendString(_T("close BGM"), NULL, 0, NULL);
	}
	return ret;
}

void Game::showMsg(LPCTSTR str)
{
	RECT r = { 640,325,900,565 };
	menu_show_detail(str, &r);
}

void Game::showPointWall(int x, int y)
{
	int x1, y1;
	x1 = 20 + x * 16 + 1;
	y1 = 20 + y * 16 + 1;

	putimage(x1, y1, &IM_WALL);
}

void Game::showPointVoid(int x, int y)
{
	COLORREF co = BK_COLOR;
	setfillcolor(co);
	setcolor(co);

	int x1, y1, x2, y2;
	x1 = 20 + x * 16;
	y1 = 20 + y * 16;
	x2 = x1 + 16;
	y2 = y1 + 16;
	fillrectangle(x1, y1, x2, y2);
}

void Game::showPointWeak(int x, int y)
{
	int x1, y1;
	x1 = 20 + x * 16 + 1;
	y1 = 20 + y * 16 + 1;
	IMAGE IM_WEAK;
	loadimage(&IM_WEAK, _T("bin/image/weakwall.jpg"), 14, 14);
	putimage(x1, y1, &IM_WEAK);
}

void Game::showPointLight(int x, int y)
{
	COLORREF co = RGB(245,90,90);
	setfillcolor(co);
	setlinecolor(co);

	int x1, y1, x2, y2;
	x1 = 20 + x * 16 + 1;
	y1 = 20 + y * 16 + 1;
	x2 = x1 + 16 - 2;
	y2 = y1 + 16 - 2;
	fillrectangle(x1, y1, x2, y2);
}

/*显示游戏开始后所用时间(不计暂停)*/
void Game::showTime()
{
	time_t t1, delta;
	time(&t1);
	delta = t1 - start_time;
	if (delta != 0) {
		last_time += delta;
		start_time = t1;  //重置计时，便于考虑暂停情况

		time_t min, hour, sec;
		sec = last_time % 60;
		min = last_time / 60;
		hour = min / 60;
		min = min % 60;

		TCHAR tc[15];
		_stprintf_s(tc, _T("Time: %02lld:%02lld:%02lld"), hour, min, sec);

		RECT r_t = { 640,305,750,325 };
		setfillcolor(BLACK);
		setlinecolor(BLACK);
		fillrectangle(r_t.left, r_t.top, r_t.right, r_t.bottom);

		LOGFONT f;
		gettextstyle(&f);
		settextcolor(LIGHTGRAY);
		_tcscpy_s(f.lfFaceName, _T("黑体"));
		f.lfItalic = false;
		f.lfHeight = 16;
		f.lfWidth = 8;
		f.lfWeight = 0;
		settextstyle(&f);
		drawtext(tc, &r_t, DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_NOCLIP);
	}
}

/*标准模式 右下方显示道具及其效果*/
void Game::showItemRules()
{
	IMAGE _IM_BOMB, _IM_SP, _IM_FOOD, _IM_BEL;
	RECT r_i[4] = {
		{640,325,900,385},  //60 each
		{640,385,900,445},
		{640,445,900,505},
		{640,505,900,565}
	};

	loadimage(&_IM_BOMB, _T("bin/image/dian.jpg"), 50, 50);  //50*50 each
	loadimage(&_IM_SP, _T("bin/image/sp.jpg"), 50, 50);
	loadimage(&_IM_FOOD, _T("bin/image/luen.png"), 50, 50);
	loadimage(&_IM_BEL, _T("bin/image/bel.jpg"), 50, 60);

	putimage(r_i[0].left, r_i[0].top + 5, &_IM_FOOD);
	RECT r_temp = { r_i[0].left + 60,r_i[0].top + 5,r_i[0].right,r_i[0].bottom - 5 };
	menu_show_detail(_T("黄金卢恩，为人所渴望的力量，使你变得更加强大(指变长)与富有(指加分)"), &r_temp);

	putimage(r_i[1].left, r_i[1].top + 5, &_IM_BOMB);
	r_temp = { r_i[1].left + 60,r_i[1].top + 5,r_i[1].right,r_i[1].bottom - 5 };
	menu_show_detail(_T("癫火，使接触者陷入癫狂(指掉血)，小木头梅琳娜提醒您，别在这理发店！"), &r_temp);

	putimage(r_i[2].left, r_i[2].top + 5, &_IM_SP);
	r_temp = { r_i[2].left + 60,r_i[2].top + 5,r_i[2].right,r_i[2].bottom - 5 };
	menu_show_detail(_T("圣杯瓶，不知为何竟是限时存在，然而无人能抵制住其诱惑(指加命，等)"), &r_temp);

	putimage(r_i[3].left, r_i[3].top + 5, &_IM_BEL);
	r_temp = { r_i[3].left + 60,r_i[3].top + 5,r_i[3].right,r_i[3].bottom - 5 };
	menu_show_detail(_T("WASD或↑←↓→控制移动，ESC暂停，ENTER继续；游戏开始与复活时自动暂停；道具效果会随难度设置而改变"), &r_temp);
}

void Game::showModeTitle(LPCTSTR title)
{
	RECT r_t = { 630,30,830,50 };

	settextcolor(RGB(150, 100, 0));
	LOGFONT f;
	gettextstyle(&f);
	f.lfHeight = 20;
	f.lfWidth = 10;
	f.lfWeight = FW_BLACK;
	_tcscpy_s(f.lfFaceName, _T("宋体"));
	f.lfQuality = ANTIALIASED_QUALITY;
	settextstyle(&f);

	drawtext(title, &r_t, DT_LEFT | DT_NOCLIP | DT_SINGLELINE | DT_VCENTER);
}

void Game::inputBox(RECT& r, char* str, int maxlen)
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

bool Game::is_pointEmpty(int x, int y) const
{
	return map[y * MAP_A + x] == MAP_VOID;
}

void Game::setPoint(int x, int y, int type)
{
	map[y * MAP_A + x] = type;
}

void Game::clearPoint(int x, int y)
{
	map[y * MAP_A + x] = MAP_VOID;
}
