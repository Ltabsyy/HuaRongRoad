#include <stdlib.h>
#include <time.h>
#include <graphics.h>

int difficulty = 10;
int sideLength = 96;
int board[10][10];
int rZero, cZero;

color_t color[19] = {
	WHITE,//0
	SALMON,//r1
	LIGHTRED,//c1
	GOLD,//r2
	ORANGE,//c2
	PALEGREEN,//r3
	SPRINGGREEN,//c3
	LIGHTSKYBLUE,//r4
	DEEPSKYBLUE,//c4
	ORCHID,//r5
	DARKORCHID,//c5
	PLUM,//r6
	MEDIUMPURPLE,//c6
	TAN,//r7
	SIENNA,//c7
	DARKGRAY,//r8
	BLACK,//c8
	WHITE,//r9
	LIGHTGRAY//c9
};

int Color(int n)
{
	int r, c;
	if(n == 0) return 0;
	n--;
	r = n / difficulty;
	c = n % difficulty;
	//0 r1 c1 r2 c2
	//0  1  2  3  4
	if(r == c) return 2*r+1;
	else if(r < c) return 2*r+1;
	else return 2*c+2;
}

void DrawSelection()
{
	int r, c, n;
	for(n=3; n<=10; n++)
	{
		r = (n-3)/4;
		c = (n-3)%4;
		if(n == 10) setcolor(WHITE);
		else setcolor(BLACK);
		setfillcolor(color[(n-2)*2]);
		ege_fillrect(c*sideLength, r*sideLength, sideLength, sideLength);
		if(n < 10) xyprintf(c*sideLength+sideLength/12+sideLength*7/32, r*sideLength+sideLength/16, "%d", n);
		else xyprintf(c*sideLength+sideLength/12, r*sideLength+sideLength/16, "%d", n);
	}
}

void DrawBoard()
{
	int r, c, n;
	for(r=0; r<difficulty; r++)
	{
		for(c=0; c<difficulty; c++)
		{
			//if(r == diffculty-1 && c == diffculty-1) n = 0;
			//else n = r*diffculty+c+1;
			n = board[r][c];
			if(Color(n) < 9 || Color(n) > 16) setcolor(BLACK);
			else setcolor(WHITE);
			setfillcolor(color[Color(n)]);
			ege_fillrect(c*sideLength, r*sideLength, sideLength, sideLength);
			if(n == 0);
			else if(n < 10) xyprintf(c*sideLength+sideLength/12+sideLength*7/32, r*sideLength+sideLength/16, "%d", n);
			else xyprintf(c*sideLength+sideLength/12, r*sideLength+sideLength/16, "%d", n);
		}
	}
}

void InitWindow()
{
	int screenHeight, screenWidth;
	DEVMODE dm;
	dm.dmSize = sizeof(DEVMODE);
	if(EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &dm) == 0)//无法获取显示屏分辨率
	{
		screenWidth = 1920;
		screenHeight = 1080;
	}
	else
	{
		screenWidth = dm.dmPelsWidth;
		screenHeight = dm.dmPelsHeight;
	}
	if(screenHeight >= 2160) sideLength = 192;
	else if(screenHeight >= 1440) sideLength = 128;
	else if(screenHeight >= 1080) sideLength = 96;
	else sideLength = 72;
	setcaption("HuaRong Road");
	SetProcessDPIAware();
	//3 4 5 6
	//7 8 9 10
	initgraph(sideLength*4, sideLength*2, INIT_RENDERMANUAL);
	setbkcolor(WHITE);
	setfont(sideLength*7/8, 0, "Consolas");
	setbkmode(TRANSPARENT);
	ege_enable_aa(true);
}

void InitBoard()
{
	int r, c, n;
	srand(time(0));
	for(r=0; r<difficulty; r++)
	{
		for(c=0; c<difficulty; c++)
		{
			board[r][c] = -1;
		}
	}
	rZero = rand() % difficulty;
	cZero = rand() % difficulty;
	board[rZero][cZero] = 0;
	for(n=1; n<difficulty*difficulty; )
	{
		r = rand() % difficulty;
		c = rand() % difficulty;
		if(board[r][c] == -1)
		{
			board[r][c] = n;
			n++;
		}
	}
}

