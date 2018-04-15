#include "Buff.h"
#include "Skill.h"
#include <iostream>

Buff::Buff(){

}
/*Buff基类构造函数，player携带buff的角色指针，holdTime持续回合数，plies层数*/
Buff::Buff(int holdTime,int plies){
	this->holdTime = holdTime;
	this->plies = plies;
}



cocos2d::Action *Buff::start(Player* player){

	return NULL;
}
void Buff::addPlies(int ap){

}


/*锁足类构造函数*/
SuoZu::SuoZu(int holdTime, int plies){
	this->holdTime = holdTime;
	this->plies = plies;
	this->buffName = SUOZU;
}
/*锁足BUFF类成员函数*/
cocos2d::Action* SuoZu::start(Player* player){
	
	//返回一个动作
	cocos2d::Action* action = NULL;
	//将人物移动次数降为0
	if (holdTime > 0){
		player->yidong = 0;
		holdTime--;
	}
	return action;
}
void SuoZu::addPlies(int ap){
	this->plies = 1;
}

/*减速类构造函数*/
JianSu::JianSu(int holdTime, int plies){
	this->holdTime = holdTime;
	this->plies = plies;
	this->buffName = MyBuff::JIANSU;
}
/*减速BUFF类成员函数*/
cocos2d::Action* JianSu::start(Player* player){
	//返回一个动作
	cocos2d::Action* action = NULL;
	return action;
}
void JianSu::addPlies(int ap){
	this->plies = 1;
}

/*能量加成BUFF类构造函数*/
NengLiang::NengLiang(int holdTime, int plies){
	this->holdTime = holdTime;
	this->plies = plies;
	this->buffName = MyBuff::NENGLIANG;
	this->tempAtt = this->tempDef = this->tempMov;
}
/*能量加成BUFF类成员函数*/
cocos2d::Action* NengLiang::start(Player* player){
	//返回一个动作
	cocos2d::Action* action = NULL;
	player->att -= tempAtt;
	player->def -= tempDef;
	player->mov -= tempMov;
	if (holdTime > 0){
		/*每一点能量提升攻击防御1%，并且每50点能量提升一点移动距离*/
		float thisUp = (((float)Skill::power) / 100.0f);
		tempAtt = thisUp * player->att;
		tempDef = thisUp * player->def;
		tempMov = thisUp * 2;
		//holdTime--;永久存在
	}
	return action;
}
void NengLiang::addPlies(int ap){
	this->plies = 1;
}



Buff* Buff::getBuff(MyBuff mb, int holdTime, int plies){
	Buff* buff = NULL;
	switch (mb)
	{
	case SUOZU:
		buff = new SuoZu(holdTime, plies);
		break;
	case JIANSU:
		break;
	default:
		break;
	}
	return buff;
}





