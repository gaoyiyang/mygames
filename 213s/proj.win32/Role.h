#ifndef _ROLE_H__
#define _ROLE_H__

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include <string>



using namespace std;

class PlayerSkill
{
public:
	static void init();
private:
	string skillName;
	/*����Ч������()*/
};

class Role
{
};

class Player 
{
public:
	static void init();
	Player();
	Player(int n);
	Player add(string n, string p, int hp, int a, int d, int s, int m);
	void setName(string);
	void setPictureName(string);
	void setMov(int);
	void setTag(int);
	void setHp(int);
	void setAtt(int);
	void setDef(int);
	void setSp(int);
	void setPos(int, int);
	void setSkill(int *);
	void setNSkill(int *);
	int attack();
	void setAtMov(int);
	void setBuff(int, int);
	
	int skillNames[5];

	int yidong = 1;
	int gongji = 1;
	int jineng = 1;

	string name;
	string pictureName;
	int tag;
	int hp;
	int maxHp;
	int att;
	int def;
	int sp;
	int mov;
	int atMov;

	//��Ļ�����Լ�cocos����
	int wx;
	int wy;
	float x;
	float y;
	float sDef;//�������ṩ�ļ���ֵ
	int skill[12];
	int nSkill[5][2];
	int buff[100][2];//����buff��ţ�buff�ӳ�Ч���Լ�buff����ʱ��
	//PlayerSkill psk;//����
	//װ��,buff
};

void gameLoad();
void gameSave();
void newGame();
Player d1(int Lv, int x, int y, int tag);//����һ��
Player d2(int Lv, int x, int y, int tag);//����2��
Player boss2(int Lv, int x, int y, int tag);//boss2��
Player liaoji0(int Lv, int x, int y, int tag);//�Ż�0
Player liaoji1(int Lv, int x, int y, int tag);//�Ż�1
Player liaoji2(int Lv, int x, int y, int tag);//�Ż�2
Player liaoji3(int Lv, int x, int y, int tag);//�Ż�3
string getMapFile(int);
void setPlayers(int);

#endif //_ROLE_H__

