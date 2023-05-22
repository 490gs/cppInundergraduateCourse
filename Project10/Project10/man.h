#include"iostream"
#include<windows.h>
#include <graphics.h>    
#include <conio.h>
#include<cmath>
#include<vector>
#include<iterator>
using namespace std;

#ifndef MAN_H
#define MAN_H

#define WIN_WIDTH 960			//窗口宽
#define WIN_HIGH 640			//窗口高

#define MIN_DISTANCE 0			//最小移动要求距离
#define MIN_DA PI/3				//最小可通过角
#define MIN_DISOFRETREAT 0.6	//最小撤退要求距离
#define MIN_DISOFFIGHT	1.5		//最小近战要求距离
#define RRATIO 1.5				//实际半径比
#define LUARM 1.6				//上臂长与身体半径r比
#define LLARM 1.6				//下臂长与身体半径r比
#define AUARM PI/2				//上臂相对身体方向角度
#define ALARM 0					//下臂相对身体方向角度
#define AHAND -PI/2				//手相对身体方向角度
#define WIDTHOFGUN 2			//枪半径
#define LIFELINEHIGH 20			//血量线高
#define LIFELINELENGTH 70		//血量线长
#define MAN_VALUE 2
const double PI=acos(-1);
struct target					//目标结构体。
{
	vector<int>setOfFind;
	int first_target;
	double get_damage;
}t_empty;
struct xya
{
	double x,y,a;
};
struct actlist//动作结构体。
{
	int n;//动作需要帧数。
	vector<double>act0,act1,act2,act3;//身体旋转角，手相对角，下臂相对角，上臂相对角。
}act_default,act_real;
vector<actlist>setOfAct;//存放所有动作结构体。
void initialize_act()//初始化动作结构体。
{
	act_default.n=1;act_default.act0.push_back(0);act_default.act1.push_back(0);act_default.act2.push_back(-PI/6);act_default.act3.push_back(PI/6);
	setOfAct.push_back(act_default);//默认动作为射击状态。
	double a1[7]={-5*PI/6,-2*PI/3,-PI/3,0,PI/4,PI/3,PI/3},
		a2[7]={-2*PI/3,-PI/6,0,PI/6,PI/3,PI/2,PI/2},
		a3[7]={-PI/6,PI/6,PI/3,PI/2,2*PI/3,2*PI/3,2*PI/3};
	act_real.n=7;
	for(int i=6;i!=-1;--i)
	{
		act_real.act0.push_back(0);act_real.act1.push_back(a1[i]);
		act_real.act2.push_back(a2[i]);act_real.act3.push_back(a3[i]);
	}
	setOfAct.push_back(act_real);
}
class man
{
private:
	double x,y,a;									//横纵坐标及方向角和移动角。
	double aUarmR,aLarmR,aHandR,lUarmR,lLarmR;				//右：上臂相对角，下臂相对角，手相对角，上臂相对长，下臂相对长。
	double aUarmL,aLarmL,aHandL,lUarmL,lLarmL;				//左：上臂相对角，下臂相对角，手相对角，上臂相对长，下臂相对长。				
	int type,r,num;										//类型，可受伤半径，不允许speed大于2r，编号。
	int max_life,defense,lucky;
public:
	int myself,enemy;
	int state,state_r,state_l,life,nowAct,max_ammunition,ammunition,load;							//总体状态，右臂和左臂状态。0表示空闲。-1表示射击态。生命值.当前动作,弹药量。			
	double lengthOfGun,speed,damage,range;
	man(int,int,int,int,double,double,double);
	
	inline double getx(){return x;}
	inline double gety(){return y;}
	inline double geta(){return a;}
	inline int getr(){return r;}
	xya getHand(int i);

	inline void getDamage(int in_damage){life+=(defense-in_damage>0?0:defense-in_damage);if(life<0){life=0;state=0;}}//受伤函数。
	bool act(int);
	target find(int );
	inline void rotate(double aTarget){a=aTarget;}	//转动函数，转至da弧度。
	void move(double,double);						//移动函数，单次移动，朝a_move方向移动。
	double move(double,double,double);				//移动函数,单次移动，移动到(pa,pb)点返回前进角度。
	void display();									//绘制

};vector<man>setOfMan;						//保存场景中所有人的信息。
man noman();								//表示不存在的人。


