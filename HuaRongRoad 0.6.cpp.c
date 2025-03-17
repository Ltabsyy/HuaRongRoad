//#define _WIN32_WINNT 0xa00//使用SetProcessDPIAware()
#include <stdlib.h>
#include <time.h>
#include <graphics.h>

int difficulty = 10;
int sideLength = 96;
int** board = 0;
int rZero, cZero;
int colorStyle = 0;

#define heightOfChar sideLength*7/8//sideLength*7/16
#define xOfChar sideLength/12
#define dxOfChar heightOfChar/4//数字每缺一位时的向右偏移量
#define yOfChar sideLength/16//sideLength*9/32

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
	MEDIUMORCHID,//r5
	BLUEVIOLET,//c5
	VIOLET,//r6
	LIGHTMAGENTA,//c6
	TAN,//r7
	SIENNA,//c7
	DARKGRAY,//r8
	BLACK,//c8
	WHITE,//r9
	LIGHTGRAY//c9
};

void SetColor(int n)
{
	int r, c;
	int fontColor, fillColor;
	if(n == 0)
	{
		setfillcolor(color[0]);
		return;
	}
	n--;
	r = n / difficulty;
	c = n % difficulty;
	//填充颜色
	if(colorStyle == 0)//降阶
	{
		//0 r1 c1 r2 c2
		//0  1  2  3  4
		if(r == c) fillColor = 2*r%18+1;
		else if(r < c) fillColor = 2*r%18+1;
		else fillColor = 2*c%18+2;
	}
	else if(colorStyle == 1)//层先
	{
		fillColor = 2*r%18+2;
	}
	else//四分降阶
	{
		//16 15 黑 深
		//18 17 浅 白
		fillColor = 15+2*(r>=(difficulty+1)/2)+(c<(difficulty+1)/2);
	}
	//字体颜色
	if(colorStyle == 2)
	{
		r %= (difficulty+1)/2;
		c %= (difficulty+1)/2;
		if(r == c) fontColor = 2*r%18+1;
		else if(r < c) fontColor = 2*r%18+1;
		else fontColor = 2*c%18+2;
		if(fontColor == fillColor)
		{
			if(fillColor > 16) fontColor = 16;
			else fontColor = 17;
		}
	}
	else
	{
		if(fillColor < 9 || fillColor > 16) fontColor = 16;//黑
		else fontColor = 0;//白
	}
	setcolor(color[fontColor]);
	setfillcolor(color[fillColor]);
}

int RightNumber(int r, int c)
{
	if(r == difficulty-1 && c == difficulty-1) return 0;
	else return r*difficulty+c+1;;
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
		if(n < 10) xyprintf(c*sideLength+xOfChar+dxOfChar, r*sideLength+yOfChar, "%d", n);
		else xyprintf(c*sideLength+xOfChar, r*sideLength+yOfChar, "%d", n);
	}
}

void DrawBoard()
{
	int r, c, n;
	for(r=0; r<difficulty; r++)
	{
		for(c=0; c<difficulty; c++)
		{
			n = board[r][c];
			//n = RightNumber(r, c);
			SetColor(n);
			ege_fillrect(c*sideLength, r*sideLength, sideLength, sideLength);
			if(n == 0);
			else if(n < 10) xyprintf(c*sideLength+xOfChar+dxOfChar, r*sideLength+yOfChar, "%d", n);
			else if(n < 100) xyprintf(c*sideLength+xOfChar, r*sideLength+yOfChar, "%d", n);
			else//设置难度鼠标在界外松开时可设置为超10阶华容
			{
				setfont(heightOfChar/2, 0, "Consolas");
				if(n < 1000) xyprintf(c*sideLength+xOfChar+dxOfChar/2, r*sideLength+yOfChar+dxOfChar, "%d", n);
				else xyprintf(c*sideLength+xOfChar, r*sideLength+yOfChar+dxOfChar, "%d", n);
				setfont(heightOfChar, 0, "Consolas");
			}
		}
	}
}

