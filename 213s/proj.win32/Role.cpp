#include "Role.h"
#include "MapTest.h"
#include "AppDelegate.h"
#include "Role.h"
#include "HelloWorldScene.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include <fstream>
#include <iostream>
#include <sstream>

USING_NS_CC;

using namespace cocostudio::timeline;

extern int nowNum;


extern int winTag[100][100];
extern ssp allPlayer[20];
extern int skillOver;
extern int iMove[3];//ǰ��Ϊ�ƶ��˵ı�ţ�����Ϊ�ƶ��ķ�λ


Player::Player()
{
}

//void Player::startTime(cocos2d::Layer* layer){
//	for (int j = 0; j < 100; j++){
//		if (this->buff[j][1] > 0){//��������buff�б�
//			switch (j){//buffʵ�ֹ���
//			case 0:{//���0��ѣ��	
//					   this->yidong = this->gongji = this->jineng = 0;
//			}break;
//			case 1:{//���1������
//					   this->yidong = 0;
//			}break;
//			case 2:{//���2�����ٹ⻷��ȫ������ƶ�����-1
//					   if (this->name == "player3"){
//						   for (int lj = 4; lj < 20; lj++){
//							   if (allPlayer[lj].p.buff[3][1] <= 0){
//								   allPlayer[lj].p.setBuff(3, 1);
//							   }
//						   }
//					   }
//			}
//			case 3:{//���3�����ٹ⻷����debuff
//					   if (this->buff[j][0] == 1){
//						   this->setMov(this->mov - 1);
//						   this->buff[j][0] = 0;
//					   }
//			}break;
//			case 4:{//���4�����ƹ⻷��ȫԱ�ж�ʱ������ʦ��ǰ�����ָ�Ѫ��
//					   if (this->name == "player3"){
//						   for (int lj = 0; lj < 4; lj++){
//							   if (allPlayer[lj].p.buff[5][1] <= 0){
//								   allPlayer[lj].p.setBuff(5, 1);
//							   }
//						   }
//					   }
//			}
//			case 5:{//���5�����ƹ⻷����buff
//					   if (this->buff[j][0] == 1){
//						   if (true){
//							   this->hp += allPlayer[3].p.hp * 0.05;
//							   if (this->hp > this->maxHp) this->hp = this->maxHp;
//							   this->buff[j][0] = 0;
//						   }
//					   }
//			}break;
//			case 52:{//���52����λ�ƶ���ͨ������Ż�λ�ã����Լ���������������Ż��ϣ��ٽ����ƶ�
//						if (true){//�����Լ��Ļغ�
//							if (this->buff[j][0] == 1){//״̬��־δ��0
//								float lx[3] = { -1.0, -1.0, -1.0 };
//								float ly[3] = { -1.0, -1.0, -1.0 };
//								int lwx, lwy;
//								for (int li = 0; li < 20; li++){
//									for (int lj = 0; lj < 3; lj++){
//										if (allPlayer[li].p.name == "liaoji" + nToS(lj) && allPlayer[li].a != 0){
//											lx[lj] = allPlayer[li].p.x;
//											ly[lj] = allPlayer[li].p.y;
//										}
//									}
//								}
//								int a = rand() % 3;
//								if (lx[a] != -1.0 && ly[a] != -1.0){
//									lwx = lx[a] / 30;
//									lwy = ly[a] / 30;
//									if (winTag[lwx + 1][lwy] == 0)lwx += 1;
//									else if (winTag[lwx - 1][lwy] == 0)lwx -= 1;
//									else if (winTag[lwx][lwy + 1] == 0)lwy += 1;
//									else if (winTag[lwx][lwy - 1] == 0)lwy -= 1;
//									else {
//										lwx = this->wx;
//										lwy = this->wy;
//									}
//									if (lwx > 23 || lwx < 0 || lwy > 17 || lwy < 0){
//										lwx = this->wx;
//										lwy = this->wy;
//									}
//								}
//								else {
//									lwx = this->wx;
//									lwy = this->wy;
//								}
//								//�ƶ���ɫ
//								//����δ���
//								winTag[this->wx][this->wy] = 0;
//								this->setPos(lwx, lwy);
//								winTag[this->wx][this->wy] = this->tag;
//								layer->getChildByTag(this->tag)->setPosition(ccp(this->x, this->y));
//							}
//						}
//			}break;
//			case 53:{//���53�������⻷���Ż�0��Χ2��λ�����ƶ��������1
//						if (this->name == "liaoji0"){
//							for (int lj = 0; lj < 4; lj++){
//								if (abs(allPlayer[lj].p.wx - this->wx) + abs(allPlayer[lj].p.wy - this->wy) <= 2
//									&& allPlayer[lj].p.buff[54][1] <= 0){
//									allPlayer[lj].p.setBuff(54, 1);
//								}
//							}
//						}
//			}break;
//			case 54:{//���54�������⻷����debuff
//						if (this->buff[j][0] == 1){
//							this->setMov(this->mov - 1);
//							this->buff[j][0] = 0;
//						}
//			}break;
//			case 55:{//���55�����չ⻷���Ż�1��Χ2��λ���������е���ÿ�غ���ʧ5%��ǰ����
//						if (this->name == "liaoji1"){
//							for (int lj = 0; lj < 4; lj++){
//								if (abs(allPlayer[lj].p.wx - this->wx) + abs(allPlayer[lj].p.wy - this->wy) <= 2
//									&& allPlayer[lj].p.buff[56][1] <= 0){
//									allPlayer[lj].p.setBuff(56, 1);
//								}
//							}
//						}
//			}break;
//			case 56:{//���56�����չ⻷����debuff
//						this->hp = this->hp - (this->hp * 0.05) * this->sDef;
//			}break;
//			case 57:{//���53����ʴ�⻷���Ż�2��Χ2��λ�������3������
//						if (this->name == "liaoji2"){
//							for (int lj = 0; lj < 4; lj++){
//								if (abs(allPlayer[lj].p.wx - this->wx) + abs(allPlayer[lj].p.wy - this->wy) <= 2
//									&& allPlayer[lj].p.buff[58][1] <= 0){
//									allPlayer[lj].p.setBuff(58, 1);
//								}
//							}
//						}
//			}break;
//			case 58:{//���58����ʴ�⻷����debuff
//						if (this->buff[j][0] == 1){
//							this->setDef(this->def - 3);
//							this->buff[j][0] = 0;
//						}
//			}break;
//			}
//
//		}
//	}
//}

