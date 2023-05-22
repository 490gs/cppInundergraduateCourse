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

inline double distanceToLine(double x0,double y0,double x1,double x2,double y1,double y2)//��㵽�߾���
{
	return ( (x1-x2)*(y0-y1)-(y1-y2)*(x0-x1) )/pow(pow(x1-x2,2)+pow(y1-y2,2),0.5);
}
double& max_find(double &a1,double &a2,double &a3,double &a4,int b)//���ص�b���a��
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
	double x,y,a,r;						//��ǰ��������ͽǶȣ��ڵ��뾶��
	int type,master[2],enemy;			//���ͺͰ󶨵��˵��֣�-1�����֣�1�����֡��������͡�
	double speed,damage,range;			//�ٶȣ��˺�����̡�
public:
	shell(int,int,int);			//�����ڵ���
	target shoot();			//���������
	target find();			//��Ŀ�꺯����
	inline int getMaster(){return master[0];}//��������˱�š�
	inline bool isHasRange(){return range!=0;};//�ж��Ƿ���ʣ����̺�����
	void display();
};vector<shell>setOfShell;		//���ڴ�ų����������ڵ���Ϣ��
class sword
{
private:
	double x,y,a,r,x_last,y_last,a_last;		//��ǰ��������ͽǶȣ��������ղź�������ͽǶȡ�
	int type,master[2],enemy;					//���ͺͰ󶨵��˵��֣�-1�����֣�1�����֡��������͡�
	double damage,r_speed;						//�˺�����ת�ٶȡ�
public:
	sword(int,int,int);
	target cut();					//ն��������
	target find();				//��ת��Ŀ�꺯����
	target find_m();				//ƽ����Ŀ�꺯����
	inline int getMaster(){return master[0];}//�����ս��˱�š�
	void display();
};vector<sword>setOfSword;			//���ڴ�ų��������е�����Ϣ��


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
	if( (master[1]==1&&setOfMan[master[0]].state_r>=0)||(master[1]==-1&&setOfMan[master[0]].state_l>=0) )//����ս����ִ����ս�״̬��
	line(x,y,x+r*cos(a),y+r*sin(a));
}

//��������������ڵ��ٶ�״̬�͵������ͣ������˺�ֵ�����û�л����κ����壬��range��С��
target shell::shoot()
{
	double realSpeed=speed;
	target t=find();
	if(t.first_target==-1)//������û��Ŀ�ꡣ
	{
		if(realSpeed<=range)//�ٶ�С��ʣ����̣����ٶ����ǰ������̼�С��
		{
			x+=realSpeed*cos(a);
			y+=realSpeed*sin(a);
			range-=realSpeed;
		}
		else//�ٶȴ���ʣ����̣���ʣ�����ǰ������̹��㡣
		{
			x+=range*cos(a);
			y+=range*sin(a);
			range=0;
		}
		t.first_target=0;
		return t;//����0�˺���
	}
	else//��Ŀ�ꡣ
	{
		man m=setOfMan[find().first_target];//��Ŀ��Ϊm��
		double dx=m.getx()-x,dy=m.gety()-y,distance=dx*cos(a)+dy*sin(a),realDistance=distance;//�ڵ�ǰ����Ŀ�꣬������̹��㡣
		x+=realDistance*cos(a);
		y+=realDistance*sin(a);
		range=0;
		t.get_damage=damage;
		return t;//������ʵ�˺���
	}
}

//��Ҫ���man��state_r���Ż���
target sword::cut()
{
	if(setOfMan[master[0]].state_r<0)return t_empty;//����ս���û���ս������ؿ�target��
	x_last=x,y_last-y,a_last=a;	xya xya0=setOfMan[master[0]].getHand(master[1]);
	x=xya0.x;y=xya0.y;a=xya0.a;
	r_speed=a-a_last;//ת���ٶ�δ��ǰ����ղŽ�֮�
	target t=find_m();
	x=xya0.x;y=xya0.y;
	target t1=find();
	if(t.first_target==-1)//���ƶ�ʱû��Ŀ�ꡣ
	{
		t=t1;//��t1����t��
		if(t.first_target==-1)//��û��Ŀ�ꡣ
		{
			t.get_damage=0;
			return t;
		}
		else
		{
			t.get_damage=damage;
			return t;//������ʵ�˺���
		}
	}
	if(t1.first_target!=-1)//��ת��ʱ��Ŀ�ꡣ
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
			t.setOfFind.push_back(te);//�ϲ�����setOfFind��
	}
	if(t.first_target==-1)//��û��Ŀ�ꡣ
		{
			t.get_damage=0;
			return t;
		}
		else
		{
			t.get_damage=damage;
			return t;//������ʵ�˺���
		}
}


