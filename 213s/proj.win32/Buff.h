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
	//����ö������MyBuffΪ��ɫ����һ��buff
	static Buff* getBuff(MyBuff, int,int plies);
	//ִ�к���
	virtual cocos2d::Action* start(Player*);
	//���BUFF����
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
����
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
////����ö������MyBuffΪ��ɫ����һ��buff
//Buff* getBuff(MyBuff, int, Player*);
//}
#endif //_BUFF_H__