void newGame()
{
	ifstream iFin;
	ofstream oFin;
	string str;
	iFin.open("..\\sav.txt", ios::out);
	if (!iFin.is_open()){
		iFin.close();
		oFin.open("..\\sav.txt");
		str = "player0 rw00.csb 100001 1000 240 8 20 3 2 101 102 103 104 105 106 107 108 109 110 111 112 0 0 0 0 0";
		oFin << str << "\n";
		str = "player1 rw01.csb 100002 1200 200 10 18 4 1 205 207 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0";
		oFin << str << "\n";
		str = "player2 rw02.csb 100003 950 250 6 19 2 3 309 312 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0";
		oFin << str << "\n";
		str = "player3 rw03.csb 100004 1000 230 10 18 4 2 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0";
		oFin << str << "\n";
		oFin.close();
	}
}

void gameLoad()
{
	for (int i = 0; i < 20; i++){
		allPlayer[i].a = 0;
		for (int j = 0; j < 100; j++){
			allPlayer[i].p.buff[j][0] = allPlayer[i].p.buff[j][1] = 0;
		}
	}
	newGame();
	ifstream fin;
	fin.open("..\\sav.txt", ios::in);
	char line[1024];
	string name, pictureName;
	int hp, att, def, sp, mov, tag, atMov;
	int skill[12];
	int nSkill[5];
	while (fin.getline(line, sizeof(line))){
		stringstream ss(line);
		ss >> name;
		ss >> pictureName;
		ss >> tag;
		ss >> hp;
		ss >> att;
		ss >> def;
		ss >> sp;
		ss >> mov;
		ss >> atMov;
		for (int i = 0; i < 12; i++){
			ss >> skill[i];
		}
		for (int i = 0; i < 5; i++){
			ss >> nSkill[i];
		}
		for (int i = 0; i < 4; i++){
			if (allPlayer[i].a == 0){
				allPlayer[i].a = 1;
				allPlayer[i].p.setName(name);
				allPlayer[i].p.setPictureName(pictureName);
				allPlayer[i].p.setTag(tag);
				allPlayer[i].p.setHp(hp);
				allPlayer[i].p.setAtt(att);
				allPlayer[i].p.setDef(def);
				allPlayer[i].p.setSp(sp);
				allPlayer[i].p.setMov(mov);
				allPlayer[i].p.setAtMov(atMov);
				allPlayer[i].p.setSkill(skill);
				allPlayer[i].p.setNSkill(nSkill);
				break;
			}
		}
	}
	fin.close();
}

void gameSave()
{
	ofstream fin;
	fin.open("..\\sav.txt", ios::out | ios::trunc);//��д��ʽ���ļ�������ļ����ڣ�������Ϊ0
	for (int i = 0; i < 4; i++){
		fin << allPlayer[i].p.name << " ";
		fin << allPlayer[i].p.pictureName << " ";
		fin << allPlayer[i].p.tag << " ";
		fin << allPlayer[i].p.hp << " ";
		fin << allPlayer[i].p.att << " ";
		fin << allPlayer[i].p.def << " ";
		fin << allPlayer[i].p.sp << " ";
		fin << allPlayer[i].p.mov << " ";
		fin << allPlayer[i].p.atMov << " ";
		for (int j = 0; j < 12; j++)fin << allPlayer[i].p.skill[j] << " ";
		for (int j = 0; j < 5; j++)fin << allPlayer[i].p.nSkill[j][0] << " ";
		fin << "\n";
	}
	fin.close();
}