man::man(int in_num,int in_type,int in_r,int in_myself,double in_x=WIN_WIDTH/2,double in_y=0,double in_a=PI/2):num(in_num),
	type(in_type),r(in_r),myself(in_myself),x(in_x),y(in_y),a(in_a)
	{
		enemy=(myself==0?1:0);
		aUarmR=AUARM,aLarmR=ALARM,aHandR=AHAND,lUarmR=LUARM,lLarmR=LLARM;
		aUarmL=-1*AUARM,aLarmL=-1*ALARM,aHandL=-1*AHAND,lUarmL=LUARM,lLarmL=LLARM;
		state=1;state_r=0,state_l=0;nowAct=0;
		switch(type)//0为普通型，1为近战强化型，2为远程强化型，3为血量强化型，4为速度强化型，5为全能型。
		{
		case 0:max_life=life=1000;lengthOfGun=15;damage=5,range=500,defense=5,speed=15,lucky=10,load=1,max_ammunition=ammunition=100;break;
		case 1:max_life=life=1000;lengthOfGun=15;damage=10,range=500,defense=5,speed=15,lucky=10,load=1,max_ammunition=ammunition=100;break;
		case 2:max_life=life=1000;lengthOfGun=15;damage=5,range=700,defense=5,speed=15,lucky=10,load=1,max_ammunition=ammunition=100;break;
		case 3:max_life=life=3000;lengthOfGun=15;damage=5,range=500,defense=8,speed=15,lucky=10,load=1,max_ammunition=ammunition=100;break;
		case 4:max_life=life=1000;lengthOfGun=15;damage=5,range=500,defense=5,speed=25,lucky=10,load=1,max_ammunition=ammunition=100;break;
		case 5:max_life=life=1500;lengthOfGun=15;damage=7,range=600,defense=6,speed=20,lucky=15,load=1,max_ammunition=ammunition=100;break;
		default:max_life=life=1000;lengthOfGun=15;damage=15,range=500,defense=5,speed=15,lucky=10,load=1,ammunition=100;break;
		}
	}
void man::display()//绘制
	{
		if(state==1)//还活着。
		{
			setlinecolor(RGB(255,0,0));
			line(x-LIFELINEHIGH/2,y-LIFELINEHIGH,x+LIFELINEHIGH/2,y-LIFELINEHIGH);
			setlinecolor(RGB(0,255,0));
			line(x-LIFELINEHIGH/2,y-LIFELINEHIGH,x+LIFELINEHIGH*(double(life)/double(max_life)-0.5),y-LIFELINEHIGH);
			setlinecolor(RGB(255,255,255));
			circle(x,y,r);
			xya r3=getHand(3),l3=getHand(-3),r2=getHand(2),l2=getHand(-2),r1=getHand(1),l1=getHand(-1);
			line(r3.x,r3.y,r2.x,r2.y);
			line(r2.x,r2.y,r1.x,r1.y);
			line(l3.x,l3.y,l2.x,l2.y);
			line(l2.x,l2.y,l1.x,l1.y);
			if(state_r<0)line(r1.x,r1.y,r1.x+lengthOfGun*cos(r1.a),r1.y+lengthOfGun*sin(r1.a));
			if(state_l<0)line(l1.x,l1.y,l1.x+lengthOfGun*cos(l1.a),l1.y+lengthOfGun*sin(l1.a));
		}
		else if(state==0||state==-1)
		{
			for(int i=-1;i!=2;++i)
				for(int j=-1;j!=2;++j)
					circle(x+i*r,y+j*r,r);
			--state;
		}
		else
			return;
	}