void Move(int rt, int ct)//移动空白块到目标位置
{
	int r, c;
	if(rt == rZero)
	{
		if(ct < cZero && ct >= 0)
		{
			for(c=cZero; c>ct; c--)
			{
				board[rt][c] = board[rt][c-1];
			}
			board[rt][ct] = 0;
			cZero = ct;
		}
		else if(ct > cZero && ct < difficulty)
		{
			for(c=cZero; c<ct; c++)
			{
				board[rt][c] = board[rt][c+1];
			}
			board[rt][ct] = 0;
			cZero = ct;
		}
	}
	else if(ct == cZero)
	{
		if(rt < rZero && rt >= 0)
		{
			for(r=rZero; r>rt; r--)
			{
				board[r][ct] = board[r-1][ct];
			}
			board[rt][ct] = 0;
			rZero = rt;
		}
		else if(rt > rZero && rt < difficulty)
		{
			for(r=rZero; r<rt; r++)
			{
				board[r][ct] = board[r+1][ct];
			}
			board[rt][ct] = 0;
			rZero = rt;
		}
	}
}

int main()
{
	int r, c;
	mouse_msg mouseMsg;
	key_msg keyMsg;
	/*选择难度*/
	InitWindow();
	DrawSelection();
	difficulty = 0;
	while(difficulty == 0)
	{
		while(mousemsg())
		{
			mouseMsg = getmouse();
			if(mouseMsg.is_up())//选择难度
			{
				r = mouseMsg.y / sideLength;
				c = mouseMsg.x / sideLength;
				difficulty = 4*r+c+3;
			}
			if(mouseMsg.is_wheel() && keystate(key_control))//调整显示大小
			{
				if(mouseMsg.wheel > 0) sideLength += 8;
				else if(sideLength > 8) sideLength -= 8;
				resizewindow(sideLength*4, sideLength*2);
				setfont(sideLength*7/8, 0, "Consolas");//更新字体大小
				DrawSelection();
			}
		}
		while(kbmsg())
		{
			keyMsg = getkey();
			if(keyMsg.msg == key_msg_down)
			{
				if(keyMsg.key >= '3' && keyMsg.key <= '9')//选择难度
				{
					difficulty = keyMsg.key-'3'+3;
				}
				else if(keyMsg.key >= key_num3 && keyMsg.key <= key_num9)//小键盘识别
				{
					difficulty = keyMsg.key-key_num3+3;
				}
				else if(keyMsg.key == '0' || keyMsg.key == '1' || keyMsg.key == key_num0 || keyMsg.key == key_num1)
				{
					difficulty = 10;
				}
				else if(keyMsg.key == '2' || keyMsg.key == key_num2)
				{
					difficulty = 3;
				}
			}
		}
		delay_ms(50);
	}
	/*游戏*/
	InitBoard();
	resizewindow(sideLength*difficulty, sideLength*difficulty);
	DrawBoard();
	while(1)
	{
		while(mousemsg())
		{
			mouseMsg = getmouse();
			if(mouseMsg.is_down())//移动
			{
				r = mouseMsg.y / sideLength;
				c = mouseMsg.x / sideLength;
				Move(r, c);
				DrawBoard();
			}
			if(mouseMsg.is_wheel() && keystate(key_control))//调整显示大小
			{
				if(mouseMsg.wheel > 0) sideLength += 8;
				else if(sideLength > 8) sideLength -= 8;
				resizewindow(sideLength*difficulty, sideLength*difficulty);
				setfont(sideLength*7/8, 0, "Consolas");//更新字体大小
				DrawBoard();
			}
		}
		while(kbmsg())
		{
			keyMsg = getkey();
			if(keyMsg.msg == key_msg_down)//移动
			{
				if(keyMsg.key == 'W') Move(rZero-1, cZero);
				else if(keyMsg.key == 'A') Move(rZero, cZero-1);
				else if(keyMsg.key == 'S') Move(rZero+1, cZero);
				else if(keyMsg.key == 'D') Move(rZero, cZero+1);
				DrawBoard();
			}
		}
		delay_ms(50);
	}
	closegraph();
	return 0;
}

/*--------------------------------
更新日志：
HuaRongRoad 0.2
——新增 点击空白块的同行或同列方块时快速移动方块
——新增 根据屏幕分辨率调整显示大小
——新增 按WASD移动空白块
——新增 按数字键选择难度
——新增 Ctrl+滚轮调整显示大小
——优化 移动在鼠标任意键按下时触发
——优化 仅在移动后刷新界面
--------------------------------*/
