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
	initgraph(WIN_WIDTH,WIN_HIGH);   // ������ͼ���ڣ���СΪ 640x480 ����
	char s[]="��ӭ�����������ʼ��\0";
	char s1[]="��������ƽ�ɫ�ƶ�����Ľ�ɫ�����·���\0";
	char s2[]="�����ʹ�õ����������뷨���밴Shift����\
			 ����4682�ֱ������������ת����13�ֱ�������������\0";
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
		char s[]="���������Ѿ��\0";
		outtextxy(WIN_WIDTH/3+30,WIN_HIGH/2,s);
	}
	else
		{
		char s[]="û�뵽�����Ȼ��ô������\0";
		outtextxy(WIN_WIDTH/3+30,WIN_HIGH/2,s);
		}
	Sleep(3000);
    _getch();              // �����������
    closegraph();          // �رջ�ͼ����*/
}