void InitWindow(int mode)
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
	if(mode == 0)
	{
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
		setfont(heightOfChar, 0, "Consolas");
		setbkmode(TRANSPARENT);
		ege_enable_aa(true);
	}
	else
	{
		while(sideLength*difficulty > screenWidth || sideLength*(difficulty+1) > screenHeight)
		{
			sideLength -= 8;
		}
		if(sideLength < 16) sideLength = 16;
		resizewindow(sideLength*difficulty, sideLength*difficulty);
		setfont(heightOfChar, 0, "Consolas");
	}
}

void Resize(char mode)//调整显示大小
{
	if(mode == '+')//16-48时每格调整8，192+时32
	{
		if(sideLength >= 192) sideLength += 32;
		else sideLength += 8;
	}
	else if(mode == '-')
	{
		if(sideLength > 192) sideLength -= 32;
		else if(sideLength > 16) sideLength -= 8;
	}
}

int CapsLk()
{
	BYTE lpKeyState[256];
	if(GetKeyboardState(lpKeyState))
	{
		return lpKeyState[VK_CAPITAL];
	}
	else return 0;
}

int IsSolvable()//丢丢hamburger算法
{
	int r1, c1, r2, c2, n = 0;
	for(r1=0; r1<difficulty; r1++)
	{
		for(c1=0; c1<difficulty; c1++)
		{
			if(board[r1][c1] == 0) continue;
			for(r2=0; r2<=r1; r2++)
			{
				for(c2=0; c2<difficulty; c2++)
				{
					if(r1 == r2 && c1 == c2) break;
					if(board[r1][c1] < board[r2][c2] || board[r2][c2] == 0)
					{
						n++;
					}
				}
			}
		}
	}
	return n%2 == (rZero+cZero)%2;
}

void InitBoard()
{
	int r, c, n;
	srand(time(0));
	while(1)
	{
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
		if(IsSolvable()) break;//判断可解性
	}
}

