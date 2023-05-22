#include"iostream"
#include<windows.h>
#include <graphics.h>    
#include <conio.h>
#include<cmath>
#include<vector>
#include<iterator>
#include"man.h"
using namespace std;

#ifndef WEAPON_H
#define WEAPON_H

inline double distanceToLine(double x0,double y0,double x1,double x2,double y1,double y2)//求点到线距离
{
	return ( (x1-x2)*(y0-y1)-(y1-y2)*(x0-x1) )/pow(pow(x1-x2,2)+pow(y1-y2,2),0.5);
}
double& max_find(double &a1,double &a2,double &a3,double &a4,int b)//返回第b大的a。
{
	double c[4]={a1,a2,a3,a4};
	for(int i=0;i!=3;++i)
		for(int j=i;j!=4;++j)
			if(c[j]>c[i])
			{
				double tem=c[j];c[j]=c[i];c[i]=tem;
			}
	if(a1==c[b])return a1;
	if(a2==c[b])return a2;
	if(a3==c[b])return a3;
	if(a4==c[b])return a4;
}

class shell
{
private:
	double x,y,a,r;						//当前横纵坐标和角度，炮弹半径。
	int type,master[2],enemy;			//类型和绑定的人的手，-1是左手，1是右手。敌人类型。
	double speed,damage,range;			//速度，伤害，射程。
public:
	shell(int,int,int);			//创建炮弹。
	target shoot();			//射击函数。
	target find();			//找目标函数。
	inline int getMaster(){return master[0];}//返回射击人编号。
	inline bool isHasRange(){return range!=0;};//判断是否有剩余射程函数。
	void display();
};vector<shell>setOfShell;		//用于存放场景中所有炮弹信息。
class sword
{
private:
	double x,y,a,r,x_last,y_last,a_last;		//当前横纵坐标和角度，剑长，刚才横纵坐标和角度。
	int type,master[2],enemy;					//类型和绑定的人的手，-1是左手，1是右手。敌人类型。
	double damage,r_speed;						//伤害，旋转速度。
public:
	sword(int,int,int);
	target cut();					//斩击函数。
	target find();				//旋转找目标函数。
	target find_m();				//平移找目标函数。
	inline int getMaster(){return master[0];}//返回握剑人编号。
	void display();
};vector<sword>setOfSword;			//用于存放场景中所有刀剑信息。


shell::shell(int intype,int inmaster,int inHand=1):type(intype)
{
	master[0]=inmaster,master[1]=inHand;enemy=setOfMan[master[0]].enemy;
	xya xya0=setOfMan[master[0]].getHand(master[1]);
	a=xya0.a;
	x=xya0.x+setOfMan[master[0]].lengthOfGun*cos(a);
	y=xya0.y+setOfMan[master[0]].lengthOfGun*sin(a);
	switch(type)
	{
	case 0:r=2;range=200;damage=10;speed=5;break;
	case 1:r=2;range=300;damage=15;speed=5;break;
	default:r=2;range=400;damage=10;speed=5;break;
	}
	range+=setOfMan[master[0]].range;
}
sword::sword(int intype,int inmaster,int inHand=1):type(intype)
{
	master[0]=inmaster,master[1]=inHand;enemy=setOfMan[master[0]].enemy;
	xya xya0=setOfMan[master[0]].getHand(master[1]);
	x=xya0.x;y=xya0.y;a=xya0.a;
	switch(type)
	{
	case 0:r=100;damage=5;break;
	case 1:r=200;damage=15;break;
	default:r=100;damage=10;break;
	}
	damage+=setOfMan[master[0]].damage;
}

void shell::display()
{
	circle(x,y,r);
}
void sword::display()
{
	if( (master[1]==1&&setOfMan[master[0]].state_r>=0)||(master[1]==-1&&setOfMan[master[0]].state_l>=0) )//如果握剑的手处于握剑状态。
	line(x,y,x+r*cos(a),y+r*sin(a));
}