//右手方面的动作。输入进行的动作的标号。返回是否进行输入的要求。正数进行动作，负数或零不改变。只有当
bool man::act(int in_nowAct)
{
	if(state_r>0)//如果右手状态大于1，表示正在进行相关动作。拒绝回应输入。
	{
		a+=setOfAct[nowAct].act0[state_r];
		aHandR=setOfAct[nowAct].act1[state_r];
		aLarmR=setOfAct[nowAct].act2[state_r];
		aUarmR=setOfAct[nowAct].act3[state_r];//执行该动作。
		--state_r;
		if(state_r==0)nowAct=0;
		return false;
	}
	else//如果右手状态为零或处于射击状态，接受输入，并将当前动作设为输入，右手状态设为最高一步。
	{
		if(in_nowAct<0)//输入小于零，进入射击状态。
		{
			a+=setOfAct[0].act0[0];
			aHandR=setOfAct[0].act1[0];
			aLarmR=setOfAct[0].act2[0];
			aUarmR=setOfAct[0].act3[0];//执行射击动作。
			state_r=-1;return true;
		}
		if(in_nowAct==0)
		{
			aHandR=AHAND;
			aLarmR=ALARM;
			aUarmR=AUARM;//执行空闲动作。
			state_r=0;return true;
		}
		nowAct=in_nowAct;state_r=setOfAct[nowAct].n-1;return true;
	}
}
//获取相关部位位置角度信息，正数为右侧，负数为左侧，1是手，2是肘，3是肩。
xya man::getHand(int i=1)
{
	xya t;
	switch(i)
	{
	case 1:{t.x=getHand(2).x+r*lLarmR*cos(a+aLarmR),t.y=getHand(2).y+r*lLarmR*sin(a+aLarmR),t.a=a+aHandR;return t;}
	case 2:{t.x=getHand(3).x+r*lUarmR*cos(a+aUarmR),t.y=getHand(3).y+r*lUarmR*sin(a+aUarmR),t.a=a+aLarmR;return t;}
	case 3:{t.x=x+r*cos(a+PI/2);t.y=y+r*sin(a+PI/2);t.a=a+aUarmR;return t;}
	case -1:{t.x=getHand(-2).x+r*lLarmL*cos(a+aLarmL),t.y=getHand(-2).y+r*lLarmL*sin(a+aLarmL),t.a=a+aHandL;return t;}
	case -2:{t.x=getHand(-3).x+r*lUarmL*cos(a+aUarmL),t.y=getHand(-3).y+r*lUarmL*sin(a+aUarmL),t.a=a+aLarmL;return t;}
	case -3:{t.x=x+r*cos(a-PI/2);t.y=y+r*sin(a-PI/2);t.a=a+aUarmL;return t;}
	default:return getHand(1);
	}
}
//找人函数。
target man::find(int myEnemy=-1)//-1表示寻找所有人。
{
	target t;int count=-1;double min_distance=pow(WIN_WIDTH,2)+pow(WIN_HIGH,2);
	if(myEnemy!=-1)//如果不是寻找所有人。
		for(auto m:setOfMan)
		{
			++count;//计数器加1，从0开始计数。
			double dx=m.getx()-x,dy=m.gety()-y,distance=dx*dx+dy*dy;//炮弹与该人的横向距离，纵向距离，和距离平方。
			if(distance<min_distance)t.first_target=count;
			continue;
		}
	else
		for(auto m:setOfMan)
		{
			++count;//计数器加1，从0开始计数。
			if(m.enemy!=enemy)continue;//如果不是敌人，到下一个人。
			double dx=m.getx()-x,dy=m.gety()-y,distance=dx*dx+dy*dy;//炮弹与该人的横向距离，纵向距离，和距离平方。
			if(distance<min_distance)t.first_target=count;
			continue;
		}
	return t;
}

//移动函数，单次移动，朝a_move方向移动。
void man::move(double a_move,double state_move)
{
	double realSpeed=speed*state_move;
	x+=realSpeed*cos(a_move);if(x>WIN_WIDTH||x<0)x-=realSpeed*cos(a_move);
	y+=realSpeed*sin(a_move);if(y>WIN_HIGH||y<0)x-=realSpeed*sin(a_move);
}
//移动函数，单次移动，只有目的地距离大于MIN_DIATANCE时才移动,移动到(a,b)点，真实速度与自身速度比为stade，返回前进角度或0，0表示未移动。
double man::move(double pa,double pb,double state_move)
{
	if(pa<0||pa>WIN_WIDTH||pb<0||pb>WIN_HIGH)return 0;
	double distance=(pa-x)*(pa-x)+(pb-y)*(pb-y);//自身与目的地距离平方
	if(distance>pow(MIN_DISTANCE,2))
	{
		double a_move=atan((pb-y)/(pa-x));
		if(pa<x)a_move+=PI;						//前进角度

		double realSpeed=speed*state_move;
		if(distance>=pow(realSpeed,2))			//距离大于真实速度，按真实速度移动；
		{
			x+=realSpeed*cos(a_move);
			y+=realSpeed*sin(a_move);
		}
		else									//距离小于真实速度，直接移动到目的地；
		{
			x=pa;y=pb;
		}
		return a_move;							//移动过，返回前进角度；
	}
	else return 0;								//未移动，返回0；
}
#endif