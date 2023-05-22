#include "man.h"
#include"weapon.h"
#include <stdlib.h>
#include <time.h> 

#ifndef AI_H
#define AI_H
#define NUM_MAN 4//场景中人的数量
int counter,switch_loop;
//初始化游戏。
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
	MOUSEMSG msg;int i=0;//操作的编号。
	if(setOfMan[i].state_r<=0)//如果右手空闲和处于射击状态。
	{
		if(_kbhit())
		{
			switch(_getch())
			{
			case 75:setOfMan[i].move(setOfMan[i].getx()-setOfMan[i].speed,setOfMan[i].gety(),2);break;//左移
			case 77:setOfMan[i].move(setOfMan[i].getx()+setOfMan[i].speed,setOfMan[i].gety(),2);break;//右移
			case 72:setOfMan[i].move(setOfMan[i].getx(),setOfMan[i].gety()-setOfMan[i].speed,2);break;//上移
			case 80:setOfMan[i].move(setOfMan[i].getx(),setOfMan[i].gety()+setOfMan[i].speed,2);break;//下移
			case 52:setOfMan[i].rotate(PI);break;//左转
			case 54:setOfMan[i].rotate(0);break;//右转
			case 56:setOfMan[i].rotate(-PI/2);break;//上转
			case 50:setOfMan[i].rotate(PI/2);break;//下转
			case 49:setOfMan[i].act(1);break;////使用刀剑武器
			case 51:if(setOfMan[i].act(-1))setOfShell.push_back(shell(0,i));break;//使用炮弹武器
			default:break;
			}
		}
	}
	else setOfMan[i].act(0);//否则继续动作。
}

void AI(int timeCounter)
{
	double state_speed=0.02;
	if(setOfMan[0].state<=0)switch_loop=-1;//判断是否失败。
	int i=0;
	for(int j=1;j!=NUM_MAN;++j)
		if(setOfMan[j].state>0)++i;
	if(i==0)switch_loop=1;//判断是否胜利。

	for(int i=1;i!=NUM_MAN;++i)//开始循环。
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
		if(distance>0.8*pow(setOfMan[i].range,2))//如果超出0.8倍射程，向目标前进。
			setOfMan[i].move(x1,y1,state_speed);
		if(setOfMan[i].state_r<0)//炮弹攻击
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
		else //刀剑攻击
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
		setOfMan[i].ammunition+=setOfMan[i].load;//装填弹药。若超出最大弹药量则设为最大弹药量。
		if(setOfMan[i].ammunition>setOfMan[i].max_ammunition)setOfMan[i].ammunition=setOfMan[i].max_ammunition;
	}
}

//显示函数，用于完成对刀剑和炮弹的操作，并对场景所有物体进行显示。-------还没有加概率luck；
void display()
{
	for(auto s=setOfSword.begin();s!=setOfSword.end();)//对剑的处理。
	{
		int i=s->getMaster();
		if(setOfMan[i].state_r>=0)//若握剑人处于握剑状态；
		{
			target t=s->cut();
			s->display();
			for(auto t1:t.setOfFind)//计算伤害；
				setOfMan[t1].getDamage(t.get_damage);
		}
		if(setOfMan[i].state==-1)s=setOfSword.erase(s);
		else ++s;
	}
	for(auto s=setOfShell.begin();s!=setOfShell.end();)//对炮弹的处理；
	{
		int i=s->getMaster();
		target t=s->shoot();
		s->display();
		for(auto t1:t.setOfFind)//计算伤害；
			setOfMan[t1].getDamage(t.get_damage);
		if(!s->isHasRange())s=setOfShell.erase(s);//若射程归零，擅长该炮弹。
		else ++s;
	}
	for(auto m=setOfMan.begin();m!=setOfMan.end();++m)//对人的处理。
		m->display();
}
#endif