void Player::setName(string s){ this->name = s; }
void Player::setPictureName(string s){ this->pictureName = s; }
void Player::setMov(int i){ this->mov = i; }
void Player::setTag(int i){ this->tag = i; }
void Player::setHp(int h){ this->hp = this->maxHp = h; }
void Player::setAtt(int a){ this->att = a; }
void Player::setDef(int d){ this->def = d; this->sDef = (1.0 - (float)d / (10 + d)); }
void Player::setSp(int s){ this->sp = s; }
void Player::setPos(int x, int y){ this->wx = x; this->wy = y; this->x = x * 30; this->y = y * 30; }
void Player::setAtMov(int n){ this->atMov = n; }
void Player::setSkill(int *n){ for (int i = 0; i < 12; i++)this->skill[i] = n[i]; }
void Player::setNSkill(int *n){ 
	for (int i = 0; i < 5; i++)
	{
		this->nSkill[i][0] = this->nSkill[i][1] = 0;
	}
	for (int i = 0; i < 5; i++)
	{
		this->nSkill[i][0] = n[i];
		string str = nToS(n[i]);
		str[0] = '0';
		int num = sToN(str);
		if (num % 4 != 0)this->nSkill[i][1] = 5;
		else if ( this->nSkill[i][0] != 0 ) this->nSkill[i][1] = 2;
	}
}
void Player::setBuff(int n, int time){

	this->buff[n][0] = 1;
	if (this->buff[n][1] <= time) this->buff[n][1] = time;
}

Player d1(int Lv, int x, int y, int tag)
{
	Player p;
	p.setName("xiaobing1");
	p.setPictureName("rw04.csb");
	p.setHp(50 + Lv * 30);
	p.setAtt(5 + Lv * 5);
	p.setAtMov(2);
	p.setDef(5);
	p.setSp(18);
	p.setPos(x, y);
	p.setTag(tag);
	p.setMov(2);
	return p;
}

Player d2(int Lv, int x, int y, int tag)
{
	Player p;
	p.setName("xiaobing2");
	p.setPictureName("rw04.csb");
	p.setHp(50 + Lv * 30);
	p.setAtt(5 + Lv * 5);
	p.setAtMov(1);
	p.setDef(5);
	p.setSp(20);
	p.setPos(x, y);
	p.setTag(tag);
	p.setMov(3);
	return p;
}

Player boss2(int Lv, int x, int y, int tag)
{
	Player p;
	p.setName("boss2");
	p.setPictureName("rw04.csb");
	p.setHp(100 + Lv * 100);
	p.setAtt(25 + Lv * 5);
	p.setAtMov(200);
	p.setDef(8);
	p.setSp(17);
	p.setPos(x, y);
	p.setTag(tag);
	p.setMov(4);
	p.setBuff(52, 999);
	return p;
}
Player liaoji0(int Lv, int x, int y, int tag)
{
	Player p;
	p.setName("liaoji0");
	p.setPictureName("rw04.csb");
	p.setHp(200 + Lv * 50);
	p.setAtt(0);
	p.setAtMov(0);
	p.setDef(0);
	p.setSp(5);
	p.setPos(x, y);
	p.setTag(tag);
	p.setMov(2);
	p.setBuff(53, 999);
	return p;
}

Player liaoji1(int Lv, int x, int y, int tag)
{
	Player p;
	p.setName("liaoji1");
	p.setPictureName("rw04.csb");
	p.setHp(200 + Lv * 50);
	p.setAtt(0);
	p.setAtMov(0);
	p.setDef(0);
	p.setSp(5);
	p.setPos(x, y);
	p.setTag(tag);
	p.setMov(2);
	p.setBuff(55, 999);
	return p;
}
Player liaoji2(int Lv, int x, int y, int tag)
{
	Player p;
	p.setName("liaoji2");
	p.setPictureName("rw04.csb");
	p.setHp(200 + Lv * 50);
	p.setAtt(0);
	p.setAtMov(0);
	p.setDef(0);
	p.setSp(5);
	p.setPos(x, y);
	p.setTag(tag);
	p.setMov(2);
	p.setBuff(57, 999);
	return p;
}
Player liaoji3(int Lv, int x, int y, int tag)
{
	Player p;
	p.setName("liaoji3");
	p.setPictureName("rw04.csb");
	p.setHp(200 + Lv * 50);
	p.setAtt(0);
	p.setAtMov(0);
	p.setDef(0);
	p.setSp(5);
	p.setPos(x, y);
	p.setTag(tag);
	p.setMov(2);
	return p;
}





