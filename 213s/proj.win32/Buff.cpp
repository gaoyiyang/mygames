#include "Buff.h"
#include "Skill.h"
#include <iostream>

Buff::Buff(){

}
/*Buff���๹�캯����playerЯ��buff�Ľ�ɫָ�룬holdTime�����غ�����plies����*/
Buff::Buff(int holdTime,int plies){
	this->holdTime = holdTime;
	this->plies = plies;
}



cocos2d::Action *Buff::start(Player* player){

	return NULL;
}
void Buff::addPlies(int ap){

}


/*�����๹�캯��*/
SuoZu::SuoZu(int holdTime, int plies){
	this->holdTime = holdTime;
	this->plies = plies;
	this->buffName = SUOZU;
}
/*����BUFF���Ա����*/
cocos2d::Action* SuoZu::start(Player* player){
	
	//����һ������
	cocos2d::Action* action = NULL;
	//�������ƶ�������Ϊ0
	if (holdTime > 0){
		player->yidong = 0;
		holdTime--;
	}
	return action;
}
void SuoZu::addPlies(int ap){
	this->plies = 1;
}

/*�����๹�캯��*/
JianSu::JianSu(int holdTime, int plies){
	this->holdTime = holdTime;
	this->plies = plies;
	this->buffName = MyBuff::JIANSU;
}
/*����BUFF���Ա����*/
cocos2d::Action* JianSu::start(Player* player){
	//����һ������
	cocos2d::Action* action = NULL;
	return action;
}
void JianSu::addPlies(int ap){
	this->plies = 1;
}

/*�����ӳ�BUFF�๹�캯��*/
NengLiang::NengLiang(int holdTime, int plies){
	this->holdTime = holdTime;
	this->plies = plies;
	this->buffName = MyBuff::NENGLIANG;
	this->tempAtt = this->tempDef = this->tempMov;
}
/*�����ӳ�BUFF���Ա����*/
cocos2d::Action* NengLiang::start(Player* player){
	//����һ������
	cocos2d::Action* action = NULL;
	player->att -= tempAtt;
	player->def -= tempDef;
	player->mov -= tempMov;
	if (holdTime > 0){
		/*ÿһ������������������1%������ÿ50����������һ���ƶ�����*/
		float thisUp = (((float)Skill::power) / 100.0f);
		tempAtt = thisUp * player->att;
		tempDef = thisUp * player->def;
		tempMov = thisUp * 2;
		//holdTime--;���ô���
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