//��Ŀ�꺯��������״̬�͵������ͣ�����Ŀ��ṹ�塣
target shell::find()//Ӧ��ȥ��state��
{
	target t;t.first_target=-1;double min_distance=range*range;int count=-1;//Ԥ��Ԥ���ű�ţ���С����ƽ������������
	for(auto m:setOfMan)
	{
		/*�����ж���ˣ�˼·���Ƿ��ǵ��ˣ��Ƿ��Ѿ����У��Ƿ���ǰ���Ƿ�����������Ƿ񳬳��ٶȡ�*/
		++count;//��������1����0��ʼ������
		if(m.myself!=enemy)continue;//������ǵ��ˣ�����һ���ˡ�
		if(m.state<=0)continue;//��������ţ�����һ���ˡ�
		double dx=m.getx()-x,dy=m.gety()-y,distance=dx*dx+dy*dy;//�ڵ�����˵ĺ�����룬������룬�;���ƽ����
		if( distance<=pow(r+m.getr(),2) )
		{//�������С���ڵ����˰뾶�ͣ�������˵ı��,��������С����С�����Ǵ���Ϊ��һ��Ŀ�꣬����һ���ˡ�
			t.setOfFind.push_back(count);
			if(distance<min_distance)
			{
				min_distance=distance;
				t.first_target=count;
			}
			continue;
		}
		if(dx*cos(a)+dy*sin(a)<0)continue;//�����ǰ�˲����ڵ�ǰ�򣬵���һ���ˡ�
		if( pow(cos(a)*dy-sin(a)*dx,2)>r+m.getr() )continue;//�����ǰ�����ӵ��켣�ľ�������ڵ����˰뾶�ͣ�����һ���ˡ�
		if(distance>pow(speed,2))continue;//������������ʵ�ٶȣ�����һ���ˡ�
		if(distance>pow(range,2))continue;//��������������̣�����һ���ˡ�
		t.setOfFind.push_back(count);
		if(distance<min_distance)
			{
				min_distance=distance;
				t.first_target=count;
			}//������˵ı��,��������С����С�����Ǵ���Ϊ��һ��Ŀ�꣬����һ���ˡ�
	}
	return t;
}
target sword::find()
{
	/*�����ж���ˣ�˼·���Ƿ��ǵ��ˣ��Ƿ�����������Ƿ��Ѿ����У��Ƿ���ǰ���Ƿ񳬳��ٶȡ�*/
	target t;t.first_target=-1;int count=-1;double min_da=r_speed;//Ԥ��Ŀ��ṹ�壬����������С��ǡ�
	for(auto m:setOfMan)
	{
		++count;//��������1����0��ʼ������
		if(m.myself!=enemy)continue;//������ǵ��ˣ�����һ���ˡ�
		if(m.state<=0)continue;//��������ţ�����һ���ˡ�
		double dx=m.getx()-x,dy=m.gety()-y,distance=dx*dx+dy*dy;//��ת��������˵ĺ�����룬������룬�;���ƽ����
		double a_m=atan(dy/dx);if(dx<0)a_m+=PI;double da=a_m-a;//���������ת���ĵĽǶȣ����뽣�ļнǡ�
		if( distance>pow(r+m.getr(),2)|| dx*cos(a)+dy*sin(a)<0 )continue;//������������ʵ�뾶���˰뾶�ͻ��ڷ����򣬵���һ���ˡ�
		if( pow(cos(a)*dy-sin(a)*dx,2)<m.getr()&&distance<pow(r,2)+pow(m.getr(),2) )
		{//���������ľ���С���˰뾶���ҿ����е�����������˵ı�ţ�����ǶȲ�С����С�ǶȲ�Ǵ���Ϊ��һ��Ŀ�꣬����һ���ˡ�
			t.setOfFind.push_back(count);
			if(da<min_da)
			{
				min_da=da;
				t.first_target=count;
			}
			continue;
		}
		if(r_speed*da<0)continue;//�����ǰ�˲��ڽ�ǰ�򣬵���һ���ˡ�
		if(da>r_speed)continue;//����ת���ٶȣ�����һ���ˡ�
		t.setOfFind.push_back(count);
		if(da<min_da)
		{
			min_da=da;
			t.first_target=count;
		}//��������˵ı�ţ�����ǶȲ�С����С�ǶȲ�Ǵ���Ϊ��һ��Ŀ�꣬����һ���ˡ�
	}
	return t;
}
target sword::find_m()
{
	/*�����ж���ˣ�˼·������ƽ���ı��Σ��жϸ��������ڲ����ı����С�*/
	target t;t.first_target=-1;int count=-1;//Ԥ��Ŀ��ṹ�壬��������
	double dx=x-x_last,dy=y-y_last,min_distance=pow(dx,2)+pow(dy,2);
	for(auto m:setOfMan)
	{
		++count;//��������1����0��ʼ������
		if(m.myself!=enemy)continue;//������ǵ��ˣ�����һ���ˡ�
		if(m.state<=0)continue;//��������ţ�����һ���ˡ�
		double x1=x_last+r*cos(a_last),y1=y_last+r*sin(a_last);//�ղŽ�ĩ�ˡ�
		double x2=x+r*cos(a_last),y2=y+r*sin(a_last);//���ڽ�ĩ�ˡ�
		/*double recx1=max_find(x_last,x,x1,x2,0)+r,recx2=max_find(x_last,x,x1,x2,1)+r,
		recx3=max_find(x_last,x,x1,x2,2)-r,recx4=max_find(x_last,x,x1,x2,3)-r,
		recy1=max_find(y_last,y,y1,y2,0)+r,recy2=max_find(y_last,y,y1,y2,1)+r,
		recy3=max_find(y_last,y,y1,y2,2)-r,recy4=max_find(y_last,y,y1,y2,3)-r;//���ȷ�������ĸ��㡣*/
		if(distanceToLine(m.getx(),m.gety(),x_last,y_last,x1,y1)*distanceToLine(m.getx(),m.gety(),x,y,x2,y2)>=0)continue;
		if(distanceToLine(m.getx(),m.gety(),x_last,y_last,x,y)*distanceToLine(m.getx(),m.gety(),x1,y1,x2,y2)>=0)continue;
		t.setOfFind.push_back(count);
		double dx1=m.getx()-x_last,dy1=m.gety()-y_last,distance=dx1*dx+dy1*dy;
		if(distance<min_distance)
		{
			min_distance=distance;
			t.first_target=count;
		}//��������˵ı�ţ�����ǶȲ�С����С�ǶȲ�Ǵ���Ϊ��һ��Ŀ�꣬����һ���ˡ�
	}
	return t;
}
#endif