//射击函数，输入炮弹速度状态和敌人类型，返回伤害值，如果没有击中任何物体，将range减小。
target shell::shoot()
{
	double realSpeed=speed;
	target t=find();
	if(t.first_target==-1)//若距离没有目标。
	{
		if(realSpeed<=range)//速度小于剩余射程，按速度射程前进，射程减小。
		{
			x+=realSpeed*cos(a);
			y+=realSpeed*sin(a);
			range-=realSpeed;
		}
		else//速度大于剩余射程，按剩余射程前进，射程归零。
		{
			x+=range*cos(a);
			y+=range*sin(a);
			range=0;
		}
		t.first_target=0;
		return t;//返回0伤害。
	}
	else//有目标。
	{
		man m=setOfMan[find().first_target];//记目标为m。
		double dx=m.getx()-x,dy=m.gety()-y,distance=dx*cos(a)+dy*sin(a),realDistance=distance;//炮弹前进至目标，并将射程归零。
		x+=realDistance*cos(a);
		y+=realDistance*sin(a);
		range=0;
		t.get_damage=damage;
		return t;//返回真实伤害。
	}
}

//需要结合man的state_r以优化。
target sword::cut()
{
	if(setOfMan[master[0]].state_r<0)return t_empty;//如果握剑人没有握剑，返回空target。
	x_last=x,y_last-y,a_last=a;	xya xya0=setOfMan[master[0]].getHand(master[1]);
	x=xya0.x;y=xya0.y;a=xya0.a;
	r_speed=a-a_last;//转动速度未当前角与刚才角之差。
	target t=find_m();
	x=xya0.x;y=xya0.y;
	target t1=find();
	if(t.first_target==-1)//若移动时没有目标。
	{
		t=t1;//将t1赋给t。
		if(t.first_target==-1)//若没有目标。
		{
			t.get_damage=0;
			return t;
		}
		else
		{
			t.get_damage=damage;
			return t;//返回真实伤害。
		}
	}
	if(t1.first_target!=-1)//若转动时有目标。
	{
		vector<int>tem;
		for(auto m1:t1.setOfFind)
		{
			int has=0;
			for(auto m:t.setOfFind)
			{
				if(m==m1)has=1;
			}
			tem.push_back(m1);
		}
		for(auto te:tem)
			t.setOfFind.push_back(te);//合并两个setOfFind。
	}
	if(t.first_target==-1)//若没有目标。
		{
			t.get_damage=0;
			return t;
		}
		else
		{
			t.get_damage=damage;
			return t;//返回真实伤害。
		}
}


