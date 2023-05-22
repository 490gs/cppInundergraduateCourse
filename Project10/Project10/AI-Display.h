#include "man.h"
#include"weapon.h"
#include <stdlib.h>
#include <time.h> 

#ifndef AI_H
#define AI_H
#define NUM_MAN 4//�������˵�����
int counter,switch_loop;
//��ʼ����Ϸ��
void initialize_game()
{
	int counter=-1,switch_loop=0;
	initialize_act();
	t_empty.first_target=-1;t_empty.get_damage=0;
	setOfMan.push_back(man(0,0,10,1,WIN_WIDTH/2,WIN_HIGH,-PI/2));
	for(int i=1;i!=NUM_MAN;++i)
		setOfMan.push_back(man(i,0,10,0,i*WIN_WIDTH/NUM_MAN,25,PI/2));
	for(int i=0;i!=NUM_MAN;++i)
		setOfSword.push_back(sword(0,i));
}

void control()
{
	MOUSEMSG msg;int i=0;//�����ı�š�
	if(setOfMan[i].state_r<=0)//������ֿ��кʹ������״̬��
	{
		if(_kbhit())
		{
			switch(_getch())
			{
			case 75:setOfMan[i].move(setOfMan[i].getx()-setOfMan[i].speed,setOfMan[i].gety(),2);break;//����
			case 77:setOfMan[i].move(setOfMan[i].getx()+setOfMan[i].speed,setOfMan[i].gety(),2);break;//����
			case 72:setOfMan[i].move(setOfMan[i].getx(),setOfMan[i].gety()-setOfMan[i].speed,2);break;//����
			case 80:setOfMan[i].move(setOfMan[i].getx(),setOfMan[i].gety()+setOfMan[i].speed,2);break;//����
			case 52:setOfMan[i].rotate(PI);break;//��ת
			case 54:setOfMan[i].rotate(0);break;//��ת
			case 56:setOfMan[i].rotate(-PI/2);break;//��ת
			case 50:setOfMan[i].rotate(PI/2);break;//��ת
			case 49:setOfMan[i].act(1);break;////ʹ�õ�������
			case 51:if(setOfMan[i].act(-1))setOfShell.push_back(shell(0,i));break;//ʹ���ڵ�����
			default:break;
			}
		}
	}
	else setOfMan[i].act(0);//�������������
}

void AI(int timeCounter)
{
	double state_speed=0.02;
	if(setOfMan[0].state<=0)switch_loop=-1;//�ж��Ƿ�ʧ�ܡ�
	int i=0;
	for(int j=1;j!=NUM_MAN;++j)
		if(setOfMan[j].state>0)++i;
	if(i==0)switch_loop=1;//�ж��Ƿ�ʤ����

	for(int i=1;i!=NUM_MAN;++i)//��ʼѭ����
	{
		srand((unsigned)time(NULL)); 
		double x2=rand()%3*setOfMan[i].getr()+setOfMan[i].getx(),y2=rand()%3*setOfMan[i].getr()+setOfMan[i].gety(),p_change[3];
		for(int j=0;j<NUM_MAN-1;++j)
		{
			p_change[j]=rand()/double(RAND_MAX);
		}
		if(setOfMan[i].state<=0)continue;
		double x=setOfMan[i].getx(),y=setOfMan[i].gety();
		double x1=setOfMan[0].getx(),y1=setOfMan[0].gety(),
			distance=pow(x-x1,2)+pow(y-y1,2),a=atan( (y1-y)/(x1-x) );if(x1<x)a+=PI;
		if(distance>0.8*pow(setOfMan[i].range,2))//�������0.8����̣���Ŀ��ǰ����
			setOfMan[i].move(x1,y1,state_speed);
		if(setOfMan[i].state_r<0)//�ڵ�����
		{
			if(distance<pow(10*setOfMan[i].getr(),2))
			{
				setOfMan[i].rotate(a-PI/3);
				setOfMan[i].act(1);
			}
			if(p_change[i-1]>0.5){setOfMan[i].state_r=0;continue;}
			setOfMan[i].rotate(a);
			setOfMan[i].act(-1);
			if(setOfMan[i].ammunition>0&&timeCounter%100==0){setOfShell.push_back(shell(0,i)),--setOfMan[i].ammunition;}
			if(distance<0.5*pow(setOfMan[i].range,2))setOfMan[i].move(x1,y1,-state_speed);
			else setOfMan[i].move(x2,y2,state_speed);
		}
		else //��������
		{
			if(p_change[i-1]>0.4){setOfMan[i].state_r=-1;continue;}cout<<distance<<' '<<pow(10*setOfMan[i].getr(),2)<<endl;
			if(distance<pow(10*setOfMan[i].getr(),2))
			{
				setOfMan[i].rotate(a-PI/4);
				setOfMan[i].act(1);
			}
			else
				setOfMan[i].move(x1,y1,2*state_speed);
		}
		setOfMan[i].ammunition+=setOfMan[i].load;//װ�ҩ�����������ҩ������Ϊ���ҩ����
		if(setOfMan[i].ammunition>setOfMan[i].max_ammunition)setOfMan[i].ammunition=setOfMan[i].max_ammunition;
	}
}

//��ʾ������������ɶԵ������ڵ��Ĳ��������Գ����������������ʾ��-------��û�мӸ���luck��
void display()
{
	for(auto s=setOfSword.begin();s!=setOfSword.end();)//�Խ��Ĵ���
	{
		int i=s->getMaster();
		if(setOfMan[i].state_r>=0)//���ս��˴����ս�״̬��
		{
			target t=s->cut();
			s->display();
			for(auto t1:t.setOfFind)//�����˺���
				setOfMan[t1].getDamage(t.get_damage);
		}
		if(setOfMan[i].state==-1)s=setOfSword.erase(s);
		else ++s;
	}
	for(auto s=setOfShell.begin();s!=setOfShell.end();)//���ڵ��Ĵ���
	{
		int i=s->getMaster();
		target t=s->shoot();
		s->display();
		for(auto t1:t.setOfFind)//�����˺���
			setOfMan[t1].getDamage(t.get_damage);
		if(!s->isHasRange())s=setOfShell.erase(s);//����̹��㣬�ó����ڵ���
		else ++s;
	}
	for(auto m=setOfMan.begin();m!=setOfMan.end();++m)//���˵Ĵ���
		m->display();
}
#endif