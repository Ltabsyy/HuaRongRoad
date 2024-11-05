#include <stdlib.h>
#include <time.h>
#include <graphics.h>

int difficulty = 10;
int sideLength = 96;
int board[10][10];

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
	for(n=0; n<difficulty*difficulty; )
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

void Move(int rs, int cs)
{
	int rt, ct = -1;
	if(rs > 0 && board[rs-1][cs] == 0)
	{
		rt = rs-1;
		ct = cs;
	}
	else if(cs > 0 && board[rs][cs-1] == 0)
	{
		rt = rs;
		ct = cs-1;
	}
	else if(rs+1 < difficulty && board[rs+1][cs] == 0)
	{
		rt = rs+1;
		ct = cs;
	}
	else if(cs+1 < difficulty && board[rs][cs+1] == 0)
	{
		rt = rs;
		ct = cs+1;
	}
	if(ct != -1)
	{
		board[rt][ct] = board[rs][cs];
		board[rs][cs] = 0;
	}
}

int main()
{
	int r, c, n;
	mouse_msg mouseMsg;
	InitWindow();
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
	difficulty = 0;
	while(difficulty == 0)
	{
		while(mousemsg())
		{
			mouseMsg = getmouse();
			if(mouseMsg.is_up())
			{
				r = mouseMsg.y / sideLength;
				c = mouseMsg.x / sideLength;
				difficulty = 4*r+c+3;
			}
		}
		delay_ms(50);
	}
	InitBoard();
	resizewindow(sideLength*difficulty, sideLength*difficulty);
	while(1)
	{
		DrawBoard();
		while(mousemsg())
		{
			mouseMsg = getmouse();
			if(mouseMsg.is_left() && mouseMsg.is_up())
			{
				r = mouseMsg.y / sideLength;
				c = mouseMsg.x / sideLength;
				Move(r, c);
			}
		}
		delay_ms(50);
	}
	closegraph();
	return 0;
}