//找目标函数，输入状态和敌人类型，返回目标结构体。
target shell::find()//应该去掉state。
{
	target t;t.first_target=-1;double min_distance=range*range;int count=-1;//预设预设存放标号，最小距离平方，计数器。
	for(auto m:setOfMan)
	{
		/*能命中多个人，思路：是否是敌人；是否已经命中；是否在前向；是否可能相遇；是否超出速度。*/
		++count;//计数器加1，从0开始计数。
		if(m.myself!=enemy)continue;//如果不是敌人，到下一个人。
		if(m.state<=0)continue;//如果不活着，到下一个人。
		double dx=m.getx()-x,dy=m.gety()-y,distance=dx*dx+dy*dy;//炮弹与该人的横向距离，纵向距离，和距离平方。
		if( distance<=pow(r+m.getr(),2) )
		{//如果距离小于炮弹与人半径和，记入此人的编号,若其距离差小于最小距离差，记此人为第一个目标，到下一个人。
			t.setOfFind.push_back(count);
			if(distance<min_distance)
			{
				min_distance=distance;
				t.first_target=count;
			}
			continue;
		}
		if(dx*cos(a)+dy*sin(a)<0)continue;//如果当前人不在炮弹前向，到下一个人。
		if( pow(cos(a)*dy-sin(a)*dx,2)>r+m.getr() )continue;//如果当前人离子弹轨迹的距离大于炮弹和人半径和，到下一个人。
		if(distance>pow(speed,2))continue;//如果距离大于真实速度，到下一个人。
		if(distance>pow(range,2))continue;//如果距离大于真射程，到下一个人。
		t.setOfFind.push_back(count);
		if(distance<min_distance)
			{
				min_distance=distance;
				t.first_target=count;
			}//记入此人的编号,若其距离差小于最小距离差，记此人为第一个目标，到下一个人。
	}
	return t;
}
target sword::find()
{
	/*能命中多个人，思路：是否是敌人；是否可能相遇；是否已经命中；是否在前向；是否超出速度。*/
	target t;t.first_target=-1;int count=-1;double min_da=r_speed;//预设目标结构体，计数器，最小差角。
	for(auto m:setOfMan)
	{
		++count;//计数器加1，从0开始计数。
		if(m.myself!=enemy)continue;//如果不是敌人，到下一个人。
		if(m.state<=0)continue;//如果不活着，到下一个人。
		double dx=m.getx()-x,dy=m.gety()-y,distance=dx*dx+dy*dy;//旋转中心与该人的横向距离，纵向距离，和距离平方。
		double a_m=atan(dy/dx);if(dx<0)a_m+=PI;double da=a_m-a;//该人相对旋转中心的角度，和与剑的夹角。
		if( distance>pow(r+m.getr(),2)|| dx*cos(a)+dy*sin(a)<0 )continue;//如果距离大于真实半径与人半径和或在反方向，到下一个人。
		if( pow(cos(a)*dy-sin(a)*dx,2)<m.getr()&&distance<pow(r,2)+pow(m.getr(),2) )
		{//如果到刀身的距离小于人半径和且可能切到，记入这个人的编号，若其角度差小于最小角度差，记此人为第一个目标，到下一个人。
			t.setOfFind.push_back(count);
			if(da<min_da)
			{
				min_da=da;
				t.first_target=count;
			}
			continue;
		}
		if(r_speed*da<0)continue;//如果当前人不在剑前向，到下一个人。
		if(da>r_speed)continue;//超出转动速度，到下一个人。
		t.setOfFind.push_back(count);
		if(da<min_da)
		{
			min_da=da;
			t.first_target=count;
		}//记入这个人的编号，若其角度差小于最小角度差，记此人为第一个目标，到下一个人。
	}
	return t;
}
target sword::find_m()
{
	/*能命中多个人，思路：构建平行四边形，判断该人中心在不在四边形中。*/
	target t;t.first_target=-1;int count=-1;//预设目标结构体，计数器。
	double dx=x-x_last,dy=y-y_last,min_distance=pow(dx,2)+pow(dy,2);
	for(auto m:setOfMan)
	{
		++count;//计数器加1，从0开始计数。
		if(m.myself!=enemy)continue;//如果不是敌人，到下一个人。
		if(m.state<=0)continue;//如果不活着，到下一个人。
		double x1=x_last+r*cos(a_last),y1=y_last+r*sin(a_last);//刚才剑末端。
		double x2=x+r*cos(a_last),y2=y+r*sin(a_last);//现在剑末端。
		/*double recx1=max_find(x_last,x,x1,x2,0)+r,recx2=max_find(x_last,x,x1,x2,1)+r,
		recx3=max_find(x_last,x,x1,x2,2)-r,recx4=max_find(x_last,x,x1,x2,3)-r,
		recy1=max_find(y_last,y,y1,y2,0)+r,recy2=max_find(y_last,y,y1,y2,1)+r,
		recy3=max_find(y_last,y,y1,y2,2)-r,recy4=max_find(y_last,y,y1,y2,3)-r;//大概确定矩形四个点。*/
		if(distanceToLine(m.getx(),m.gety(),x_last,y_last,x1,y1)*distanceToLine(m.getx(),m.gety(),x,y,x2,y2)>=0)continue;
		if(distanceToLine(m.getx(),m.gety(),x_last,y_last,x,y)*distanceToLine(m.getx(),m.gety(),x1,y1,x2,y2)>=0)continue;
		t.setOfFind.push_back(count);
		double dx1=m.getx()-x_last,dy1=m.gety()-y_last,distance=dx1*dx+dy1*dy;
		if(distance<min_distance)
		{
			min_distance=distance;
			t.first_target=count;
		}//记入这个人的编号，若其角度差小于最小角度差，记此人为第一个目标，到下一个人。
	}
	return t;
}
#endif