int CheckEnd()
{
	int r, c;
	for(r=0; r<difficulty; r++)
	{
		for(c=0; c<difficulty; c++)
		{
			if((r == difficulty-2 && c == difficulty-1) || (r == difficulty-1 && c == difficulty-2));
			else if(board[r][c] != RightNumber(r, c)) return 0;
		}
	}
	return 1;
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
	int r, c, t0, t1;
	int isMoving = 0, isMoved = 0, isEnd = 0;
	int moveLock = CapsLk();
	mouse_msg mouseMsg;
	key_msg keyMsg;
	/*选择难度*/
	InitWindow(0);
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
				if(mouseMsg.wheel > 0) Resize('+');
				else Resize('-');
				resizewindow(sideLength*4, sideLength*2);
				setfont(heightOfChar, 0, "Consolas");//更新字体大小
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
				else if(keyMsg.key == key_capslock)
				{
					moveLock = !moveLock;
				}
			}
		}
		delay_ms(50);
	}
	/*游戏*/
	board =(int**) calloc(difficulty, sizeof(int*));
	for(r=0; r<difficulty; r++)
	{
		board[r] = (int*) calloc(difficulty, sizeof(int));
	}
	InitBoard();
	InitWindow(1);
	DrawBoard();
	isMoving = moveLock;
	t0 = clock();
	while(1)
	{
		while(mousemsg())
		{
			mouseMsg = getmouse();
			if(moveLock)
			{
				if(mouseMsg.is_down()) isMoving = 0;
				if(mouseMsg.is_up()) isMoving = 1;
			}
			else
			{
				if(mouseMsg.is_up()) isMoving = 0;
			}
			if((!moveLock && mouseMsg.is_down()) || (mouseMsg.is_move() && isMoving == 1))//移动
			{
				r = mouseMsg.y / sideLength;
				c = mouseMsg.x / sideLength;
				Move(r, c);
				isMoving = 1;
				isMoved = 1;
			}
			if(mouseMsg.is_wheel() && keystate(key_control))//调整显示大小
			{
				if(mouseMsg.wheel > 0) Resize('+');
				else Resize('-');
				resizewindow(sideLength*difficulty, sideLength*difficulty);
				setfont(heightOfChar, 0, "Consolas");//更新字体大小
				DrawBoard();
			}
		}
		while(kbmsg())
		{
			keyMsg = getkey();
			if(keyMsg.msg == key_msg_down)
			{
				if(keyMsg.key == 'W' || keyMsg.key == 'A' || keyMsg.key == 'S' || keyMsg.key == 'D')//移动
				{
					if(keyMsg.key == 'W') Move(rZero-1, cZero);
					else if(keyMsg.key == 'A') Move(rZero, cZero-1);
					else if(keyMsg.key == 'S') Move(rZero+1, cZero);
					else if(keyMsg.key == 'D') Move(rZero, cZero+1);
					isMoved = 1;
				}
				else if(keyMsg.key == 'R')
				{
					InitBoard();
					DrawBoard();
					t0 = clock();
					isEnd = 0;
				}
				else if(keyMsg.key == key_capslock)
				{
					moveLock = !moveLock;
					isMoving = moveLock;
				}
				else if(keyMsg.key == 'C')
				{
					colorStyle = (colorStyle+1)%3;
					isMoved = 1;
				}
			}
		}
		if(isMoved == 1)
		{
			DrawBoard();
			isMoved = 0;
			if(isEnd == 1 || CheckEnd() == 1)
			{
				//游戏结束时显示用时，并且可以继续移动，归位时总显示第一次用时，调整显示大小后用时消失
				if(isEnd == 0)
				{
					t1 = clock();
					isEnd = 1;
					delay_ms(1000);
				}
				if(CheckEnd() == 1)
				{
					setcolor(BLACK);
					setfont(heightOfChar/2, 0, "Consolas");
					//xyprintf(cZero*sideLength, rZero*sideLength, "Time", t1-t0);
					if((t1-t0)/1000 >= 1000) xyprintf(cZero*sideLength+xOfChar, rZero*sideLength+yOfChar+dxOfChar, "%d", (t1-t0+500)/1000);
					else if((t1-t0)/1000 >= 100) xyprintf(cZero*sideLength+xOfChar+dxOfChar/2, rZero*sideLength+yOfChar+dxOfChar, "%d", (t1-t0+500)/1000);
					else if((t1-t0)/1000 >= 10) xyprintf(cZero*sideLength+xOfChar, rZero*sideLength+yOfChar+dxOfChar, "%.1f", (float)(t1-t0)/1000);
					else xyprintf(cZero*sideLength+xOfChar, rZero*sideLength+yOfChar+dxOfChar, "%.2f", (float)(t1-t0)/1000);
					setfont(heightOfChar, 0, "Consolas");
				}
			}
			delay_ms(0);
		}
		else delay_ms(50);
	}
	closegraph();
	for(r=0; r<difficulty; r++)
	{
		free(board[r]);
	}
	free(board);
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
HuaRongRoad 0.3
——新增 滑动操作(鼠标按下过程中持续触发移动)
——新增 胜利时显示用时
——优化 调整紫色和粉色区分
HuaRongRoad 0.4
——新增 使用丢丢hamburger算法维持可解性
——新增 按R重置
——新增 使用动态内存分配
——优化 显示大小采用部分非线性调整
——优化 统一字体位置设计语言
——优化 默认显示大小不超过屏幕大小
——优化 移动后不再延时
HuaRongRoad 0.5
——优化 设置难度鼠标在界外松开时的3位数以上数字显示
——修复 不能正确的适配屏幕分辨率
HuaRongRoad 0.6
——新增 CapsLk键亮起时鼠标直接触发移动，按下时不触发
——新增 按C切换着色风格
——优化 默认显示大小
——修复 部分难度不显示用时
--------------------------------*/
