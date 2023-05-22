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

#define WIN_WIDTH 960			//���ڿ�
#define WIN_HIGH 640			//���ڸ�

#define MIN_DISTANCE 0			//��С�ƶ�Ҫ�����
#define MIN_DA PI/3				//��С��ͨ����
#define MIN_DISOFRETREAT 0.6	//��С����Ҫ�����
#define MIN_DISOFFIGHT	1.5		//��С��սҪ�����
#define RRATIO 1.5				//ʵ�ʰ뾶��
#define LUARM 1.6				//�ϱ۳�������뾶r��
#define LLARM 1.6				//�±۳�������뾶r��
#define AUARM PI/2				//�ϱ�������巽��Ƕ�
#define ALARM 0					//�±�������巽��Ƕ�
#define AHAND -PI/2				//��������巽��Ƕ�
#define WIDTHOFGUN 2			//ǹ�뾶
#define LIFELINEHIGH 20			//Ѫ���߸�
#define LIFELINELENGTH 70		//Ѫ���߳�
#define MAN_VALUE 2
const double PI=acos(-1);
struct target					//Ŀ��ṹ�塣
{
	vector<int>setOfFind;
	int first_target;
	double get_damage;
}t_empty;
struct xya
{
	double x,y,a;
};
struct actlist//�����ṹ�塣
{
	int n;//������Ҫ֡����
	vector<double>act0,act1,act2,act3;//������ת�ǣ�����Խǣ��±���Խǣ��ϱ���Խǡ�
}act_default,act_real;
vector<actlist>setOfAct;//������ж����ṹ�塣
void initialize_act()//��ʼ�������ṹ�塣
{
	act_default.n=1;act_default.act0.push_back(0);act_default.act1.push_back(0);act_default.act2.push_back(-PI/6);act_default.act3.push_back(PI/6);
	setOfAct.push_back(act_default);//Ĭ�϶���Ϊ���״̬��
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
	double x,y,a;									//�������꼰����Ǻ��ƶ��ǡ�
	double aUarmR,aLarmR,aHandR,lUarmR,lLarmR;				//�ң��ϱ���Խǣ��±���Խǣ�����Խǣ��ϱ���Գ����±���Գ���
	double aUarmL,aLarmL,aHandL,lUarmL,lLarmL;				//���ϱ���Խǣ��±���Խǣ�����Խǣ��ϱ���Գ����±���Գ���				
	int type,r,num;										//���ͣ������˰뾶��������speed����2r����š�
	int max_life,defense,lucky;
public:
	int myself,enemy;
	int state,state_r,state_l,life,nowAct,max_ammunition,ammunition,load;							//����״̬���ұۺ����״̬��0��ʾ���С�-1��ʾ���̬������ֵ.��ǰ����,��ҩ����			
	double lengthOfGun,speed,damage,range;
	man(int,int,int,int,double,double,double);
	
	inline double getx(){return x;}
	inline double gety(){return y;}
	inline double geta(){return a;}
	inline int getr(){return r;}
	xya getHand(int i);

	inline void getDamage(int in_damage){life+=(defense-in_damage>0?0:defense-in_damage);if(life<0){life=0;state=0;}}//���˺�����
	bool act(int);
	target find(int );
	inline void rotate(double aTarget){a=aTarget;}	//ת��������ת��da���ȡ�
	void move(double,double);						//�ƶ������������ƶ�����a_move�����ƶ���
	double move(double,double,double);				//�ƶ�����,�����ƶ����ƶ���(pa,pb)�㷵��ǰ���Ƕȡ�
	void display();									//����

};vector<man>setOfMan;						//���泡���������˵���Ϣ��
man noman();								//��ʾ�����ڵ��ˡ�


man::man(int in_num,int in_type,int in_r,int in_myself,double in_x=WIN_WIDTH/2,double in_y=0,double in_a=PI/2):num(in_num),
	type(in_type),r(in_r),myself(in_myself),x(in_x),y(in_y),a(in_a)
	{
		enemy=(myself==0?1:0);
		aUarmR=AUARM,aLarmR=ALARM,aHandR=AHAND,lUarmR=LUARM,lLarmR=LLARM;
		aUarmL=-1*AUARM,aLarmL=-1*ALARM,aHandL=-1*AHAND,lUarmL=LUARM,lLarmL=LLARM;
		state=1;state_r=0,state_l=0;nowAct=0;
		switch(type)//0Ϊ��ͨ�ͣ�1Ϊ��սǿ���ͣ�2ΪԶ��ǿ���ͣ�3ΪѪ��ǿ���ͣ�4Ϊ�ٶ�ǿ���ͣ�5Ϊȫ���͡�
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
void man::display()//����
	{
		if(state==1)//�����š�
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

//���ַ���Ķ�����������еĶ����ı�š������Ƿ���������Ҫ���������ж������������㲻�ı䡣ֻ�е�
bool man::act(int in_nowAct)
{
	if(state_r>0)//�������״̬����1����ʾ���ڽ�����ض������ܾ���Ӧ���롣
	{
		a+=setOfAct[nowAct].act0[state_r];
		aHandR=setOfAct[nowAct].act1[state_r];
		aLarmR=setOfAct[nowAct].act2[state_r];
		aUarmR=setOfAct[nowAct].act3[state_r];//ִ�иö�����
		--state_r;
		if(state_r==0)nowAct=0;
		return false;
	}
	else//�������״̬Ϊ��������״̬���������룬������ǰ������Ϊ���룬����״̬��Ϊ���һ����
	{
		if(in_nowAct<0)//����С���㣬�������״̬��
		{
			a+=setOfAct[0].act0[0];
			aHandR=setOfAct[0].act1[0];
			aLarmR=setOfAct[0].act2[0];
			aUarmR=setOfAct[0].act3[0];//ִ�����������
			state_r=-1;return true;
		}
		if(in_nowAct==0)
		{
			aHandR=AHAND;
			aLarmR=ALARM;
			aUarmR=AUARM;//ִ�п��ж�����
			state_r=0;return true;
		}
		nowAct=in_nowAct;state_r=setOfAct[nowAct].n-1;return true;
	}
}
//��ȡ��ز�λλ�ýǶ���Ϣ������Ϊ�Ҳ࣬����Ϊ��࣬1���֣�2���⣬3�Ǽ硣
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
//���˺�����
target man::find(int myEnemy=-1)//-1��ʾѰ�������ˡ�
{
	target t;int count=-1;double min_distance=pow(WIN_WIDTH,2)+pow(WIN_HIGH,2);
	if(myEnemy!=-1)//�������Ѱ�������ˡ�
		for(auto m:setOfMan)
		{
			++count;//��������1����0��ʼ������
			double dx=m.getx()-x,dy=m.gety()-y,distance=dx*dx+dy*dy;//�ڵ�����˵ĺ�����룬������룬�;���ƽ����
			if(distance<min_distance)t.first_target=count;
			continue;
		}
	else
		for(auto m:setOfMan)
		{
			++count;//��������1����0��ʼ������
			if(m.enemy!=enemy)continue;//������ǵ��ˣ�����һ���ˡ�
			double dx=m.getx()-x,dy=m.gety()-y,distance=dx*dx+dy*dy;//�ڵ�����˵ĺ�����룬������룬�;���ƽ����
			if(distance<min_distance)t.first_target=count;
			continue;
		}
	return t;
}

//�ƶ������������ƶ�����a_move�����ƶ���
void man::move(double a_move,double state_move)
{
	double realSpeed=speed*state_move;
	x+=realSpeed*cos(a_move);if(x>WIN_WIDTH||x<0)x-=realSpeed*cos(a_move);
	y+=realSpeed*sin(a_move);if(y>WIN_HIGH||y<0)x-=realSpeed*sin(a_move);
}
//�ƶ������������ƶ���ֻ��Ŀ�ĵؾ������MIN_DIATANCEʱ���ƶ�,�ƶ���(a,b)�㣬��ʵ�ٶ��������ٶȱ�Ϊstade������ǰ���ǶȻ�0��0��ʾδ�ƶ���
double man::move(double pa,double pb,double state_move)
{
	if(pa<0||pa>WIN_WIDTH||pb<0||pb>WIN_HIGH)return 0;
	double distance=(pa-x)*(pa-x)+(pb-y)*(pb-y);//������Ŀ�ĵؾ���ƽ��
	if(distance>pow(MIN_DISTANCE,2))
	{
		double a_move=atan((pb-y)/(pa-x));
		if(pa<x)a_move+=PI;						//ǰ���Ƕ�

		double realSpeed=speed*state_move;
		if(distance>=pow(realSpeed,2))			//���������ʵ�ٶȣ�����ʵ�ٶ��ƶ���
		{
			x+=realSpeed*cos(a_move);
			y+=realSpeed*sin(a_move);
		}
		else									//����С����ʵ�ٶȣ�ֱ���ƶ���Ŀ�ĵأ�
		{
			x=pa;y=pb;
		}
		return a_move;							//�ƶ���������ǰ���Ƕȣ�
	}
	else return 0;								//δ�ƶ�������0��
}
#endif