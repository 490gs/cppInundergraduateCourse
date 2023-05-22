#include<iostream>
#include<windows.h>
#include <graphics.h>    
#include <conio.h>
#include"man.h"
#include"weapon.h"
#include"AI-Display.h"
using namespace std;


void main()
{
	initialize_game();
	initgraph(WIN_WIDTH,WIN_HIGH);   // 创建绘图窗口，大小为 640x480 像素
	char s[]="欢迎，按任意键开始。\0";
	char s1[]="方向键控制角色移动，你的角色在最下方。\0";
	char s2[]="如果能使用的是中文输入法，请按Shift键。\
			 数字4682分别代表朝左右上下转动。13分别代表砍击和射击。\0";
	outtextxy(WIN_WIDTH/3+30,WIN_HIGH/3,s);
	outtextxy(WIN_WIDTH/3,WIN_HIGH/3+30,s1);
	outtextxy(60,WIN_HIGH/3+60,s2);
	Sleep(3000);
	_getch();
	BeginBatchDraw();
	while(switch_loop==0)
	{
		control();
		AI(++counter);
		display();
		FlushBatchDraw();
		cleardevice();
	}
	EndBatchDraw();
	if(switch_loop==1)
	{
		char s[]="你可真厉害丫。\0";
		outtextxy(WIN_WIDTH/3+30,WIN_HIGH/2,s);
	}
	else
		{
		char s[]="没想到，你居然这么垃圾。\0";
		outtextxy(WIN_WIDTH/3+30,WIN_HIGH/2,s);
		}
	Sleep(3000);
    _getch();              // 按任意键继续
    closegraph();          // 关闭绘图窗口*/
}