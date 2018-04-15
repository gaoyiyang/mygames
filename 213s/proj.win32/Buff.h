#ifndef _BUFF_H__
#define _BUFF_H__

#include "Role.h"
#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"

using namespace cocostudio::timeline;
using namespace cocos2d::ui;

enum MyBuff
{
	BUFF,SUOZU, JIANSU, NENGLIANG
};
class Buff
{
public:
	Buff(int holdTime,int plies);
	Buff();
	//根据枚举类型MyBuff为角色设置一个buff
	static Buff* getBuff(MyBuff, int,int plies);
	//执行函数
	virtual cocos2d::Action* start(Player*);
	//添加BUFF层数
	virtual void addPlies(int);
	MyBuff buffName = BUFF;
	int holdTime;
	int plies;
};
class BuffList{
public:
	BuffList();
	Buff *getBuff(int index);
	void add();
	void remove(int index);
private:
	Buff buffs[1];
};

/*
锁足
*/
class SuoZu : public Buff{
public:
	SuoZu(int holdTime, int plies);
	cocos2d::Action* start(Player*);
	void addPlies(int);
};
class JianSu : public Buff{
public:
	JianSu(int holdTime, int plies);
	cocos2d::Action* start(Player*);
	void addPlies(int);
};

class NengLiang : public Buff{
public:
	NengLiang(int holdTime, int plies);
	cocos2d::Action* start(Player*);
	void addPlies(int);
private:
	int tempAtt;
	int tempDef;
	int tempMov;
};
//namespace buff{
////根据枚举类型MyBuff为角色设置一个buff
//Buff* getBuff(MyBuff, int, Player*);
//}
#endif //_BUFF_H__



