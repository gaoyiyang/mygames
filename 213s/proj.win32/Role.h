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
	/*技能效果函数()*/
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

	//屏幕坐标以及cocos坐标
	int wx;
	int wy;
	float x;
	float y;
	float sDef;//防御力提供的减伤值
	int skill[12];
	int nSkill[5][2];
	int buff[100][2];//人物buff编号，buff加持效果以及buff持续时间
	//PlayerSkill psk;//技能
	//装备,buff
};

void gameLoad();
void gameSave();
void newGame();
Player d1(int Lv, int x, int y, int tag);//敌人一号
Player d2(int Lv, int x, int y, int tag);//敌人2号
Player boss2(int Lv, int x, int y, int tag);//boss2号
Player liaoji0(int Lv, int x, int y, int tag);//僚机0
Player liaoji1(int Lv, int x, int y, int tag);//僚机1
Player liaoji2(int Lv, int x, int y, int tag);//僚机2
Player liaoji3(int Lv, int x, int y, int tag);//僚机3
string getMapFile(int);
void setPlayers(int);

#endif //_ROLE_H__

